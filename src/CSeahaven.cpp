#include <CSeahaven.h>

#include <CSeahavenStack.h>
#include <CSeahavenPile.h>
#include <CSeahavenWorkArea.h>
#include <CSeahavenMove.h>
#include <CInstCount.h>
#include <CCardDeck.h>
#include <CStrUtil.h>

#include <cstring>

enum { MAX_ITERATIONS = 100000, DIE_ITERATIONS = 101000 };

bool CSeahaven::debug_ = false;

CSeahaven::
CSeahaven()
{
  deck_ = new CCardDeck();

  stack_mgr_  = new CSeahavenStackMgr();
  num_stacks_ = stack_mgr_->getNumStacks();

  pile_mgr_  = new CSeahavenPileMgr();
  num_piles_ = pile_mgr_->getNumPiles();

  work_area_mgr_  = new CSeahavenWorkAreaMgr();
  num_work_areas_ = work_area_mgr_->getNumWorkAreas();

  memset(num_states_, 0, sizeof(num_states_));

  deal();
}

CSeahaven::
~CSeahaven()
{
  delete deck_;

  delete stack_mgr_;
  delete pile_mgr_;
  delete work_area_mgr_;

  deleteStates();
}

void
CSeahaven::
deleteStates()
{
  for (int i = 0; i < NUM_STATE_LISTS; ++i) {
    for (int j = 0; j < num_states_[i]; ++j)
      delete states_[i][j];

    states_[i].clear();

    num_states_[i] = 0;
  }
}

void
CSeahaven::
shuffle()
{
  deck_->shuffle();
}

void
CSeahaven::
deal()
{
  deck_->shuffle();

  stack_mgr_    ->clearStacks   ();
  pile_mgr_     ->clearPiles    ();
  work_area_mgr_->clearWorkAreas();

  int num_cards = deck_->getNumCards();

  int num_stack_cards = num_cards/num_stacks_;

  for (int i = 0; i < num_stacks_; ++i) {
    CSeahavenStack *stack = stack_mgr_->getStack(i);

    for (int j = 0; j < num_stack_cards; ++j) {
      CCard *card = deck_->popCard();

      card->setSide(CCard::FaceUp);

      stack->push(card);
    }
  }

  int num_extra_cards = num_cards - num_stack_cards*num_stacks_;

  for (int i = 0; i < num_extra_cards; ++i) {
    CSeahavenWorkArea *work_area = work_area_mgr_->getFreeWorkArea();

    CCard *card = deck_->popCard();

    card->setSide(CCard::FaceUp);

    work_area->push(card);
  }

  if (getDebug())
    std::cout << *this << std::endl;
}

bool
CSeahaven::
readDeck(const std::string &filename)
{
  CFile file(filename);

  deck_->restore(file);

  deal();

  return true;
}

bool
CSeahaven::
solve(CSeahavenMoveSetSet &move_set_set)
{
  if (getDebug()) {
    std::cout << "Solve" << std::endl;

    std::cout << *this << std::endl;
  }

  deleteStates();

  iterations_ = 0;

  int depth = 0;

  if (solve1(move_set_set, depth + 1)) {
    move_set_set.reverse();

    if (getDebug()) {
      std::cout << "Solved in " << iterations_ << " Iterations" << std::endl;

      std::cout << move_set_set << std::endl;
    }

    return true;
  }

  return false;
}

