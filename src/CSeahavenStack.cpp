#include <CSeahavenStack.h>

#include <CSeahaven.h>
#include <CCard.h>
#include <CCardDeck.h>
#include <CStrUtil.h>

static const int NUM_STACKS = 10;

CSeahavenStackMgr::
CSeahavenStackMgr()
{
  for (int i = 0; i < NUM_STACKS; ++i)
    stacks_.push_back(new CSeahavenStack(i));
}

CSeahavenStackMgr::
~CSeahavenStackMgr()
{
  for (int i = 0; i < NUM_STACKS; i++)
    delete stacks_[i];
}

CSeahavenStack *
CSeahavenStackMgr::
getStack(int num)
{
  assert(num >= 0 && num < NUM_STACKS);

  return stacks_[num];
}

void
CSeahavenStackMgr::
clearStacks()
{
  for (int i = 0; i < NUM_STACKS; i++)
    stacks_[i]->clear();
}

int
CSeahavenStackMgr::
getNumStacks() const
{
  return NUM_STACKS;
}

void
CSeahavenStackMgr::
print(std::ostream &os) const
{
  for (int i = 0; i < NUM_STACKS; i++) {
    CStrUtil::fprintf(os, "Stack %02d) ", i + 1);

    os << *stacks_[i] << std::endl;
  }
}

std::ostream &
operator<<(std::ostream &os, const CSeahavenStackMgr &stack_mgr)
{
  stack_mgr.print(os);

  return os;
}

CSeahavenStack::
CSeahavenStack(int num) :
 num_(num), num_cards_(0), num_consecutive_(0)
{
}

CSeahavenStack::
~CSeahavenStack()
{
}

void
CSeahavenStack::
getTopCards(std::vector<CCard *> &cards) const
{
  if (num_cards_ == 0)
    return;

  int i = num_cards_ - 1;

  CCard *card1 = cards_[i--];

  cards.push_back(card1);

  while (i >= 0) {
    CCard *card2 = cards_[i--];

    if (! card1->isNextCard(card2))
      break;

    cards.push_back(card2);

    card1 = card2;
  }
}

int
CSeahavenStack::
getNumTopCards() const
{
  int num_top_cards = 0;

  if (num_cards_ == 0)
    return num_top_cards;

  int i = num_cards_ - 1;

  CCard *card1 = cards_[i--];

  ++num_top_cards;

  while (i >= 0) {
    CCard *card2 = cards_[i--];

    if (! card1->isNextCard(card2))
      break;

    ++num_top_cards;

    card1 = card2;
  }

  return num_top_cards;
}

CCard *
CSeahavenStack::
getTopCard() const
{
  if (num_cards_ == 0)
    return nullptr;

  return cards_[num_cards_ - num_consecutive_];
}

void
CSeahavenStack::
clear()
{
  cards_.clear();

  num_cards_        = 0;
  num_consecutive_  = 0;
  king_stack_       = false;
  force_move_stack_ = false;
}

void
CSeahavenStack::
push(CCard *card)
{
  if (num_cards_ == 0) {
    num_consecutive_ = 1;

    if (card->isKing())
      king_stack_ = true;
  }
  else {
    if (card->isNextCard(cards_[num_cards_ - 1])) {
      ++num_consecutive_;

      if (num_consecutive_ > 5)
        force_move_stack_ = true;
    }
    else
      num_consecutive_ = 1;
  }

  cards_.push_back(card);

  ++num_cards_;
}

CCard *
CSeahavenStack::
pop()
{
  assert(num_cards_ > 0);

  CCard *card = cards_[--num_cards_];

  cards_.pop_back();

  if (num_cards_ == 0) {
    num_consecutive_ = 0;

    king_stack_ = false;
  }
  else {
    if (num_consecutive_ > 1) {
      --num_consecutive_;

      if (num_consecutive_ <= 5)
        force_move_stack_ = false;
    }
  }

  return card;
}

CCard *
CSeahavenStack::
peek()
{
  assert(num_cards_ > 0);

  return cards_[num_cards_ - 1];
}

CCard *
CSeahavenStack::
peek(int i)
{
  if (i < 0)
    i += num_cards_;

  assert(i >= 0 && i < num_cards_);

  return cards_[i];
}

void
CSeahavenStack::
print(std::ostream &os) const
{
  for (auto &card : cards_)
    os << *card << " ";
}

std::ostream &
operator<<(std::ostream &os, const CSeahavenStack &stack)
{
  stack.print(os);

  return os;
}
