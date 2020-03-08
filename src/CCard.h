#ifndef CCard_H
#define CCard_H

#include <CInstCount.h>
#include <CImage.h>
#include <iostream>

class CCardDeck;

class CCard {
  CINST_COUNT_MEMBER(CCard);

 public:
  enum class Suit {
    None    =-1,
    Clubs   = 0,
    Diamonds= 1,
    Hearts  = 2,
    Spades  = 3
  };

  enum class Side {
    None    =-1,
    FaceUp  = 0,
    FaceDown= 1
  };

  enum class Value {
    None =-1,
    Ace  = 0,
    Two  = 1,
    Three= 2,
    Four = 3,
    Five = 4,
    Six  = 5,
    Seven= 6,
    Eight= 7,
    Nine = 8,
    Ten  = 9,
    Jack = 10,
    Queen= 11,
    King = 12
  };

 public:
  CCard(CCardDeck *deck, Suit suit, Value value, Side side=Side::FaceDown);

  virtual ~CCard();

  CCardDeck* getDeck () const { return deck_ ; }
  Suit       getSuit () const { return suit_ ; }
  Value      getValue() const { return value_; }

  Side getSide() const { return side_; }
  void setSide(Side side);

  bool getSelected() const { return selected_; }
  void setSelected(bool selected);

  virtual CImagePtr getImage() const;

  int getIndex() const { return index_; }

  bool isSameSuit(CCard *card);

  bool isNextValue(CCard *card);
  bool isSameValue(CCard *card);
  bool isPrevValue(CCard *card);

  bool isLowerValue (CCard *card);
  bool isHigherValue(CCard *card);

  bool isNextCard(CCard *card);
  bool isSameCard(CCard *card);
  bool isPrevCard(CCard *card);

  bool isAce  () const { return value_ == Value::Ace  ; }
  bool isTwo  () const { return value_ == Value::Two  ; }
  bool isThree() const { return value_ == Value::Three; }
  bool isFour () const { return value_ == Value::Four ; }
  bool isFive () const { return value_ == Value::Five ; }
  bool isSix  () const { return value_ == Value::Six  ; }
  bool isSeven() const { return value_ == Value::Seven; }
  bool isEight() const { return value_ == Value::Eight; }
  bool isNine () const { return value_ == Value::Nine ; }
  bool isTen  () const { return value_ == Value::Ten  ; }
  bool isJack () const { return value_ == Value::Jack ; }
  bool isQueen() const { return value_ == Value::Queen; }
  bool isKing () const { return value_ == Value::King ; }

  bool isClub   () const { return suit_ == Suit::Clubs   ; }
  bool isDiamond() const { return suit_ == Suit::Diamonds; }
  bool isHeart  () const { return suit_ == Suit::Hearts  ; }
  bool isSpade  () const { return suit_ == Suit::Spades  ; }

  bool isFaceUp  () const { return side_ == Side::FaceUp;   }
  bool isFaceDown() const { return side_ == Side::FaceDown; }

  int getWidth () const;
  int getHeight() const;

  char getSuitChar() const;

  std::string getName() const;

  void print(std::ostream &os) const;

  friend std::ostream &operator<<(std::ostream &os, const CCard &card);

  static char suitToChar(Suit suit);
  static Suit charToSuit(char c);

 protected:
  CCardDeck* deck_     { nullptr };
  Suit       suit_     { Suit::None };
  Value      value_    { Value::None };
  int        index_    { -1 };
  Side       side_     { Side::None };
  CImagePtr  image_;
  bool       selected_ { false };
};

#endif