bool
CSeahaven::
solve1(CSeahavenMoveSetSet &move_set_set, int depth)
{
  if (iterations_ > 0 && (iterations_ % 1000) == 0)
    std::cerr << iterations_ << " Iterations " << std::endl;

/*
  if (iterations_ > MAX_ITERATIONS)
    setDebug(true);

  if (iterations_ > DIE_ITERATIONS) {
    return false;
*/

  //-----

  CSeahavenMoveSet move_set;
  bool    unsolvable;
  CSeahavenMoveSet undo_move_set;

  while (getForcedStackMoveSet(move_set, unsolvable)) {
    if (getDebug())
      std::cout << "Do Forced Move: " << move_set << std::endl;

    move_set.execute();

    if (getDebug())
      std::cout << *this << std::endl;

    undo_move_set.addMoveSet(move_set);

    while (getForcedPileMoveSet(move_set)) {
      if (getDebug())
        std::cout << "Do Forced Move: " << move_set << std::endl;

      move_set.execute();

      if (getDebug())
        std::cout << *this << std::endl;

      undo_move_set.addMoveSet(move_set);
    }
  }

  if (unsolvable) {
    if (! undo_move_set.isEmpty()) {
      if (getDebug())
        std::cout << "Undo Forced Moves: " << undo_move_set << std::endl;

      undo_move_set.undo();

      if (getDebug())
        std::cout << *this << std::endl;
    }

    return false;
  }

  while (getForcedPileMoveSet(move_set)) {
    if (getDebug())
      std::cout << "Do Forced Move: " << move_set << std::endl;

    move_set.execute();

    if (getDebug())
      std::cout << *this << std::endl;

    undo_move_set.addMoveSet(move_set);

    while (getForcedStackMoveSet(move_set, unsolvable)) {
      if (getDebug())
        std::cout << "Do Forced Move: " << move_set << std::endl;

      move_set.execute();

      if (getDebug())
        std::cout << *this << std::endl;

      undo_move_set.addMoveSet(move_set);
    }

    if (unsolvable) {
      if (! undo_move_set.isEmpty()) {
        if (getDebug())
          std::cout << "Undo Forced Moves: " << undo_move_set << std::endl;

        undo_move_set.undo();

        if (getDebug())
          std::cout << *this << std::endl;
      }

      return false;
    }
  }

  //-----

  bool solved = isSolved();

  if (solved) {
    if (getDebug()) {
      std::cout << "Solved" << std::endl;

      std::cout << *this << std::endl;
    }

    move_set_set.addMoveSet(undo_move_set);

    if (! undo_move_set.isEmpty()) {
      if (getDebug())
        std::cout << "Undo Forced Moves: " << undo_move_set << std::endl;

      undo_move_set.undo();

      if (getDebug())
        std::cout << *this << std::endl;
    }

    return true;
  }

  //-----

  solved = processMoves(move_set_set, depth);

  if (solved)
    move_set_set.addMoveSet(undo_move_set);

  //-----

  if (! undo_move_set.isEmpty()) {
    if (getDebug())
      std::cout << "Undo Forced Moves: " << undo_move_set << std::endl;

    undo_move_set.undo();

    if (getDebug())
      std::cout << *this << std::endl;
  }

  //-----

  return solved;
}

bool
CSeahaven::
processMoves(CSeahavenMoveSetSet &move_set_set, int depth)
{
  CSeahavenMoveSetSet move_set_set1;

  getMoves(move_set_set1);

  if (getDebug()) {
    if (! move_set_set1.isEmpty()) {
      std::cout << "Moves" << std::endl;

      std::cout << move_set_set1 << std::endl;
    }
    else
      std::cout << "No Moves" << std::endl;
  }

  int move_set_set1_size = move_set_set1.size();

  for (int i = 0; i < move_set_set1_size; ++i) {
    CSeahavenMoveSet *move_set1 = move_set_set1.getMoveSet(i);

    if (getDebug())
      std::cout << "Do Move: " << *move_set1 << std::endl;

    move_set1->execute();

    if (getDebug())
      std::cout << *this << std::endl;

    ++iterations_;

    if (! checkState()) {
      bool solved = solve1(move_set_set, depth + 1);

      if (solved) {
        move_set_set.addMoveSet(*move_set1);

        if (getDebug())
          std::cout << "Undo Move: " << *move_set1 << std::endl;

        move_set1->undo();

        if (getDebug())
          std::cout << *this << std::endl;

        return true;
      }
    }

    if (getDebug())
      std::cout << "Undo Move: " << *move_set1 << std::endl;

    move_set1->undo();

    if (getDebug())
      std::cout << *this << std::endl;
  }

  return false;
}

bool
CSeahaven::
isSolved() const
{
  return pile_mgr_->isSolved();
}

