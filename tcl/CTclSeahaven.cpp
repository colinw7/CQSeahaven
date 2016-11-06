#include "std_c++.h"
#include "std_Xt.h"
#include "CTclCanvas/CTclCanvas.h"
#include "CTclSeahaven/CTclSeahaven.h"
#include "CSeahavenLib/CSeahaven.h"
#include "CSeahavenLib/CSeahavenStack.h"
#include "CSeahavenLib/CSeahavenPile.h"
#include "CSeahavenLib/CSeahavenWorkArea.h"
#include "CSeahavenLib/CSeahavenMove.h"
#include "CCard/CCard.h"
#include "CCard/CCardDeck.h"
#include "CXLib/CXGraphics.h"
#include "CXLib/CXDragWindow.h"
#include "CXLib/CXMachine.h"
#include "COS/COSTimer.h"

enum CSeahavenAreaType {
  NO_TYPE,
  STACK_TYPE,
  PILE_TYPE,
  WORK_AREA_TYPE
};

union CSeahavenArea {
  CSeahavenStack    *stack;
  CSeahavenPile     *pile;
  CSeahavenWorkArea *work_area;
};

class CTclSeahaven : public CTclCanvas {
 private:
  CSeahaven          seahaven_;
  CImagePtr          card_images_[52];
  int                card_width_;
  int                card_height_;
  int                card_dy_;
  int                stack_x_;
  int                stack_y_;
  int                pile_x_;
  int                pile_y_;
  int                work_x_;
  int                work_y_;
  CXDragWindow      *drag_window_;
  vector<CCard *>    drag_cards_;
  CSeahavenAreaType  drag_area_type_;
  CSeahavenArea      drag_area_;
  int                drag_dx_;
  int                drag_dy_;

 public:
  CTclSeahaven(CTclApp *app);
 ~CTclSeahaven();

  bool instanceCommand(int argc, char **argv);
  bool instanceConfigure(int argc, char **argv);

  void draw();

  void mousePress();
  void mouseMotion();
  void mouseRelease();
  void keyPress() { }
  void keyRelease() { }

 private:
  void drawStacks();
  void drawPiles();
  void drawWorkAreas();
  void drawCard(CCard *card, int x, int y);
  void drawOutline(int x, int y);

  CSeahavenStack    *getStack(int x, int y);
  CSeahavenPile     *getPile(int x, int y);
  CSeahavenWorkArea *getWorkArea(int x, int y);
};

CTclSeahavenCmd::
CTclSeahavenCmd(CTclApp *app) :
 CTclAppCanvasCmd(app)
{
  init();
}

CTclSeahavenCmd::
~CTclSeahavenCmd()
{
  delete seahaven_;
}

CTclAppCanvas *
CTclSeahavenCmd::
createInstance(CTclApp *app)
{
  seahaven_ = new CTclSeahaven(app);

  return (CTclAppCanvas *) seahaven_->getCanvasWidget();
}

CTclSeahaven::
CTclSeahaven(CTclApp *app) :
 CTclCanvas(app)
{
  CXMachineInst->enterDebugMode();

  //----

  CCardDeck *deck = seahaven_.getDeck();

  card_width_  = deck->getCardWidth ();
  card_height_ = deck->getCardHeight();

  card_dy_ = card_height_/3;

  CSeahavenStackMgr *stack_mgr = seahaven_.getStackMgr();

  int num_stacks = stack_mgr->getNumStacks();

  stack_x_ = 2;
  stack_y_ = 8;

  pile_x_ = num_stacks*card_width_ + 15;
  pile_y_ = stack_y_;

  work_x_ = pile_x_;
  work_y_ = pile_y_ + 2*card_height_ + 20;

  //----

  drag_window_ = NULL;
}

CTclSeahaven::
~CTclSeahaven()
{
}

bool
CTclSeahaven::
instanceCommand(int argc, char **argv)
{
  if (argc == 1 && strcmp(argv[0], "solve") == 0) {
    CSeahavenMoveSetSet move_set_set;

    if (seahaven_.solve(move_set_set)) {
      {
        int num_move_sets = move_set_set.getNumMoveSets();

        for (int i = 0; i < num_move_sets; i++) {
          CSeahavenMoveSet *move_set = move_set_set.getMoveSet(i);

          move_set->execute();

          redraw();

          COSTimer::msleep(100);
        }
      }

      {
        move_set_set.undo();

        redraw();
      }
    }

    return true;
  }
  else if (argc == 1 && strcmp(argv[0], "newgame") == 0) {
    seahaven_.deal();

    redraw();

    return true;
  }

  return false;
}

bool
CTclSeahaven::
instanceConfigure(int, char **)
{
  return false;
}

