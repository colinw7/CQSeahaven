#include <CSeahavenPile.h>

#include <CInstCount.h>
#include <CCard.h>
#include <CStrUtil.h>

CSeahavenPileMgr::
CSeahavenPileMgr()
{
  piles_[CCard::Clubs   ] = new CSeahavenPile(CCard::Clubs   );
  piles_[CCard::Diamonds] = new CSeahavenPile(CCard::Diamonds);
  piles_[CCard::Hearts  ] = new CSeahavenPile(CCard::Hearts  );
  piles_[CCard::Spades  ] = new CSeahavenPile(CCard::Spades  );
}

CSeahavenPileMgr::
~CSeahavenPileMgr()
{
  for (int i = 0; i < NUM_PILES; i++)
    delete piles_[i];
}

CSeahavenPile *
CSeahavenPileMgr::
getPile(CCard::CCardSuit suit)
{
  return piles_[suit];
}

CSeahavenPile *
CSeahavenPileMgr::
getPile(int num)
{
  assert(num >= 0 && num < NUM_PILES);

  return piles_[num];
}

void
CSeahavenPileMgr::
clearPiles()
{
  for (int i = 0; i < NUM_PILES; i++)
    piles_[i]->clear();
}

bool
CSeahavenPileMgr::
isSolved() const
{
  for (int i = 0; i < NUM_PILES; i++)
    if (! piles_[i]->isSolved())
      return false;

  return true;
}

void
CSeahavenPileMgr::
print(std::ostream &os) const
{
  for (int i = 0; i < NUM_PILES; i++) {
    CStrUtil::fprintf(os, "Pile %02d) ", i + 1);

    os << *piles_[i] << std::endl;
  }
}

std::ostream &
operator<<(std::ostream &os, const CSeahavenPileMgr &pile_mgr)
{
  pile_mgr.print(os);

  return os;
}

CSeahavenPile::
CSeahavenPile(CCard::CCardSuit suit) :
 suit_(suit), num_cards_(0)
{
}

CSeahavenPile::
~CSeahavenPile()
{
  solved_ = false;
}

void
CSeahavenPile::
clear()
{
  cards_.clear();

  num_cards_ = 0;

  solved_ = false;
}

void
CSeahavenPile::
push(CCard *card)
{
  cards_.push_back(card);

  ++num_cards_;

  if (num_cards_ == 13)
    solved_ = true;
}

CCard *
CSeahavenPile::
pop()
{
  assert(num_cards_ > 0);

  CCard *card = cards_[--num_cards_];

  cards_.pop_back();

  solved_ = false;

  return card;
}

CCard *
CSeahavenPile::
peek()
{
  assert(num_cards_ > 0);

  return cards_[num_cards_ - 1];
}

CCard *
CSeahavenPile::
peek(int i)
{
  assert(i >= 0 && i < num_cards_);

  return cards_[i];
}

void
CSeahavenPile::
print(std::ostream &os) const
{
  CardList::const_iterator pcard1 = cards_.begin();
  CardList::const_iterator pcard2 = cards_.end  ();

  for ( ; pcard1 != pcard2; ++pcard1)
    os << **pcard1 << " ";
}

std::ostream &
operator<<(std::ostream &os, const CSeahavenPile &pile)
{
  pile.print(os);

  return os;
}