bool
CSeahaven::
getForcedStackMoveSet(CSeahavenMoveSet &move_set, bool &unsolvable)
{
  unsolvable = false;

  int num_empty_work_areas = work_area_mgr_->getNumEmpty();

  for (int i = 0; i < num_stacks_; ++i) {
    CSeahavenStack *stack1 = stack_mgr_->getStack(i);

    if (! stack1->isForceMoveStack())
      continue;

    for (int j = i + 1; j < num_stacks_; ++j) {
      CSeahavenStack *stack2 = stack_mgr_->getStack(j);

      if (! stack2->isForceMoveStack())
        continue;

      if (stacksLockEachOther(stack1, stack2)) {
        if (getDebug()) {
          std::cout << "Stack Lock" << std::endl;

          std::cout << *this << std::endl;
        }

        unsolvable = true;

        return false;
      }
    }
  }

  for (int i = 0; i < num_stacks_; ++i) {
    CSeahavenStack *stack1 = stack_mgr_->getStack(i);

    if (stack1->isEmpty())
      continue;

    CCard *stack_card1 = stack1->peek(-1);

    if (stack1->isForceMoveStack()) {
      for (int j = 0; j < num_stacks_; ++j) {
        if (i == j)
          continue;

        CSeahavenStack *stack2 = stack_mgr_->getStack(j);

        if (stack2->isEmpty())
          continue;

        int num_consecutive2 = stack2->getNumConsecutive();

        CCard *stack2_top_card = stack2->peek(-num_consecutive2);

        if (stack2_top_card->isNextCard(stack_card1)) {
          if (num_consecutive2 - 1 > num_empty_work_areas) {
            unsolvable = true;

            return false;
          }

          addStackToStack(&move_set, stack2, stack1, num_consecutive2);

          return true;
        }
      }
    }

    for (int j = 0; j < num_work_areas_; ++j) {
      std::vector<CSeahavenWorkArea *> work_areas;

      work_area_mgr_->getTopCards(j, work_areas);

      int num_cards = work_areas.size();

      if (num_cards == 0)
        continue;

      CCard *work_area_card = work_areas[0]->peek();

      if (work_area_card->isNextCard(stack_card1)) {
        addWorkToStack(&move_set, work_areas, stack1, num_cards);

        return true;
      }
    }
  }

  return false;
}

bool
CSeahaven::
getForcedPileMoveSet(CSeahavenMoveSet &move_set)
{
  for (int i = 0; i < num_stacks_; ++i) {
    CSeahavenStack *stack1 = stack_mgr_->getStack(i);

    if (stack1->isEmpty())
      continue;

    int num_consecutive1 = stack1->getNumConsecutive();

    //------

    CCard *stack_card1 = stack1->peek(-1);

    CSeahavenPile *pile = pile_mgr_->getPile(stack_card1->getSuit());

    if (pile->isEmpty()) {
      if (stack_card1->isAce()) {
        addStackToPile(&move_set, stack1, pile, num_consecutive1);

        return true;
      }
    }
    else {
      CCard *pile_card = pile->peek();

      if (pile_card->isNextCard(stack_card1)) {
        addStackToPile(&move_set, stack1, pile, num_consecutive1);

        return true;
      }
    }
  }

  //------

  for (int i = 0; i < num_work_areas_; ++i) {
    CCard *work_area_card = work_area_mgr_->getCard(i);

    if (work_area_card == NULL)
      continue;

    CSeahavenWorkArea *work_area = work_area_mgr_->getWorkArea(i);

    CSeahavenPile *pile = pile_mgr_->getPile(work_area_card->getSuit());

    if (pile->isEmpty()) {
      if (work_area_card->isAce()) {
        addWorkToPile(&move_set, work_area, pile);

        return true;
      }
    }
    else {
      CCard *pile_card = pile->peek();

      if (pile_card->isNextCard(work_area_card)) {
        addWorkToPile(&move_set, work_area, pile);

        return true;
      }
    }
  }

  return false;
}

bool
CSeahaven::
stacksLockEachOther(CSeahavenStack *stack1, CSeahavenStack *stack2)
{
  int num_consecutive1 = stack1->getNumConsecutive();
  int num_consecutive2 = stack2->getNumConsecutive();

  int num_non_consecutive1 = stack1->getNumCards() - num_consecutive1;
  int num_non_consecutive2 = stack2->getNumCards() - num_consecutive2;

  CCard *card2 = stack2->peek(-1);

  for (int i = 0; i < num_non_consecutive1; ++i) {
    CCard *card1 = stack1->peek(i);

    if (! card1->isSameSuit(card2))
      continue;

    if (card1->isLowerValue(card2))
      return true;
  }

  CCard *card1 = stack1->peek(-1);

  for (int i = 0; i < num_non_consecutive2; ++i) {
    CCard *card2 = stack2->peek(i);

    if (! card2->isSameSuit(card1))
      continue;

    if (card2->isLowerValue(card1))
      return true;
  }

  return false;
}

