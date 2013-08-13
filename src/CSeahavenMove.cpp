#include <CSeahavenMove.h>

#include <CSeahaven.h>
#include <CSeahavenStack.h>
#include <CSeahavenWorkArea.h>
#include <CSeahavenPile.h>
#include <CInstCount.h>

#include <algorithm>

int
CSeahavenMoveSetCmp::
operator()(CSeahavenMoveSet *a, CSeahavenMoveSet *b)
{
  return (a->getType() < b->getType());
}

CSeahavenMoveSetSet::
CSeahavenMoveSetSet() : num_move_sets_(0)
{
}

CSeahavenMoveSetSet::
~CSeahavenMoveSetSet()
{
  delete_all();
}

void
CSeahavenMoveSetSet::
delete_all()
{
  MoveSetList::iterator pmove_set1 = move_sets_.begin();
  MoveSetList::iterator pmove_set2 = move_sets_.end  ();

  for ( ; pmove_set1 != pmove_set2; ++pmove_set1)
    delete *pmove_set1;
}

CSeahavenMoveSet *
CSeahavenMoveSetSet::
getMoveSet(int num)
{
  assert(num >= 0 && num < num_move_sets_);

  return move_sets_[num];
}

CSeahavenMoveSet *
CSeahavenMoveSetSet::
addMoveSet()
{
  CSeahavenMoveSet *move_set = new CSeahavenMoveSet();

  move_sets_.push_back(move_set);

  ++num_move_sets_;

  return move_set;
}

CSeahavenMoveSet *
CSeahavenMoveSetSet::
addMoveSet(CSeahavenMoveSet &move_set)
{
  CSeahavenMoveSet *move_set1 = new CSeahavenMoveSet(move_set);

  move_sets_.push_back(move_set1);

  ++num_move_sets_;

  return move_set1;
}

void
CSeahavenMoveSetSet::
addMoveSetSet(CSeahavenMoveSetSet &move_set_set)
{
  MoveSetList::iterator pmove_set1 = move_set_set.move_sets_.begin();
  MoveSetList::iterator pmove_set2 = move_set_set.move_sets_.end  ();

  for ( ; pmove_set1 != pmove_set2; ++pmove_set1)
    addMoveSet(**pmove_set1);
}

void
CSeahavenMoveSetSet::
execute()
{
  MoveSetList::iterator pmove_set1 = move_sets_.begin();
  MoveSetList::iterator pmove_set2 = move_sets_.end  ();

  for ( ; pmove_set1 != pmove_set2; ++pmove_set1)
    (*pmove_set1)->execute();
}

void
CSeahavenMoveSetSet::
undo()
{
  MoveSetList::reverse_iterator pmove_set1 = move_sets_.rbegin();
  MoveSetList::reverse_iterator pmove_set2 = move_sets_.rend  ();

  for ( ; pmove_set1 != pmove_set2; ++pmove_set1)
    (*pmove_set1)->undo();
}

void
CSeahavenMoveSetSet::
reverse()
{
  std::reverse(move_sets_.begin(), move_sets_.end());
}

void
CSeahavenMoveSetSet::
sort()
{
  std::sort(move_sets_.begin(), move_sets_.end(), CSeahavenMoveSetCmp());
}

void
CSeahavenMoveSetSet::
print(std::ostream &os) const
{
  MoveSetList::const_iterator pmove_set1 = move_sets_.begin();
  MoveSetList::const_iterator pmove_set2 = move_sets_.end  ();

  for ( ; pmove_set1 != pmove_set2; ++pmove_set1)
    os << **pmove_set1 << std::endl;
}

std::ostream &
operator<<(std::ostream &os, const CSeahavenMoveSetSet &move_set_set)
{
  move_set_set.print(os);

  return os;
}

CSeahavenMoveSet::
CSeahavenMoveSet() : num_moves_(0)
{
}

