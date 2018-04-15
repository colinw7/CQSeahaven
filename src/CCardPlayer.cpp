#include <CCardPlayer.h>

CCardPlayer::
CCardPlayer(const std::string &name) :
 name_(name)
{
}

CCardPlayer::
~CCardPlayer()
{
}

void
CCardPlayer::
addCard(CCard *card)
{
  cards_.push_back(card);
}

void
CCardPlayer::
removeCard(CCard *card)
{
  cards_.remove(card);
}

int
CCardPlayer::
getNumCards() const
{
  return cards_.size();
}

CCard *
CCardPlayer::
getCard(int pos) const
{
  if (pos < 0 || pos >= (int) cards_.size())
    return nullptr;

  int i = 0;

  for (const auto &card : cards_) {
    if (i == pos)
      return card;

    ++i;
  }

  return nullptr;
}