void
CSeahaven::
getMoves(CSeahavenMoveSetSet &move_set_set)
{
  getWorkToStackMoves (move_set_set);
  getStackToStackMoves(move_set_set);
  getStackToWorkMoves (move_set_set);
}

void
CSeahaven::
getStackToStackMoves(CSeahavenMoveSetSet &move_set_set)
{
  int num_empty_work_areas = work_area_mgr_->getNumEmpty();

  for (int i = 0; i < num_stacks_; ++i) {
    CSeahavenStack *stack1 = stack_mgr_->getStack(i);

    if (stack1->isEmpty())
      continue;

    int num_consecutive = stack1->getNumConsecutive();

    CCard *stack1_top_card = stack1->peek(-num_consecutive);

    if (num_consecutive - 1 <= num_empty_work_areas) {
      int j = 0;

      for ( ; j < num_stacks_; ++j) {
        if (i == j)
          continue;

        CSeahavenStack *stack2 = stack_mgr_->getStack(j);

        if (stack2->isEmpty()) {
          if (stack1_top_card->isKing()) {
            if (! stack1->isKingStack()) {
              moveStackToStack(move_set_set, stack1, stack2, num_consecutive);
              break; // No need to look any further
            }
          }
        }
        else {
          CCard *stack2_card = stack2->peek();

          if (stack1_top_card->isNextCard(stack2_card)) {
            moveStackToStack(move_set_set, stack1, stack2, num_consecutive);
            break; // No need to look any further
          }
        }
      }

      // If stack to stack move then we don't need the stack to work

      if (j < num_stacks_)
        continue;
    }
  }
}

void
CSeahaven::
getStackToWorkMoves(CSeahavenMoveSetSet &move_set_set)
{
  int num_empty_work_areas = work_area_mgr_->getNumEmpty();

  for (int i = 0; i < num_stacks_; ++i) {
    CSeahavenStack *stack1 = stack_mgr_->getStack(i);

    if (stack1->isEmpty())
      continue;

    int num_consecutive = stack1->getNumConsecutive();

/*
    CCard *stack1_top_card = stack1->peek(-num_consecutive);
*/

    //------

    if (num_consecutive <= num_empty_work_areas) {
      CSeahavenMoveSet move_set;

      addStackToWork(&move_set, stack1, num_consecutive);

/*
      if (stack1_top_card->isKing()) {
        if (getDebug())
          std::cout << "Do Temp Move: " << move_set << std::endl;

        move_set.execute();

        if (getDebug())
          std::cout << *this << std::endl;

        if (isKingMoveStackToStack(stack1) ||
            isKingMoveWorkToStack(stack1))
          move_set_set.addMoveSet(move_set);

        if (getDebug())
          std::cout << "Undo Temp Move: " << move_set << std::endl;

        move_set.undo();

        if (getDebug())
          std::cout << *this << std::endl;
      }
      else
*/
        move_set_set.addMoveSet(move_set);
    }
  }
}

void
CSeahaven::
getWorkToStackMoves(CSeahavenMoveSetSet &move_set_set)
{
  for (int i = 0; i < num_work_areas_; ++i) {
    std::vector<CSeahavenWorkArea *> work_areas;

    work_area_mgr_->getTopCards(i, work_areas);

    int num_cards = work_areas.size();

    if (num_cards == 0)
      continue;

    CCard *work_area_card = work_areas[0]->peek();

    for (int i = 0; i < num_stacks_; ++i) {
      CSeahavenStack *stack = stack_mgr_->getStack(i);

      if (stack->isEmpty()) {
        if (work_area_card->isKing()) {
          moveWorkToStack(move_set_set, work_areas, stack, num_cards);
          break; // No need to look any further
        }
      }
      else {
        CCard *stack_card = stack->peek();

        if (work_area_card->isNextCard(stack_card)) {
          moveWorkToStack(move_set_set, work_areas, stack, num_cards);
          break; // No need to look any further
        }
      }
    }
  }
}

