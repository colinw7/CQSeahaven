#ifndef SEAHAVEN_MOVE_H
#define SEAHAVEN_MOVE_H

#include <CInstCount.h>

#include <vector>

class CSeahavenMoveSet;
class CSeahavenStack;
class CSeahavenWorkArea;
class CSeahavenPile;
class CSeahavenMove;
class CCard;

enum CSeahavenMoveType {
  STACK_TO_PILE,
  WORK_AREA_TO_PILE,
  WORK_AREA_TO_STACK,
  STACK_TO_STACK,
  STACK_TO_WORK_AREA
};

class CSeahavenMoveSetCmp {
 public:
  int operator()(CSeahavenMoveSet *a, CSeahavenMoveSet *b);
};

class CSeahavenMoveSetSet {
  CINST_COUNT_MEMBER(CSeahavenMoveSetSet);

  typedef std::vector<CSeahavenMoveSet *> MoveSetList;

 public:
  CSeahavenMoveSetSet();
 ~CSeahavenMoveSetSet();

  void delete_all();

  bool isEmpty() const { return (num_move_sets_ == 0); }

  int size() const { return num_move_sets_; }

  int getNumMoveSets() const { return num_move_sets_; }

  CSeahavenMoveSet *getMoveSet(int num);

  CSeahavenMoveSet *addMoveSet();
  CSeahavenMoveSet *addMoveSet(CSeahavenMoveSet &move_set);

  void addMoveSetSet(CSeahavenMoveSetSet &move_set);

  void execute();
  void undo();

  void reverse();

  void sort();

  void print(std::ostream &os) const;

  friend std::ostream &operator<<(std::ostream &os, const CSeahavenMoveSetSet &move_set);

 private:
  MoveSetList move_sets_;
  int         num_move_sets_;
};

class CSeahavenMoveSet {
  CINST_COUNT_MEMBER(CSeahavenMoveSet);

  typedef std::vector<CSeahavenMove *> MoveList;

 public:
  CSeahavenMoveSet();
  CSeahavenMoveSet(const CSeahavenMoveSet &move_set);
 ~CSeahavenMoveSet();

  void clear();

  CSeahavenMoveType getType() const;

  bool isEmpty() const { return (num_moves_ == 0); }

  void addMove(CSeahavenStack *stack1, CSeahavenStack *stack2);
  void addMove(CSeahavenStack *stack, CSeahavenWorkArea *work_area);
  void addMove(CSeahavenStack *stack, CSeahavenPile *pile);
  void addMove(CSeahavenWorkArea *work_area, CSeahavenStack *stack);
  void addMove(CSeahavenWorkArea *work_area, CSeahavenPile *pile);

  void addMoveSet(CSeahavenMoveSet &move_set);

  bool isToPileMove() const;

  void execute();
  void undo();

  void print(std::ostream &os) const;

  friend std::ostream &operator<<(std::ostream &os, const CSeahavenMoveSet &move_set);

 private:
  void delete_all();

 private:
  MoveList moves_;
  int      num_moves_;
};

class CSeahavenMove {
  CINST_COUNT_MEMBER(CSeahavenMove);

 public:
  CSeahavenMove(const CSeahavenMove &move);

  CSeahavenMove(CSeahavenStack *stack1, CSeahavenStack *stack2);
  CSeahavenMove(CSeahavenStack *stack, CSeahavenWorkArea *work_area);
  CSeahavenMove(CSeahavenStack *stack, CSeahavenPile *pile);
  CSeahavenMove(CSeahavenWorkArea *work_area, CSeahavenStack *stack);
  CSeahavenMove(CSeahavenWorkArea *work_area, CSeahavenPile *pile);

  CSeahavenMoveType getType() const { return type_; }

  bool isToPileMove() const;

  void execute();
  void undo();

  void print(std::ostream &os) const;

  friend std::ostream &operator<<(std::ostream &os, const CSeahavenMove &move);

 private:
  CSeahavenMoveType   type_;
  CSeahavenStack     *stack1_;
  CSeahavenStack     *stack2_;
  CSeahavenWorkArea  *work_area_;
  CSeahavenPile      *pile_;
};

#endif
