#ifndef SEAHAVEN_HISTORY_H
#define SEAHAVEN_HISTORY_H

#include <CInstCount.h>
#include <vector>

class CSeahaven;
class CSeahavenStack;
class CSeahavenWorkArea;
class CSeahavenMove;

class CSeahavenHistory {
  CINST_COUNT_MEMBER(CSeahavenHistory);

 public:
  CSeahavenHistory(CSeahaven *seahaven);

  void moveStackToStack(CSeahavenStack *stack1, CSeahavenStack *stack2);
  void moveStackToWork(CSeahavenStack *stack, CSeahavenWorkArea *work_area);
  void moveWorkToStack(CSeahavenWorkArea *work_area, CSeahavenStack *stack);

  void tidy(CSeahavenMove *move);

 private:
  bool doTidy(CSeahavenMove *move);

 private:
  CSeahaven                    *seahaven_;
  std::vector<CSeahavenMove *>  moves_;
};

#endif
