#include <CQSeahavenCanvas.h>

#include <CSeahaven.h>
#include <CSeahavenStack.h>
#include <CSeahavenPile.h>
#include <CSeahavenWorkArea.h>
#include <CSeahavenMove.h>

#include <CCard.h>
#include <CCardDeck.h>
#include <COSTimer.h>
#include <CEvent.h>
#include <CQImageUtil.h>
#include <CQUtil.h>

#include <QApplication>
#include <QPainter>
#include <QMouseEvent>

class CQDragWindow : public QWidget {
 private:
  CQSeahavenCanvas *canvas_;

 public:
  CQDragWindow(CQSeahavenCanvas *canvas);
 ~CQDragWindow();

  void paintEvent(QPaintEvent *event);
};

CQSeahavenCanvas::
CQSeahavenCanvas(QWidget *parent) :
 QWidget(parent)
{
  for (int i = 0; i < 52; i++)
    card_images_[i] = CImagePtr();

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

  drag_dx_ = 0;
  drag_dy_ = 0;

  //----

  dragWindow_ = NULL;
}

CQSeahavenCanvas::
~CQSeahavenCanvas()
{
}

void
CQSeahavenCanvas::
solve()
{
  CSeahavenMoveSetSet move_set_set;

  if (seahaven_.solve(move_set_set)) {
    {
      int num_move_sets = move_set_set.getNumMoveSets();

      for (int i = 0; i < num_move_sets; i++) {
        CSeahavenMoveSet *move_set = move_set_set.getMoveSet(i);

        move_set->execute();

        update();

        qApp->processEvents();

        COSTimer::msleep(100);
      }
    }

    {
      move_set_set.undo();

      update();
    }
  }
}

void
CQSeahavenCanvas::
newGame()
{
  seahaven_.deal();

  update();
}

void
CQSeahavenCanvas::
paintEvent(QPaintEvent *)
{
  QPainter painter(this);

  painter.fillRect(rect(), QBrush(QColor(26,128,26)));

  painter_ = &painter;

  drawStacks   ();
  drawPiles    ();
  drawWorkAreas();

  painter_ = NULL;
}

void
CQSeahavenCanvas::
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
CQSeahavenCanvas::
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
CQSeahavenCanvas::
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
CQSeahavenCanvas::
drawCard(CCard *card, int x, int y)
{
  int ind = card->getIndex();

  if (! card_images_[ind].isValid()) {
    card_images_[ind] = card->getImage();

    card_images_[ind]->convertToRGB();
  }

  painter_->drawImage(QPoint(x, y), CQImageUtil::toQImage(card_images_[ind]));
}

void
CQSeahavenCanvas::
drawOutline(int x, int y)
{
  painter_->setPen(QColor(255,255,255));

  painter_->drawRect(QRect(x, y, card_width_, card_height_));
}

void
CQSeahavenCanvas::
mousePressEvent(QMouseEvent *event)
{
  const QPoint &pos = event->pos();

  CSeahavenStack    *stack     = getStack   (pos.x(), pos.y());
  CSeahavenWorkArea *work_area = getWorkArea(pos.x(), pos.y());

  drag_cards_.clear();

  drag_area_type_ = NO_TYPE;

  int x1 = pos.x();
  int y1 = pos.y();

  if      (stack != NULL) {
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
  else
    return;

  if (drag_area_type_ == NO_TYPE)
    return;

  int num_cards = drag_cards_.size();

  if (dragWindow_ == NULL) {
    dragWindow_ = new CQDragWindow(this);

    dragWindow_->hide();
  }

  dragWindow_->resize(card_width_, card_height_ + (num_cards - 1)*card_dy_);

  dragWindow_->move(x1, y1);

  dragWindow_->show();

  dragWindow_->update();

  drag_dx_ = x1 - pos.x();
  drag_dy_ = y1 - pos.y();

  if (drag_area_type_ == STACK_TYPE) {
    for (int i = 0; i < num_cards; ++i)
      drag_area_.stack->pop();
  }
  else
    drag_area_.work_area->pop();

  update();
}

void
CQSeahavenCanvas::
mouseMoveEvent(QMouseEvent *event)
{
  const QPoint &pos = event->pos();

  if (drag_cards_.empty())
    return;

  dragWindow_->move(pos.x() + drag_dx_, pos.y() + drag_dy_);
}

void
CQSeahavenCanvas::
mouseReleaseEvent(QMouseEvent *event)
{
  const QPoint &pos = event->pos();

  if (drag_cards_.empty())
    return;

  dragWindow_->move(pos.x() + drag_dx_, pos.y() + drag_dy_);

  dragWindow_->hide();

  int num_cards = drag_cards_.size();

  if (drag_area_type_ == STACK_TYPE) {
    for (int i = num_cards - 1; i >= 0; --i)
      drag_area_.stack->push(drag_cards_[i]);
  }
  else
    drag_area_.work_area->push(drag_cards_[0]);

  CSeahavenStack    *stack     = getStack   (pos.x(), pos.y());
  CSeahavenPile     *pile      = getPile    (pos.x(), pos.y());
  CSeahavenWorkArea *work_area = getWorkArea(pos.x(), pos.y());

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

  update();
}

CSeahavenStack *
CQSeahavenCanvas::
getStack(int x, int)
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
CQSeahavenCanvas::
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
CQSeahavenCanvas::
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

void
CQSeahavenCanvas::
drawDragCards(QPainter *painter)
{
  int y = 0;

  int num_cards = drag_cards_.size();

  for (int i = num_cards - 1; i >= 0; --i) {
    int ind = drag_cards_[i]->getIndex();

    painter->drawImage(QPoint(0, y), CQImageUtil::toQImage(card_images_[ind]));

    y += card_dy_;
  }
}

//--------------------

CQDragWindow::
CQDragWindow(CQSeahavenCanvas *canvas) :
 QWidget(canvas), canvas_(canvas)
{
}

CQDragWindow::
~CQDragWindow()
{
}

void
CQDragWindow::
paintEvent(QPaintEvent *)
{
  QPainter painter(this);

  painter.fillRect(rect(), QBrush(QColor(255,255,255)));

  canvas_->drawDragCards(&painter);
}
