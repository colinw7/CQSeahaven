#ifndef CCardPlayer_H
#define CCardPlayer_H

#include <CInstCount.h>
#include <string>
#include <list>

class CCard;

class CCardPlayer {
  CINST_COUNT_MEMBER(CCardPlayer);

 public:
  CCardPlayer(const std::string &name);

  virtual ~CCardPlayer();

  void addCard   (CCard *card);
  void removeCard(CCard *card);

  int getNumCards() const;

  CCard *getCard(int pos) const;

 protected:
  typedef std::list<CCard *> CCardList;

  std::string name_;
  CCardList   cards_;
};

#endif
