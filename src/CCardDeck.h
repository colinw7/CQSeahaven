#ifndef CCardDeck_H
#define CCardDeck_H

#include <CFile.h>
#include <CCard.h>
#include <CInstCount.h>
#include <map>

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

 protected:
  void init() const;

  void deleteCards();

 protected:
  typedef std::map<CCard::Value,CCard *>   ValueCards;
  typedef std::map<CCard::Suit,ValueCards> SuitValueCards;

  typedef std::vector<CCard *> CCardOnList;
  typedef std::list  <CCard *> CCardOffList;

  bool           initialized_ { false };
  SuitValueCards cards_;
  CCardOnList    cardsOn_;
  CCardOffList   cardsOff_;
  CImagePtr      backImage_;
};

#endif
