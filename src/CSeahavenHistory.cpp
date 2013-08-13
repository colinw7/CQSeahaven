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

  std::vector<CSeahavenStack *>::iterator pstack;

  for (pstack = stacks.begin(); pstack != stacks.end(); ++pstack) {
    Card *card = (*pstack)->peek();

    if (card == NULL)
      continue;

    CSeahavenPile *pile = CSeahavenPile::getPile(card->getSuit());

    if (pile == NULL)
      continue;

    Card *pile_card = pile->peek();

    if (pile_card == NULL && ! card->isAce())
      continue;

    if (pile_card != NULL &&
        (card->getValue() != pile_card->getValue() + 1))
      continue;

    CSeahavenMove *sub_move = new CSeahavenMove(*pstack, pile);

    move->addSubMove(sub_move);

    card = (*pstack)->pop();

    pile->push(card);

    return true;
  }

  std::vector<CSeahavenWorkArea *> work_areas = seahaven_->getWorkAreas();

  std::vector<CSeahavenWorkArea *>::iterator pwork_area;

  for (pwork_area = work_areas.begin(); pwork_area != work_areas.end(); ++pwork_area) {
    Card *card = (*pwork_area)->peek();

    if (card == NULL)
      continue;

    CSeahavenPile *pile = CSeahavenPile::getPile(card->getSuit());

    if (pile == NULL)
      continue;

    Card *pile_card = pile->peek();

    if (pile_card == NULL && ! card->isAce())
      continue;

    if (pile_card != NULL &&
        (card->getValue() != pile_card->getValue() + 1))
      continue;

    CSeahavenMove *sub_move = new CSeahavenMove(*pwork_area, pile);

    move->addSubMove(sub_move);

    card = (*pwork_area)->pop();

    pile->push(card);

    return true;
  }

  return false;
}
