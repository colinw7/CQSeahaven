#include <CSeahavenHistory.h>

#include <CSeahaven.h>
#include <CSeahavenMove.h>
#include <CSeahavenStack.h>
#include <CSeahavenWorkArea.h>
#include <CSeahavenPile.h>
#include <CInstCount.h>

CSeahavenHistory::
CSeahavenHistory(CSeahaven *seahaven) : seahaven_(seahaven)
{
}

void
CSeahavenHistory::
moveStackToStack(CSeahavenStack *stack1, CSeahavenStack *stack2)
{
  CSeahavenMove *move = new CSeahavenMove(stack1, stack2);

  moves_.push_back(move);

  tidy(move);
}

void
CSeahavenHistory::
moveStackToWork(CSeahavenStack *stack, CSeahavenWorkArea *work_area)
{
  CSeahavenMove *move = new CSeahavenMove(stack, work_area);

  moves_.push_back(move);

  tidy(move);
}

void
CSeahavenHistory::
moveWorkToStack(CSeahavenWorkArea *work_area, CSeahavenStack *stack)
{
  CSeahavenMove *move = new CSeahavenMove(work_area, stack);

  moves_.push_back(move);

  tidy(move);
}

void
CSeahavenHistory::
tidy(CSeahavenMove *move)
{
  while (doTidy(move))
    ;
}

bool
CSeahavenHistory::
doTidy(CSeahavenMove *move)
{
  std::vector<CSeahavenStack *> stacks = seahaven_->getStacks();

  for (auto &stack : stacks) {
    Card *card = stack->peek();
    if (! card) continue;

    CSeahavenPile *pile = CSeahavenPile::getPile(card->getSuit());
    if (! pile) continue;

    Card *pile_card = pile->peek();

    if (! pile_card && ! card->isAce())
      continue;

    if (pile_card && (card->getValue() != pile_card->getValue() + 1))
      continue;

    CSeahavenMove *sub_move = new CSeahavenMove(stack, pile);

    move->addSubMove(sub_move);

    card = stack->pop();

    pile->push(card);

    return true;
  }

  std::vector<CSeahavenWorkArea *> work_areas = seahaven_->getWorkAreas();

  for (auto &work_area : work_areas) {
    Card *card = work_area->peek();
    if (! card) continue;

    CSeahavenPile *pile = CSeahavenPile::getPile(card->getSuit());
    if (! pile) continue;

    Card *pile_card = pile->peek();

    if (! pile_card && ! card->isAce())
      continue;

    if (pile_card && (card->getValue() != pile_card->getValue() + 1))
      continue;

    CSeahavenMove *sub_move = new CSeahavenMove(work_area, pile);

    move->addSubMove(sub_move);

    card = work_area->pop();

    pile->push(card);

    return true;
  }

  return false;
}