/*
bool
CSeahaven::
isKingMoveStackToStack(CSeahavenStack *stack)
{
  return false;
}

bool
CSeahaven::
isKingMoveWorkToStack(CSeahavenStack *stack)
{
  return false;
}
*/

bool
CSeahaven::
moveStackToStackValid(CSeahavenStack *stack1, CSeahavenStack *stack2) const
{
  if (stack1 == stack2)
    return false;

  if (stack1->isEmpty())
    return false;

  int num_top_cards = stack1->getNumTopCards();

  if (work_area_mgr_->getNumEmpty() < num_top_cards - 1)
    return false;

  CCard *top_card = stack1->getTopCard();

  if (stack2->isEmpty()) {
    if (top_card->isKing())
      return true;
  }
  else {
    CCard *card = stack2->peek();

    if (top_card->isNextCard(card))
      return true;
  }

  return false;
}

bool
CSeahaven::
moveStackToWorkAreaValid(CSeahavenStack *stack) const
{
  if (stack->isEmpty())
    return false;

  int num_top_cards = stack->getNumTopCards();

  if (work_area_mgr_->getNumEmpty() >= num_top_cards)
    return true;

  return false;
}

bool
CSeahaven::
moveStackToPileValid(CSeahavenStack *stack) const
{
  if (stack->isEmpty())
    return false;

  CCard *stack_card = stack->peek();

  CSeahavenPile *pile = pile_mgr_->getPile(stack_card->getSuit());

  if (pile->isEmpty()) {
    if (stack_card->isAce())
      return true;
  }
  else {
    CCard *pile_card = pile->peek();

    if (pile_card->isNextCard(stack_card))
      return true;
  }

  return false;
}

bool
CSeahaven::
moveWorkAreaToStackValid(CSeahavenWorkArea *work_area, CSeahavenStack *stack) const
{
  if (work_area->isEmpty())
    return false;

  CCard *work_area_card = work_area->peek();

  if (stack->isEmpty()) {
    if (work_area_card->isKing())
      return true;
  }
  else {
    CCard *stack_card = stack->peek();

    if (work_area_card->isNextCard(stack_card))
      return true;
  }

  return false;
}

bool
CSeahaven::
moveWorkAreaToPileValid(CSeahavenWorkArea *work_area) const
{
  if (work_area->isEmpty())
    return false;

  CCard *work_area_card = work_area->peek();

  CSeahavenPile *pile = pile_mgr_->getPile(work_area_card->getSuit());

  if (pile->isEmpty()) {
    if (work_area_card->isAce())
      return true;
  }
  else {
    CCard *pile_card = pile->peek();

    if (pile_card->isNextCard(work_area_card))
      return true;
  }

  return false;
}

void
CSeahaven::
moveStackToStack(CSeahavenMoveSetSet &move_set_set, CSeahavenStack *stack1,
                 CSeahavenStack *stack2, int num)
{
  CSeahavenMoveSet *move_set = move_set_set.addMoveSet();

  addStackToStack(move_set, stack1, stack2, num);
}

void
CSeahaven::
moveStackToPile(CSeahavenMoveSetSet &move_set_set, CSeahavenStack *stack,
                CSeahavenPile *pile, int num)
{
  CSeahavenMoveSet *move_set = move_set_set.addMoveSet();

  addStackToPile(move_set, stack, pile, num);
}

void
CSeahaven::
moveStackToWork(CSeahavenMoveSetSet &move_set_set, CSeahavenStack *stack, int num)
{
  CSeahavenMoveSet *move_set = move_set_set.addMoveSet();

  addStackToWork(move_set, stack, num);
}

void
CSeahaven::
moveWorkToStack(CSeahavenMoveSetSet &move_set_set, std::vector<CSeahavenWorkArea *> work_areas,
                CSeahavenStack *stack, int num)
{
  CSeahavenMoveSet *move_set = move_set_set.addMoveSet();

  addWorkToStack(move_set, work_areas, stack, num);
}