CSeahavenMoveSet::
CSeahavenMoveSet(const CSeahavenMoveSet &move_set) : num_moves_(0)
{
  MoveList::const_iterator pmove1 = move_set.moves_.begin();
  MoveList::const_iterator pmove2 = move_set.moves_.end  ();

  for ( ; pmove1 != pmove2; ++pmove1) {
    moves_.push_back(new CSeahavenMove(**pmove1));

    ++num_moves_;
  }
}

CSeahavenMoveSet::
~CSeahavenMoveSet()
{
  delete_all();
}

void
CSeahavenMoveSet::
clear()
{
  delete_all();

  moves_.clear();

  num_moves_ = 0;
}

void
CSeahavenMoveSet::
delete_all()
{
  MoveList::iterator pmove1 = moves_.begin();
  MoveList::iterator pmove2 = moves_.end  ();

  for ( ; pmove1 != pmove2; ++pmove1)
    delete *pmove1;
}

CSeahavenMoveType
CSeahavenMoveSet::
getType() const
{
  if (num_moves_ > 0)
    return moves_[0]->getType();
  else
    return STACK_TO_STACK;
}

void
CSeahavenMoveSet::
addMove(CSeahavenStack *stack1, CSeahavenStack *stack2)
{
  CSeahavenMove *move = new CSeahavenMove(stack1, stack2);

  moves_.push_back(move);

  ++num_moves_;
}

void
CSeahavenMoveSet::
addMove(CSeahavenStack *stack, CSeahavenWorkArea *work_area)
{
  CSeahavenMove *move = new CSeahavenMove(stack, work_area);

  moves_.push_back(move);

  ++num_moves_;
}

void
CSeahavenMoveSet::
addMove(CSeahavenStack *stack, CSeahavenPile *pile)
{
  CSeahavenMove *move = new CSeahavenMove(stack, pile);

  moves_.push_back(move);

  ++num_moves_;
}

void
CSeahavenMoveSet::
addMove(CSeahavenWorkArea *work_area, CSeahavenStack *stack)
{
  CSeahavenMove *move = new CSeahavenMove(work_area, stack);

  moves_.push_back(move);

  ++num_moves_;
}

void
CSeahavenMoveSet::
addMove(CSeahavenWorkArea *work_area, CSeahavenPile *pile)
{
  CSeahavenMove *move = new CSeahavenMove(work_area, pile);

  moves_.push_back(move);

  ++num_moves_;
}

void
CSeahavenMoveSet::
addMoveSet(CSeahavenMoveSet &move_set)
{
  MoveList::iterator pmove1 = move_set.moves_.begin();
  MoveList::iterator pmove2 = move_set.moves_.end  ();

  for ( ; pmove1 != pmove2; ++pmove1) {
    moves_.push_back(*pmove1);

    ++num_moves_;
  }

  move_set.moves_.clear();

  move_set.num_moves_ = 0;
}

bool
CSeahavenMoveSet::
isToPileMove() const
{
  if (num_moves_ > 0)
    return moves_[0]->isToPileMove();
  else
    return false;
}

void
CSeahavenMoveSet::
execute()
{
  MoveList::const_iterator pmove1 = moves_.begin();
  MoveList::const_iterator pmove2 = moves_.end  ();

  for ( ; pmove1 != pmove2; ++pmove1)
    (*pmove1)->execute();
}

void
CSeahavenMoveSet::
undo()
{
  MoveList::const_reverse_iterator pmove1 = moves_.rbegin();
  MoveList::const_reverse_iterator pmove2 = moves_.rend  ();

  for ( ; pmove1 != pmove2; ++pmove1)
    (*pmove1)->undo();
}

void
CSeahavenMoveSet::
print(std::ostream &os) const
{
  MoveList::const_iterator pmove1 = moves_.begin();
  MoveList::const_iterator pmove2 = moves_.end  ();

  for ( ; pmove1 != pmove2; ++pmove1)
    os << **pmove1 << " ";
}

std::ostream &
operator<<(std::ostream &os, const CSeahavenMoveSet &move_set)
{
  move_set.print(os);

  return os;
}

