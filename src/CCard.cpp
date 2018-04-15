#include <CCard.h>

#include <CCardDeck.h>
#include <CCardFaceData.h>
#include <CImageLib.h>
#include <CStrUtil.h>

CCard::
CCard(CCardDeck *deck, Suit suit, Value value, Side side) :
 deck_(deck), suit_(suit), value_(value), side_(side)
{
  index_ = int(suit_)*13 + int(value_);
}

CCard::
~CCard()
{
}

void
CCard::
setSide(CCard::Side side)
{
  side_ = side;
}

void
CCard::
setSelected(bool selected)
{
  selected_ = selected;
}

CImagePtr
CCard::
getImage() const
{
  if (! image_.isValid()) {
    CCard *th = const_cast<CCard *>(this);

    CImageNameSrc src("CCard/" + CStrUtil::toString(index_));

    th->image_ = CImageMgrInst->createImage(src);

    th->image_->read(card_face_data[index_].data, card_face_data[index_].len);
  }

  return image_;
}

bool
CCard::
isSameSuit(CCard *card)
{
  return (card->suit_ == suit_);
}

bool
CCard::
isNextValue(CCard *card)
{
  return ((int(card->value_) - int(value_)) == 1);
}

bool
CCard::
isSameValue(CCard *card)
{
  return (card->value_ == value_);
}

bool
CCard::
isPrevValue(CCard *card)
{
  return ((int(value_) - int(card->value_)) == 1);
}

bool
CCard::
isLowerValue(CCard *card)
{
  return (value_ < card->value_);
}

bool
CCard::
isHigherValue(CCard *card)
{
  return (value_ > card->value_);
}

bool
CCard::
isNextCard(CCard *card)
{
  return ((card->suit_ == suit_) && isNextValue(card));
}

bool
CCard::
isSameCard(CCard *card)
{
  return ((card->suit_ == suit_) && isSameValue(card));
}

bool
CCard::
isPrevCard(CCard *card)
{
  return ((card->suit_ == suit_) && isPrevValue(card));
}

int
CCard::
getWidth() const
{
  return deck_->getCardWidth ();
}

int
CCard::
getHeight() const
{
  return deck_->getCardHeight();
}

char
CCard::
getSuitChar() const
{
  return suitToChar(suit_);
}

std::string
CCard::
getName() const
{
  std::string str;

  CStrUtil::strprintf(&str, "%c%02d", getSuitChar(), int(value_) + 1);

  return str;
}

void
CCard::
print(std::ostream &os) const
{
  os << getName();
}

char
CCard::
suitToChar(Suit suit)
{
  switch (suit) {
    case Suit::Clubs   : return 'C';
    case Suit::Diamonds: return 'D';
    case Suit::Hearts  : return 'H';
    case Suit::Spades  : return 'S';
    default            : return '?';
  }
}

CCard::Suit
CCard::
charToSuit(char c)
{
  switch (c) {
    case 'C': return Suit::Clubs;
    case 'D': return Suit::Diamonds;
    case 'H': return Suit::Hearts;
    case 'S': return Suit::Spades;
    default : throw "Bad Suit Char";
  }
}

std::ostream &
operator<<(std::ostream &os, const CCard &card)
{
  card.print(os);

  return os;
}
