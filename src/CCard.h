#ifndef CCARD_H
#define CCARD_H

#include <CInstCount.h>
#include <CImage.h>
#include <iostream>

class CCardDeck;

class CCard {
  CINST_COUNT_MEMBER(CCard);

 public:
  enum CCardSuit  { Clubs, Diamonds, Hearts, Spades };
  enum CCardValue { Ace, Two, Three, Four, Five, Six,
                    Seven, Eight, Nine, Ten, Jack, Queen, King };
  enum CCardSide  { FaceUp, FaceDown };

 public:
  CCard(CCardDeck *deck, CCardSuit suit, CCardValue value,
        CCardSide side=FaceDown);

  virtual ~CCard();

  void setSide(CCardSide side);
  void setSelected(bool selected);

  CCardDeck  *getDeck    () const { return deck_    ; }
  CCardSuit   getSuit    () const { return suit_    ; }
  CCardValue  getValue   () const { return value_   ; }
  CCardSide   getSide    () const { return side_    ; }
  CImagePtr   getImage   () const { return image_   ; }
  bool        getSelected() const { return selected_; }

  int getIndex() const { return index_; }

  bool isSameSuit(CCard *card);

  bool isNextValue(CCard *card);
  bool isSameValue(CCard *card);
  bool isPrevValue(CCard *card);
  bool isLowerValue(CCard *card);
  bool isHigherValue(CCard *card);

  bool isNextCard(CCard *card);
  bool isSameCard(CCard *card);
  bool isPrevCard(CCard *card);

  bool isAce  () const { return value_ == Ace  ; }
  bool isTwo  () const { return value_ == Two  ; }
  bool isThree() const { return value_ == Three; }
  bool isFour () const { return value_ == Four ; }
  bool isFive () const { return value_ == Five ; }
  bool isSix  () const { return value_ == Six  ; }
  bool isSeven() const { return value_ == Seven; }
  bool isEight() const { return value_ == Eight; }
  bool isNine () const { return value_ == Nine ; }
  bool isTen  () const { return value_ == Ten  ; }
  bool isJack () const { return value_ == Jack ; }
  bool isQueen() const { return value_ == Queen; }
  bool isKing () const { return value_ == King ; }

  bool isClub   () const { return suit_ == Clubs   ; }
  bool isDiamond() const { return suit_ == Diamonds; }
  bool isHeart  () const { return suit_ == Hearts  ; }
  bool isSpade  () const { return suit_ == Spades  ; }

  bool isFaceUp  () const { return side_ == FaceUp;   }
  bool isFaceDown() const { return side_ == FaceDown; }

  int getWidth () const;
  int getHeight() const;

  char getSuitChar() const;

  void print(std::ostream &os) const;

  friend std::ostream &operator<<(std::ostream &os, const CCard &card);

  static char      suitToChar(CCardSuit suit);
  static CCardSuit charToSuit(char c);

 private:
  CCardDeck  *deck_;
  CCardSuit   suit_;
  CCardValue  value_;
  int         index_;
  CCardSide   side_;
  CImagePtr   image_;
  bool        selected_;
};

#endif
