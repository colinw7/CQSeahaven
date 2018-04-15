#ifndef CCardDeck_H
#define CCardDeck_H

#include <CFile.h>
#include <CCard.h>
#include <CInstCount.h>

class CCard;

class CCardDeck {
  CINST_COUNT_MEMBER(CCardDeck);

 public:
  enum { NUM_CARDS = 52 };

  CCardDeck();

  virtual ~CCardDeck();

  virtual CImagePtr getBackImage() const;

  virtual CCard *createCard(CCard::Suit suit, CCard::Value value);

  void save   (CFile &file);
  void restore(CFile &file);

  void shuffle();

  void undealAll();
  void dealAll();

  CCard *popCard();
  CCard *peekCard() const;
  CCard *peekCard(int pos) const;
  void   pushCard(CCard *card);

  CCard *undealCard(CCard::Suit suit, CCard::Value value);

  virtual int getCardWidth () const;
  virtual int getCardHeight() const;

  int getNumCards() const { return NUM_CARDS; }

 private:
  void init() const;

  void deleteCards();

 private:
  typedef std::vector<CCard *> CCardOnList;
  typedef std::list  <CCard *> CCardOffList;

  bool initialized_ { false };

  CCardOnList  cards_on_;
  CCardOffList cards_off_;

  CImagePtr backImage_;
};

#endif