void
CTclSeahaven::
draw()
{
  setBackground(CRGB(0.1, 0.5, 0.1));

  clear();

  drawStacks();
  drawPiles();
  drawWorkAreas();
}

void
CTclSeahaven::
drawStacks()
{
  int x = stack_x_;
  int y = stack_y_;

  CSeahavenStackMgr *stack_mgr = seahaven_.getStackMgr();

  int num_stacks = stack_mgr->getNumStacks();

  for (int i = 0; i < num_stacks; i++) {
    y = stack_y_;

    CSeahavenStack *stack = stack_mgr->getStack(i);

    int num_cards = stack->getNumCards();

    for (int j = 0; j < num_cards; j++) {
      CCard *card = stack->peek(j);

      drawCard(card, x, y);

      y += card_dy_;
    }

    x += card_width_ + 1;
  }
}

void
CTclSeahaven::
drawPiles()
{
  int x = pile_x_;
  int y = pile_y_;

  CSeahavenPileMgr *pile_mgr = seahaven_.getPileMgr();

  int num_piles = pile_mgr->getNumPiles();

  for (int i = 0; i < num_piles; i++) {
    CSeahavenPile *pile = pile_mgr->getPile(i);

    if (! pile->isEmpty()) {
      CCard *card = pile->peek();

      drawCard(card, x, y);
    }
    else
      drawOutline(x, y);

    if (i == 0 || i == 2)
      x += card_width_ + 1;
    else {
      x  = pile_x_;
      y += card_height_ + 1;
    }
  }
}

void
CTclSeahaven::
drawWorkAreas()
{
  int x = work_x_;
  int y = work_y_;

  CSeahavenWorkAreaMgr *work_area_mgr = seahaven_.getWorkAreaMgr();

  int num_work_areas = work_area_mgr->getNumWorkAreas();

  for (int i = 0; i < num_work_areas; i++) {
    CCard *card = work_area_mgr->getCard(i);

    drawOutline(x, y);

    if (card != NULL)
      drawCard(card, x, y);

    if (i == 0 || i == 2)
      x += card_width_ + 1;
    else {
      x  = work_x_;
      y += card_height_ + 1;
    }
  }
}

void
CTclSeahaven::
drawCard(CCard *card, int x, int y)
{
  int ind = card->getIndex();

  if (! card_images_[ind])
    card_images_[ind] = card->getImage();

  drawImage(card_images_[ind], x, y);
}

void
CTclSeahaven::
drawOutline(int x, int y)
{
  setForeground(CRGB(1,1,1));

  drawRectangle(x, y, card_width_, card_height_);
}

void
CTclSeahaven::
mousePress()
{
  int x = getMouseX();
  int y = getMouseY();

  CSeahavenStack    *stack     = getStack(x, y);
  CSeahavenWorkArea *work_area = getWorkArea(x, y);

  drag_cards_.clear();

  drag_area_type_ = NO_TYPE;

  int x1 = x;
  int y1 = y;

  if (stack != NULL) {
    int num = stack->getNum();

    x1 = stack_x_ + num*(card_width_ + 1);

    if (! stack->isEmpty()) {
      drag_area_type_  = STACK_TYPE;
      drag_area_.stack = stack;

      drag_area_.stack->getTopCards(drag_cards_);
    }

    y1 = stack_y_;

    y1 += (stack->getNumCards() - drag_cards_.size())*card_dy_;
  }
  else if (work_area != NULL) {
    int num = work_area->getNum();

    if (num == 0 || num == 1) {
      x1 = work_x_ + num*(card_width_ + 1);
      y1 = work_y_;
    }
    else {
      x1 = work_x_ + (num - 2)*(card_width_ + 1);
      y1 = work_y_ + card_height_ + 1;
    }

    if (! work_area->isEmpty()) {
      drag_area_type_      = WORK_AREA_TYPE;
      drag_area_.work_area = work_area;

      drag_cards_.push_back(work_area->peek());
    }
  }

  if (drag_area_type_ == NO_TYPE)
    return;

  int num_cards = drag_cards_.size();

  if (drag_window_ == NULL)
    drag_window_ = new CXDragWindow(getCXScreen(), getXWindow(), 100, 100);

  drag_window_->setSize(card_width_, card_height_ + (num_cards - 1)*card_dy_);

  drag_window_->dragStart(x1, y1);

  drag_dx_ = x1 - x;
  drag_dy_ = y1 - y;

  y1 = 0;

  for (int i = num_cards - 1; i >= 0; --i) {
    int ind = drag_cards_[i]->getIndex();

    drag_window_->getCXWindow()->drawImage(card_images_[ind], 0, y1);

    y1 += card_dy_;
  }

  if (drag_area_type_ == STACK_TYPE) {
    for (int i = 0; i < num_cards; ++i)
      drag_area_.stack->pop();
  }
  else
    drag_area_.work_area->pop();

  redraw();
}

