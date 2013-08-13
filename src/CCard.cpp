#include <CCard.h>

#include <CCardDeck.h>
#include <CCardFaceData.h>
#include <CImageLib.h>
#include <CStrUtil.h>

CCard::
CCard(CCardDeck *deck, CCardSuit suit, CCardValue value, CCardSide side) :
 deck_(deck), suit_(suit), value_(value), side_(side), selected_(false)
{
  index_ = suit_*13 + value_;

  CImageNameSrc src("CCard/" + CStrUtil::toString(index_));

  image_ = CImageMgrInst->createImage(src);

  image_->read(card_face_data[index_].data, card_face_data[index_].len, CFILE_TYPE_IMAGE_GIF);
}

CCard::
~CCard()
{
}

void
CCard::
setSide(CCard::CCardSide side)
{
  side_ = side;
}

void
CCard::
setSelected(bool selected)
{
  selected_ = selected;
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
  return ((card->value_ - value_) == 1);
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
  return ((value_ - card->value_) == 1);
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
  return ((card->suit_ == suit_) && (card->value_ - value_) == 1);
}

bool
CCard::
isSameCard(CCard *card)
{
  return ((card->suit_ == suit_) && (card->value_ == value_));
}

bool
CCard::
isPrevCard(CCard *card)
{
  return ((card->suit_ == suit_) && (value_ - card->value_) == 1);
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

void
CCard::
print(std::ostream &os) const
{
  CStrUtil::fprintf(os, "%c%02d", getSuitChar(), value_ + 1);
}

char
CCard::
suitToChar(CCardSuit suit)
{
  switch (suit) {
    case Clubs:
      return 'C';
    case Diamonds:
      return 'D';
    case Hearts:
      return 'H';
    case Spades:
      return 'S';
    default:
      return '?';
  }
}

CCard::CCardSuit
CCard::
charToSuit(char c)
{
  switch (c) {
    case 'C':
      return Clubs;
    case 'D':
      return Diamonds;
    case 'H':
      return Hearts;
    case 'S':
      return Spades;
    default:
      throw "Bad Suit Char";
  }
}

std::ostream &
operator<<(std::ostream &os, const CCard &card)
{
  card.print(os);

  return os;
}
