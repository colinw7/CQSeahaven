#ifndef SEAHAVEN_STACK_H
#define SEAHAVEN_STACK_H

#include <vector>
#include <iostream>
#include <CInstCount.h>

class CCard;
class CSeahavenStack;

class CSeahavenStackMgr {
  CINST_COUNT_MEMBER(CSeahavenStackMgr);

  typedef std::vector<CSeahavenStack *> StackList;

 public:
  CSeahavenStackMgr();
 ~CSeahavenStackMgr();

  CSeahavenStack *getStack(int num);

  void clearStacks();

  int getNumStacks() const;

  void print(std::ostream &os) const;

  friend std::ostream &operator<<(std::ostream &os, const CSeahavenStackMgr &stack_mgr);

 private:
  StackList stacks_;
};

class CSeahavenStack {
  CINST_COUNT_MEMBER(CSeahavenStack);

  typedef std::vector<CCard *> CardList;

 public:
  int getNum() const { return num_; }

  CSeahavenStack(int num);
 ~CSeahavenStack();

  int getNumCards() const { return num_cards_; }

  bool isEmpty() const { return (num_cards_ == 0); }

  int  getNumConsecutive() { return num_consecutive_; }

  bool isKingStack() const { return king_stack_; }
  bool isForceMoveStack() const { return force_move_stack_; }

  void getTopCards(std::vector<CCard *> &cards) const;

  int getNumTopCards() const;

  CCard *getTopCard() const;

  void clear();

  void   push(CCard *card);
  CCard *pop ();
  CCard *peek();
  CCard *peek(int i);

  void print(std::ostream &os) const;

  friend std::ostream &operator<<(std::ostream &os, const CSeahavenStack &stack);

 private:
  int      num_;
  CardList cards_;
  int      num_cards_;
  int      num_consecutive_;
  bool     king_stack_;
  bool     force_move_stack_;
};

#endif