void
CTclSeahaven::
mouseMotion()
{
  if (drag_cards_.empty())
    return;

  int x = getMouseX();
  int y = getMouseY();

  drag_window_->dragMotion(x + drag_dx_, y + drag_dy_);
}

void
CTclSeahaven::
mouseRelease()
{
  if (drag_cards_.empty())
    return;

  int x = getMouseX();
  int y = getMouseY();

  drag_window_->dragEnd(x + drag_dx_, y + drag_dy_);

  int num_cards = drag_cards_.size();

  if (drag_area_type_ == STACK_TYPE) {
    for (int i = num_cards - 1; i >= 0; --i)
      drag_area_.stack->push(drag_cards_[i]);
  }
  else
    drag_area_.work_area->push(drag_cards_[0]);

  CSeahavenStack    *stack     = getStack(x, y);
  CSeahavenPile     *pile      = getPile(x, y);
  CSeahavenWorkArea *work_area = getWorkArea(x, y);

  if (drag_area_type_ == STACK_TYPE) {
    int num = drag_area_.stack->getNumConsecutive();

    if      (stack != NULL) {
      if (seahaven_.moveStackToStackValid(drag_area_.stack, stack)) {
        CSeahavenMoveSet move_set;

        seahaven_.addStackToStack(&move_set, drag_area_.stack, stack, num);

        move_set.execute();
      }
    }
    else if (work_area != NULL) {
      if (seahaven_.moveStackToWorkAreaValid(drag_area_.stack)) {
        CSeahavenMoveSet move_set;

        seahaven_.addStackToWork(&move_set, drag_area_.stack, num);

        move_set.execute();
      }
    }
    else if (pile != NULL) {
      if (seahaven_.moveStackToPileValid(drag_area_.stack)) {
        CSeahavenMoveSet move_set;

        CSeahavenPileMgr *pile_mgr = seahaven_.getPileMgr();

        CSeahavenPile *pile = pile_mgr->getPile(drag_cards_[0]->getSuit());

        seahaven_.addStackToPile(&move_set, drag_area_.stack, pile, num);

        move_set.execute();
      }
    }
  }
  else {
    if (stack != NULL) {
      if (seahaven_.moveWorkAreaToStackValid(drag_area_.work_area, stack)) {
        CSeahavenMoveSet move_set;

        seahaven_.addWorkToStack(&move_set, drag_area_.work_area, stack);

        move_set.execute();
      }
    }
    else if (pile != NULL) {
      if (seahaven_.moveWorkAreaToPileValid(drag_area_.work_area)) {
        CSeahavenMoveSet move_set;

        CSeahavenPileMgr *pile_mgr = seahaven_.getPileMgr();

        CSeahavenPile *pile = pile_mgr->getPile(drag_cards_[0]->getSuit());

        seahaven_.addWorkToPile(&move_set, drag_area_.work_area, pile);

        move_set.execute();
      }
    }
  }

  redraw();
}

CSeahavenStack *
CTclSeahaven::
getStack(int x, int /*y*/)
{
  CSeahavenStackMgr *stack_mgr = seahaven_.getStackMgr();

  int num_stacks = stack_mgr->getNumStacks();

  int x1 = stack_x_;
  int x2 = stack_x_ + num_stacks*(card_width_ + 1) - 1;

  if (x < x1 || x > x2)
    return NULL;

  int stack_num = (x - x1)/(card_width_ + 1);

  return stack_mgr->getStack(stack_num);
}

CSeahavenPile *
CTclSeahaven::
getPile(int x, int y)
{
  CSeahavenPileMgr *pile_mgr = seahaven_.getPileMgr();

  if (x < pile_x_ || x >= pile_x_ + 2*card_width_)
    return NULL;

  if (y < pile_y_ || y >= pile_y_ + 2*card_height_)
    return NULL;

  int i = (x - pile_x_)/card_width_;
  int j = (y - pile_y_)/card_height_;

  return pile_mgr->getPile(i + 2*j);
}

CSeahavenWorkArea *
CTclSeahaven::
getWorkArea(int x, int y)
{
  CSeahavenWorkAreaMgr *work_area_mgr = seahaven_.getWorkAreaMgr();

  if (x < work_x_ || x >= work_x_ + 2*card_width_)
    return NULL;

  if (y < work_y_ || y >= work_y_ + 2*card_height_)
    return NULL;

  int i = (x - work_x_)/card_width_;
  int j = (y - work_y_)/card_height_;

  return work_area_mgr->getWorkArea(i + 2*j);
}
