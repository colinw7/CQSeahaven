#include <CCardDeck.h>
#include <CCard.h>
#include <CCardBackData.h>
#include <CImageLib.h>
#include <CStrUtil.h>
#include <COSRand.h>
#include <cstring>

const int CARD_WIDTH  = 64;
const int CARD_HEIGHT = 96;

const int NUM_SHUFFLES = 100;

static int randCard();

static char suit_chars[] = "CDHS";

CCardDeck::
CCardDeck()
{
}

CCardDeck::
~CCardDeck()
{
  deleteCards();
}

void
CCardDeck::
init() const
{
  if (! initialized_) {
    CCardDeck *th = const_cast<CCardDeck *>(this);

    th->initialized_ = true;

    for (int i = 0; i < NUM_CARDS; i++) {
      CCard::Suit  suit  = (CCard::Suit ) (i / 13);
      CCard::Value value = (CCard::Value) (i % 13);

      CCard *card = th->createCard(suit, value);

      th->cardsOn_.push_back(card);
    }

    th->shuffle();
  }
}

CCard *
CCardDeck::
createCard(CCard::Suit suit, CCard::Value value)
{
  CCard *card = new CCard(this, suit, value);

  assert(! cards_[suit][value]);

  cards_[suit][value] = card;

  return card;
}

CImagePtr
CCardDeck::
getBackImage() const
{
  if (! backImage_.isValid()) {
    CCardDeck *th = const_cast<CCardDeck *>(this);

    CImageNameSrc src("CCardDeck/back");

    th->backImage_ = CImageMgrInst->createImage(src);

    th->backImage_->read(card_back_data.data, card_back_data.len);
  }

  return backImage_;
}

void
CCardDeck::
deleteCards()
{
  for (auto &valueCards : cards_)
    for (auto &valueCard : valueCards.second)
      delete valueCard.second;

  cardsOn_ .clear();
  cardsOff_.clear();
  cards_   .clear();
}

void
CCardDeck::
save(CFile &file)
{
  init();

  for (const auto &card : cardsOn_) {
    CCard::Suit  suit  = card->getSuit ();
    CCard::Value value = card->getValue();

    file.printf("%c%02d\n", suit_chars[int(suit)], int(value) + 1);
  }
}

void
CCardDeck::
restore(CFile &file)
{
  init();

  deleteCards();

  std::string line;

  for (int i = 0; i < NUM_CARDS; i++) {
    if (! file.readLine(line))
      throw "Bad CCardDeck File";

    if (line.size() < 2)
      throw "Bad CCardDeck File";

    char        suit_char = line[0];
    std::string value_str = line.substr(1);

    if (! CStrUtil::isInteger(value_str))
      throw "Bad CCardDeck File";

    int isuit  = (strchr(suit_chars, suit_char) - suit_chars);
    int ivalue = CStrUtil::toInteger(value_str) - 1;

    if (isuit < 0 || isuit > 4)
      throw "Bad CCardDeck File";

    if (ivalue < 0 || ivalue > 13)
      throw "Bad CCardDeck File";

    CCard::Suit  suit  = (CCard::Suit ) isuit;
    CCard::Value value = (CCard::Value) ivalue;

    CCard *card = createCard(suit, value);

    cardsOn_.push_back(card);
  }
}

void
CCardDeck::
shuffle()
{
  init();

  undealAll();

  COSRand::srand();

  for (int i = 0; i < NUM_SHUFFLES; i++) {
    int card1 = randCard();
    int card2 = randCard();

    while (card1 == card2)
      card2 = randCard();

    std::swap(cardsOn_[card1], cardsOn_[card2]);
  }
}

void
CCardDeck::
undealAll()
{
  init();

  for (auto &card : cardsOff_)
    cardsOn_.push_back(card);

  cardsOff_.clear();
}

void
CCardDeck::
dealAll()
{
  init();

  for (std::size_t i = 0; i < cardsOn_.size(); i++)
    popCard();
}

CCard *
CCardDeck::
popCard()
{
  init();

  CCard *card = peekCard();

  cardsOn_.pop_back();

  cardsOff_.push_back(card);

  return card;
}

CCard *
CCardDeck::
peekCard() const
{
  init();

  return cardsOn_[cardsOn_.size() - 1];
}

CCard *
CCardDeck::
peekCard(int pos) const
{
  init();

  if (pos < 0 || pos >= (int) cardsOn_.size())
    return nullptr;

  return cardsOn_[pos];
}

void
CCardDeck::
pushCard(CCard *card)
{
  init();

  cardsOff_.remove(card);

  cardsOn_.push_back(card);
}

CCard *
CCardDeck::
undealCard(CCard::Suit suit, CCard::Value value)
{
  init();

  for (auto &card : cardsOff_) {
    if (suit == card->getSuit() && value == card->getValue()) {
      cardsOn_.push_back(card);

      cardsOff_.remove(card);

      return card;
    }
  }

  return nullptr;
}

int
CCardDeck::
getCardWidth() const
{
  return CARD_WIDTH;
}

int
CCardDeck::
getCardHeight() const
{
  return CARD_HEIGHT;
}

static int
randCard()
{
  long r = COSRand::rand();

  int card = r % CCardDeck::NUM_CARDS;

  return card;
}