void
CSeahaven::
moveWorkToPile(CSeahavenMoveSetSet &move_set_set, std::vector<CSeahavenWorkArea *> work_areas,
               CSeahavenPile *pile, int num)
{
  CSeahavenMoveSet *move_set = move_set_set.addMoveSet();

  for (int i = 0; i < num; ++i)
    addWorkToPile(move_set, work_areas[i], pile);
}

void
CSeahaven::
moveWorkToPile(CSeahavenMoveSetSet &move_set_set, CSeahavenWorkArea *work_area,
               CSeahavenPile *pile)
{
  CSeahavenMoveSet *move_set = move_set_set.addMoveSet();

  addWorkToPile(move_set, work_area, pile);
}

void
CSeahaven::
addStackToStack(CSeahavenMoveSet *move_set, CSeahavenStack *stack1,
                CSeahavenStack *stack2, int num)
{
  std::vector<CSeahavenWorkArea *> work_areas;

  work_area_mgr_->getEmptyWorkAreas(work_areas);

  for (int i = 0; i < num - 1; ++i)
    move_set->addMove(stack1, work_areas[i]);

  move_set->addMove(stack1, stack2);

  for (int i = num - 2; i >= 0; --i)
    move_set->addMove(work_areas[i], stack2);
}

void
CSeahaven::
addStackToPile(CSeahavenMoveSet *move_set, CSeahavenStack *stack,
               CSeahavenPile *pile, int num)
{
  for (int i = 0; i < num; ++i)
    move_set->addMove(stack, pile);
}

void
CSeahaven::
addStackToWork(CSeahavenMoveSet *move_set, CSeahavenStack *stack, int num)
{
  std::vector<CSeahavenWorkArea *> work_areas;

  work_area_mgr_->getEmptyWorkAreas(work_areas);

  for (int i = 0; i < num; ++i)
    move_set->addMove(stack, work_areas[i]);
}

void
CSeahaven::
addWorkToStack(CSeahavenMoveSet *move_set, CSeahavenWorkArea *work_area, CSeahavenStack *stack)
{
  move_set->addMove(work_area, stack);
}

void
CSeahaven::
addWorkToStack(CSeahavenMoveSet *move_set, std::vector<CSeahavenWorkArea *> work_areas,
               CSeahavenStack *stack, int num)
{
  for (int i = 0; i < num; ++i)
    move_set->addMove(work_areas[i], stack);
}

void
CSeahaven::
addWorkToPile(CSeahavenMoveSet *move_set, CSeahavenWorkArea *work_area, CSeahavenPile *pile)
{
  move_set->addMove(work_area, pile);
}

void
CSeahaven::
save(CFile &file)
{
  for (int i = 0; i < num_stacks_; ++i) {
    CSeahavenStack *stack = stack_mgr_->getStack(i);

    int num_cards = stack->getNumCards();

    for (int j = 0; j < num_cards; ++j) {
      CCard *card = stack->peek(j);

      if (j > 0)
        file.printf(" ");

      char suit  = card->getSuitChar();
      int  value = card->getValue   ();

      file.printf("%c%02d", suit, value + 1);
    }

    file.printf("\n");
  }

  for (int i = 0; i < num_piles_; ++i) {
    CSeahavenPile *pile = pile_mgr_->getPile(i);

    int num_cards = pile->getNumCards();

    for (int j = 0; j < num_cards; ++j) {
      CCard *card = pile->peek(j);

      if (j > 0)
        file.printf(" ");

      char suit  = card->getSuitChar();
      int  value = card->getValue   ();

      file.printf("%c%02d", suit, value + 1);
    }

    file.printf("\n");
  }

  for (int i = 0; i < num_work_areas_; ++i) {
    CCard *card = work_area_mgr_->getCard(i);

    if (i > 0)
      file.printf(" ");

    if (card != NULL) {
      char suit  = card->getSuitChar();
      int  value = card->getValue   ();

      file.printf("%c%02d", suit, value + 1);
    }
    else
      file.printf("---");
  }

  file.printf("\n");
}