CSeahavenMove::
CSeahavenMove(const CSeahavenMove &move) :
 type_(move.type_), stack1_(move.stack1_), stack2_(move.stack2_),
 work_area_(move.work_area_), pile_(move.pile_)
{
}

CSeahavenMove::
CSeahavenMove(CSeahavenStack *stack1, CSeahavenStack *stack2) :
 type_(STACK_TO_STACK), stack1_(stack1), stack2_(stack2),
 work_area_(NULL), pile_(NULL)
{
}

CSeahavenMove::
CSeahavenMove(CSeahavenStack *stack, CSeahavenWorkArea *work_area) :
 type_(STACK_TO_WORK_AREA), stack1_(stack), stack2_(NULL),
 work_area_(work_area), pile_(NULL)
{
}

CSeahavenMove::
CSeahavenMove(CSeahavenStack *stack, CSeahavenPile *pile) :
 type_(STACK_TO_PILE), stack1_(stack), stack2_(NULL),
 work_area_(NULL), pile_(pile)
{
}

CSeahavenMove::
CSeahavenMove(CSeahavenWorkArea *work_area, CSeahavenStack *stack) :
 type_(WORK_AREA_TO_STACK), stack1_(stack), stack2_(NULL),
 work_area_(work_area), pile_(NULL)
{
}

CSeahavenMove::
CSeahavenMove(CSeahavenWorkArea *work_area, CSeahavenPile *pile) :
 type_(WORK_AREA_TO_PILE), stack1_(NULL), stack2_(NULL),
 work_area_(work_area), pile_(pile)
{
}

bool
CSeahavenMove::
isToPileMove() const
{
  return (type_ == STACK_TO_PILE || type_ == WORK_AREA_TO_PILE);
}

void
CSeahavenMove::
execute()
{
  if      (type_ == STACK_TO_STACK) {
    stack2_->push(stack1_->pop());
  }
  else if (type_ == STACK_TO_WORK_AREA) {
    work_area_->push(stack1_->pop());
  }
  else if (type_ == STACK_TO_PILE) {
    pile_->push(stack1_->pop());
  }
  else if (type_ == WORK_AREA_TO_STACK) {
    stack1_->push(work_area_->pop());
  }
  else if (type_ == WORK_AREA_TO_PILE) {
    pile_->push(work_area_->pop());
  }
}

void
CSeahavenMove::
undo()
{
  if      (type_ == STACK_TO_STACK) {
    stack1_->push(stack2_->pop());
  }
  else if (type_ == STACK_TO_WORK_AREA) {
    stack1_->push(work_area_->pop());
  }
  else if (type_ == STACK_TO_PILE) {
    stack1_->push(pile_->pop());
  }
  else if (type_ == WORK_AREA_TO_STACK) {
    work_area_->push(stack1_->pop());
  }
  else if (type_ == WORK_AREA_TO_PILE) {
    work_area_->push(pile_->pop());
  }
}

void
CSeahavenMove::
print(std::ostream &os) const
{
  if      (type_ == STACK_TO_STACK) {
    os << "Stack " << (stack1_->getNum() + 1) << " to ";
    os << "Stack " << (stack2_->getNum() + 1);
  }
  else if (type_ == STACK_TO_WORK_AREA) {
    os << "Stack "     << (stack1_   ->getNum() + 1) << " to ";
    os << "Work Area " << (work_area_->getNum() + 1);
  }
  else if (type_ == STACK_TO_PILE) {
    os << "Stack " << (stack1_->getNum() + 1) << " to ";
    os << "Pile";
  }
  else if (type_ == WORK_AREA_TO_STACK) {
    os << "Work Area " << (work_area_->getNum() + 1) << " to ";
    os << "Stack "     << (stack1_   ->getNum() + 1);
  }
  else if (type_ == WORK_AREA_TO_PILE) {
    os << "Work Area " << (work_area_->getNum() + 1) << " to ";
    os << "Pile";
  }
}

std::ostream &
operator<<(std::ostream &os, const CSeahavenMove &move)
{
  move.print(os);

  return os;
}
