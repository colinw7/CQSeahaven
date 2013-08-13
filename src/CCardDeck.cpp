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
  CImageNameSrc src("CCardDeck/back");

  back_image_ = CImageMgrInst->createImage(src);

  back_image_->read(card_back_data.data, card_back_data.len);

  for (int i = 0; i < NUM_CARDS; i++) {
    CCard *card = new CCard(this,
                            (CCard::CCardSuit ) (i / 13),
                            (CCard::CCardValue) (i % 13));

    cards_on_.push_back(card);
  }

  shuffle();
}

CCardDeck::
~CCardDeck()
{
  deleteCards();
}

void
CCardDeck::
deleteCards()
{
  int num_cards_on = cards_on_.size();

  for (int i = 0; i < num_cards_on; i++)
    delete cards_on_[i];

  cards_on_.clear();

  CCardOffList::iterator pcard1 = cards_off_.begin();
  CCardOffList::iterator pcard2 = cards_off_.end  ();

  for ( ; pcard1 != pcard2; ++pcard1)
    delete *pcard1;

  cards_off_.clear();
}

void
CCardDeck::
save(CFile &file)
{
  int num_cards_on = cards_on_.size();

  for (int i = 0; i < num_cards_on; i++) {
    int suit  = cards_on_[i]->getSuit ();
    int value = cards_on_[i]->getValue();

    file.printf("%c%02d\n", suit_chars[suit], value + 1);
  }
}

void
CCardDeck::
restore(CFile &file)
{
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

    int suit  = (strchr(suit_chars, suit_char) - suit_chars);
    int value = CStrUtil::toInteger(value_str) - 1;

    if (suit < 0 || suit > 4)
      throw "Bad CCardDeck File";

    if (value < 0 || value > 13)
      throw "Bad CCardDeck File";

    CCard *card = new CCard(this,
                          (CCard::CCardSuit ) suit,
                          (CCard::CCardValue) value);

    cards_on_.push_back(card);
  }
}

void
CCardDeck::
shuffle()
{
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
  CCardOffList::const_iterator pcard1 = cards_off_.begin();
  CCardOffList::const_iterator pcard2 = cards_off_.end  ();

  for ( ; pcard1 != pcard2; ++pcard1)
    cards_on_.push_back(*pcard1);

  cards_off_.clear();
}

void
CCardDeck::
dealAll()
{
  int num_cards_on = cards_on_.size();

  for (int i = 0; i < num_cards_on; i++)
    popCard();
}

CCard *
CCardDeck::
popCard()
{
  CCard *card = cards_on_[cards_on_.size() - 1];

  cards_on_.pop_back();

  cards_off_.push_back(card);

  return card;
}

CCard *
CCardDeck::
peekCard() const
{
  return cards_on_[cards_on_.size() - 1];
}

CCard *
CCardDeck::
peekCard(int pos) const
{
  if (pos < 0 || pos >= (int) cards_on_.size())
    return NULL;

  return cards_on_[pos];
}

void
CCardDeck::
pushCard(CCard *card)
{
  cards_off_.remove(card);

  cards_on_.push_back(card);
}

CCard *
CCardDeck::
undealCard(CCard::CCardSuit suit, CCard::CCardValue value)
{
  CCardOffList::iterator pcard1 = cards_off_.begin();
  CCardOffList::iterator pcard2 = cards_off_.end  ();

  for ( ; pcard1 != pcard2; ++pcard1) {
    if (suit == (*pcard1)->getSuit() && value == (*pcard1)->getValue()) {
      cards_on_.push_back(*pcard1);

      cards_off_.remove(*pcard1);

      return *pcard1;
    }
  }

  return NULL;
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
