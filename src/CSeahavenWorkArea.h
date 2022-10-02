#ifndef SEAHAVEN_WORK_AREA_H
#define SEAHAVEN_WORK_AREA_H

#include <vector>
#include <iostream>
#include <CInstCount.h>

enum { NUM_WORK_AREAS = 4 };

class CCard;
class CSeahavenStack;
class CSeahavenWorkArea;

class CSeahavenWorkAreaMgr {
  CINST_COUNT_MEMBER(CSeahavenWorkAreaMgr);

 public:
  CSeahavenWorkAreaMgr();
 ~CSeahavenWorkAreaMgr();

  uint getNumWorkAreas() const { return NUM_WORK_AREAS; }

  CCard             *getCard(int num);
  CSeahavenWorkArea *getWorkArea(int num);
  CSeahavenWorkArea *getFreeWorkArea();
  uint               getNumEmpty() { return num_empty_; }

  void getEmptyWorkAreas(std::vector<CSeahavenWorkArea *> &work_areas);

  void clearWorkAreas();

  void getTopCards(int num, std::vector<CSeahavenWorkArea *> &work_areas);

  bool isEmpty(int num) const;

  void   push(int num, CCard *card);
  CCard *pop(int num);
  CCard *peek(int num) const;

  void print(std::ostream &os) const;

  friend std::ostream &operator<<(std::ostream &os, const CSeahavenWorkAreaMgr &work_area_mgr);

 private:
  CCard             *cards_[NUM_WORK_AREAS];
  CSeahavenWorkArea *work_areas_[NUM_WORK_AREAS];
  uint               num_empty_;
};

class CSeahavenWorkArea {
  CINST_COUNT_MEMBER(CSeahavenWorkArea);

 public:
  CSeahavenWorkArea(CSeahavenWorkAreaMgr *mgr, int num);

  int getNum() const { return num_; }

  bool isEmpty() const;

  void   push(CCard *card);
  CCard *pop();
  CCard *peek() const;

 private:
  CSeahavenWorkAreaMgr *mgr_;
  int                   num_;
};

#endif
