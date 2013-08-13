#ifndef CCARD_DECK_H
#define CCARD_DECK_H

#include <CFile.h>
#include <CCard.h>
#include <CInstCount.h>

class CCard;

class CCardDeck {
  CINST_COUNT_MEMBER(CCardDeck);

 public:
  enum { NUM_CARDS = 52 };

  CCardDeck();
 ~CCardDeck();

  CImagePtr getBackImage() const { return back_image_; }

  void save(CFile &file);
  void restore(CFile &file);

  void   shuffle();
  void   undealAll();
  void   dealAll();
  CCard *popCard();
  CCard *peekCard() const;
  CCard *peekCard(int pos) const;
  void   pushCard(CCard *card);

  CCard *undealCard(CCard::CCardSuit suit, CCard::CCardValue value);

  int getCardWidth () const;
  int getCardHeight() const;

  int getNumCards() const { return NUM_CARDS; }

 private:
  void deleteCards();

 private:
  typedef std::vector<CCard *> CCardOnList;
  typedef std::list  <CCard *> CCardOffList;

  CCardOnList  cards_on_;
  CCardOffList cards_off_;

  CImagePtr back_image_;
};

#endif
