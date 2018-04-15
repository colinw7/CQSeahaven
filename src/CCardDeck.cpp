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
  init();
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

      th->cards_on_.push_back(card);
    }

    th->shuffle();
  }
}

CCard *
CCardDeck::
createCard(CCard::Suit suit, CCard::Value value)
{
  return new CCard(this, suit, value);
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
  for (auto &card : cards_on_)
    delete card;

  cards_on_.clear();

  for (auto &card : cards_off_)
    delete card;

  cards_off_.clear();
}

void
CCardDeck::
save(CFile &file)
{
  init();

  for (const auto &card : cards_on_) {
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

    cards_on_.push_back(card);
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

    std::swap(cards_on_[card1], cards_on_[card2]);
  }
}

void
CCardDeck::
undealAll()
{
  init();

  for (auto &card : cards_off_)
    cards_on_.push_back(card);

  cards_off_.clear();
}

void
CCardDeck::
dealAll()
{
  init();

  for (std::size_t i = 0; i < cards_on_.size(); i++)
    popCard();
}

CCard *
CCardDeck::
popCard()
{
  init();

  CCard *card = peekCard();

  cards_on_.pop_back();

  cards_off_.push_back(card);

  return card;
}

CCard *
CCardDeck::
peekCard() const
{
  init();

  return cards_on_[cards_on_.size() - 1];
}

CCard *
CCardDeck::
peekCard(int pos) const
{
  init();

  if (pos < 0 || pos >= (int) cards_on_.size())
    return nullptr;

  return cards_on_[pos];
}

void
CCardDeck::
pushCard(CCard *card)
{
  init();

  cards_off_.remove(card);

  cards_on_.push_back(card);
}

CCard *
CCardDeck::
undealCard(CCard::Suit suit, CCard::Value value)
{
  init();

  for (auto &card : cards_off_) {
    if (suit == card->getSuit() && value == card->getValue()) {
      cards_on_.push_back(card);

      cards_off_.remove(card);

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
