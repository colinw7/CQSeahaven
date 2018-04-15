#include <CSeahavenWorkArea.h>

#include <CSeahavenStack.h>
#include <CSeahaven.h>
#include <CInstCount.h>
#include <CCard.h>
#include <CCardDeck.h>
#include <CStrUtil.h>

CSeahavenWorkAreaMgr::
CSeahavenWorkAreaMgr()
{
  for (int i = 0; i < NUM_WORK_AREAS; ++i) {
    cards_     [i] = nullptr;
    work_areas_[i] = new CSeahavenWorkArea(this, i);
  }

  num_empty_ = NUM_WORK_AREAS;
}

CSeahavenWorkAreaMgr::
~CSeahavenWorkAreaMgr()
{
  for (int i = 0; i < NUM_WORK_AREAS; ++i)
    delete work_areas_[i];
}

CCard *
CSeahavenWorkAreaMgr::
getCard(int num)
{
  assert(num >= 0 && num < NUM_WORK_AREAS);

  return cards_[num];
}

CSeahavenWorkArea *
CSeahavenWorkAreaMgr::
getWorkArea(int num)
{
  assert(num >= 0 && num < NUM_WORK_AREAS);

  return work_areas_[num];
}

CSeahavenWorkArea *
CSeahavenWorkAreaMgr::
getFreeWorkArea()
{
  for (int i = 0; i < NUM_WORK_AREAS; ++i)
    if (! cards_[i])
      return work_areas_[i];

  return nullptr;
}

void
CSeahavenWorkAreaMgr::
getEmptyWorkAreas(std::vector<CSeahavenWorkArea *> &work_areas)
{
  for (int i = 0; i < NUM_WORK_AREAS; ++i)
    if (! cards_[i])
      work_areas.push_back(work_areas_[i]);
}

void
CSeahavenWorkAreaMgr::
clearWorkAreas()
{
  for (int i = 0; i < NUM_WORK_AREAS; ++i)
    cards_[i] = nullptr;

  num_empty_ = NUM_WORK_AREAS;
}

void
CSeahavenWorkAreaMgr::
getTopCards(int num, std::vector<CSeahavenWorkArea *> &work_areas)
{
  assert(num >= 0 && num < NUM_WORK_AREAS);

  CCard *card = cards_[num];

  if (! card)
    return;

  bool done[4] = { false, false, false, false };

  work_areas.push_back(work_areas_[num]);

  done[num] = true;

  CCard *card1 = card;
  CCard *card2 = nullptr;

  int i = 0;

  while (i < NUM_WORK_AREAS) {
    if (done[i])
      goto next;

    if (! cards_[i]) {
      done[i] = true;
      goto next;
    }

    card2 = cards_[i];

    if (! card->isSameSuit(card2)) {
      done[i] = true;
      goto next;
    }

    if (card->isNextValue(card2)) {
      work_areas.clear();
      return;
    }

    if (card2->isNextValue(card1)) {
      work_areas.push_back(work_areas_[i]);

      done[i] = true;

      card1 = card2;

      i = 0;
    }

 next:
    i++;
  }
}

bool
CSeahavenWorkAreaMgr::
isEmpty(int num) const
{
  assert(num >= 0 && num < NUM_WORK_AREAS);

  return ! cards_[num];
}

void
CSeahavenWorkAreaMgr::
push(int num, CCard *card)
{
  assert(num >= 0 && num < NUM_WORK_AREAS && ! cards_[num]);

  cards_[num] = card;

  --num_empty_;
}

CCard *
CSeahavenWorkAreaMgr::
pop(int num)
{
  assert(num >= 0 && num < NUM_WORK_AREAS && cards_[num]);

  CCard *card = cards_[num];

  cards_[num] = nullptr;

  ++num_empty_;

  return card;
}

CCard *
CSeahavenWorkAreaMgr::
peek(int num) const
{
  assert(num >= 0 && num < NUM_WORK_AREAS && cards_[num]);

  return cards_[num];
}

void
CSeahavenWorkAreaMgr::
print(std::ostream &os) const
{
  for (int i = 0; i < NUM_WORK_AREAS; ++i) {
    CStrUtil::fprintf(os, "Work %02d) ", i + 1);

    if (cards_[i])
      os << *cards_[i];
    else
      os << "---";

    os << std::endl;
  }
}

std::ostream &
operator<<(std::ostream &os, const CSeahavenWorkAreaMgr &work_area_mgr)
{
  work_area_mgr.print(os);

  return os;
}

CSeahavenWorkArea::
CSeahavenWorkArea(CSeahavenWorkAreaMgr *mgr, int num) :
 mgr_(mgr), num_(num)
{
}

bool
CSeahavenWorkArea::
isEmpty() const
{
  return mgr_->isEmpty(num_);
}

void
CSeahavenWorkArea::
push(CCard *card)
{
  mgr_->push(num_, card);
}

CCard *
CSeahavenWorkArea::
pop()
{
  return mgr_->pop(num_);
}

CCard *
CSeahavenWorkArea::
peek() const
{
  return mgr_->peek(num_);
}