void
CSeahaven::
restore(CFile &file)
{
  stack_mgr_    ->clearStacks   ();
  pile_mgr_     ->clearPiles    ();
  work_area_mgr_->clearWorkAreas();

  deck_->dealAll();

  std::string line;

  for (int i = 0; i < num_stacks_; ++i) {
    if (! file.readLine(line))
      throw "Bad File";

    CSeahavenStack *stack = stack_mgr_->getStack(i);

    std::vector<std::string> words;

    CStrUtil::addWords(line, words);

    int num_words = words.size();

    for (int j = 0; j < num_words; ++j) {
      if (words[j].size() < 1)
        throw "Bad File";

      char        suit_char = words[j][0];
      std::string value_str = words[j].substr(1);

      CCard::CCardSuit suit = CCard::charToSuit(suit_char);

      if (! CStrUtil::isInteger(value_str))
        throw "Bad File";

      int value = CStrUtil::toInteger(value_str);

      CCard *card = deck_->undealCard(suit, (CCard::CCardValue) (value - 1));

      stack->push(card);
    }
  }

  for (int i = 0; i < num_piles_; ++i) {
    if (! file.readLine(line))
      throw "Bad File";

    CSeahavenPile *pile = pile_mgr_->getPile(i);

    std::vector<std::string> words;

    CStrUtil::addWords(line, words);

    int num_words = words.size();

    for (int j = 0; j < num_words; ++j) {
      if (words[j].size() < 1)
        throw "Bad File";

      char        suit_char = words[j][0];
      std::string value_str = words[j].substr(1);

      CCard::CCardSuit suit = CCard::charToSuit(suit_char);

      if (! CStrUtil::isInteger(value_str))
        throw "Bad File";

      int value = CStrUtil::toInteger(value_str);

      CCard *card = deck_->undealCard(suit, (CCard::CCardValue) (value - 1));

      pile->push(card);
    }
  }

  if (! file.readLine(line))
    throw "Bad File";

  std::vector<std::string> words;

  CStrUtil::addWords(line, words);

  int num_words = words.size();

  if (num_words != num_work_areas_)
    throw "Bad File";

  for (int i = 0; i < num_work_areas_; ++i) {
    if (words[i].size() < 1)
      throw "Bad File";

    if (words[i] != "---") {
      CSeahavenWorkArea *work_area = work_area_mgr_->getWorkArea(i);

      char        suit_char = words[i][0];
      std::string value_str = words[i].substr(1);

      CCard::CCardSuit suit = CCard::charToSuit(suit_char);

      if (! CStrUtil::isInteger(value_str))
        throw "Bad File";

      int value = CStrUtil::toInteger(value_str);

      CCard *card = deck_->undealCard(suit, (CCard::CCardValue) (value - 1));

      work_area->push(card);
    }
  }

  deck_->dealAll();
}

bool
CSeahaven::
checkState()
{
  CSeahavenState *state = new CSeahavenState(stack_mgr_, num_stacks_);

  int key = state->getKey() % NUM_STATE_LISTS;

  for (int i = 0; i < num_states_[key]; ++i) {
    if (*states_[key][i] == *state) {
      delete state;

      return true;
    }
  }

  states_[key].push_back(state);

  ++num_states_[key];

  return false;
}

void
CSeahaven::
print(std::ostream &os) const
{
  os << *stack_mgr_     << std::endl;
  os << *pile_mgr_      << std::endl;
  os << *work_area_mgr_;
}

std::ostream &
operator<<(std::ostream &os, const CSeahaven &seahaven)
{
  seahaven.print(os);

  return os;
}

CSeahavenState::
CSeahavenState(CSeahavenStackMgr *stack_mgr, int num_stacks)
{
  key_ = 0;

  int k = 0;

  memset(c_, 0, sizeof(c_));

  for (int i = 0; i < num_stacks; ++i) {
    CSeahavenStack *stack = stack_mgr->getStack(i);

    int num_cards = stack->getNumCards();

    int k1 = k;

    for (int j = 0; j < num_cards; ++j) {
      CCard *card = stack->peek(j);

      int ind = card->getIndex();

      c_[k1++] = ind;

      key_ ^= ind;
    }

    k += 22;
  }
}

int
operator==(const CSeahavenState &lhs, const CSeahavenState &rhs)
{
  return (memcmp(lhs.c_, rhs.c_, sizeof(lhs.c_)) == 0);
}