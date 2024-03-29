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

  uint getNumPiles() const { return NUM_PILES; }

  CSeahavenPile *getPile(CCard::Suit suit);
  CSeahavenPile *getPile(int num);

  void clearPiles();

  bool isSolved() const;

  void print(std::ostream &os) const;

  friend std::ostream &operator<<(std::ostream &os, const CSeahavenPileMgr &pile_mgr);
};

class CSeahavenPile {
  CINST_COUNT_MEMBER(CSeahavenPile);

 public:
  CSeahavenPile(CCard::Suit suit);
 ~CSeahavenPile();

  CCard::Suit getSuit() const { return suit_; }

  bool isEmpty() const { return (num_cards_ == 0); }

  uint getNumCards() const { return num_cards_; }

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

  CCard::Suit suit_;
  CardList    cards_;
  uint        num_cards_ { 0 };
  bool        solved_ { false };
};

#endif
