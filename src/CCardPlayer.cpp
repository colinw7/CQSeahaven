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
    return NULL;

  CCardList::const_iterator pcard1 = cards_.begin();
  CCardList::const_iterator pcard2 = cards_.end  ();

  for (int i = 0; i < pos && pcard1 != pcard2; i++)
    ++pcard1;

  if (pcard1 == pcard2)
    return NULL;

  return *pcard1;
}
