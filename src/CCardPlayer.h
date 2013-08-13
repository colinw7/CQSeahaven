#ifndef CCARD_PLAYER_H
#define CCARD_PLAYER_H

#include <CInstCount.h>
#include <string>
#include <list>

class CCard;

class CCardPlayer {
  CINST_COUNT_MEMBER(CCardPlayer);

 public:
  CCardPlayer(const std::string &name);
 ~CCardPlayer();

  void addCard(CCard *card);

  void removeCard(CCard *card);

  int getNumCards() const;

  CCard *getCard(int pos) const;

 private:
  typedef std::list<CCard *> CCardList;

  std::string name_;
  CCardList   cards_;
};

#endif
