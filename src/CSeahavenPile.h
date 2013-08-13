#ifndef SEAHAVEN_PILE_H
#define SEAHAVEN_PILE_H

#include <CInstCount.h>

#include <CCard.h>

enum { NUM_PILES = 4 };

class CSeahavenPile;

class CSeahavenPileMgr {
  CINST_COUNT_MEMBER(CSeahavenPileMgr);

 private:
  CSeahavenPile *piles_[NUM_PILES];

 public:
  CSeahavenPileMgr();
 ~CSeahavenPileMgr();

  int getNumPiles() const { return NUM_PILES; }

  CSeahavenPile *getPile(CCard::CCardSuit suit);
  CSeahavenPile *getPile(int num);

  void clearPiles();

  bool isSolved() const;

  void print(std::ostream &os) const;

  friend std::ostream &operator<<(std::ostream &os, const CSeahavenPileMgr &pile_mgr);
};

class CSeahavenPile {
  CINST_COUNT_MEMBER(CSeahavenPile);

 public:
  CSeahavenPile(CCard::CCardSuit suit);
 ~CSeahavenPile();

  CCard::CCardSuit getSuit() const { return suit_; }

  bool isEmpty() const { return (num_cards_ == 0); }

  int getNumCards() const { return num_cards_; }

  bool isSolved() const { return solved_; }

  void clear();

  void   push(CCard *card);
  CCard *pop ();
  CCard *peek();
  CCard *peek(int i);

  void print(std::ostream &os) const;

  friend std::ostream &operator<<(std::ostream &os, const CSeahavenPile &pile);

 private:
  typedef std::vector<CCard *> CardList;

  CCard::CCardSuit suit_;
  CardList         cards_;
  int              num_cards_;
  bool             solved_;
};

#endif
