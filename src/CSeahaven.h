#ifndef SEAHAVEN_H
#define SEAHAVEN_H

#include <CInstCount.h>
#include <CSeahavenMove.h>
#include <CFile.h>

enum { NUM_STATE_LISTS = 64 };

class CCard;
class CCardDeck;
class CSeahavenStackMgr;
class CSeahavenStack;
class CSeahavenPileMgr;
class CSeahavenPile;
class CSeahavenWorkAreaMgr;
class CSeahavenWorkArea;

class CSeahavenState {
  CINST_COUNT_MEMBER(CSeahavenState);

 public:
  CSeahavenState(CSeahavenStackMgr *stack_mgr, int num_stacks);

  int getKey() const { return key_; }

  friend int operator==(const CSeahavenState &lhs, const CSeahavenState &rhs);

 private:
  char c_[220];    // 10x22
  int  key_ { 0 };
};

class CSeahaven {
  using StateList = std::vector<CSeahavenState *>;

  CINST_COUNT_MEMBER(CSeahaven);

 public:
  static void setDebug(bool debug) { debug_ = debug; }
  static bool getDebug() { return debug_; }

  CSeahaven();
 ~CSeahaven();

  CCardDeck            *getDeck       () const { return deck_         ; }
  CSeahavenStackMgr    *getStackMgr   () const { return stack_mgr_    ; }
  CSeahavenPileMgr     *getPileMgr    () const { return pile_mgr_     ; }
  CSeahavenWorkAreaMgr *getWorkAreaMgr() const { return work_area_mgr_; }

  void deal();

  void shuffle();

  bool readDeck(const std::string &filename);

  bool solve(CSeahavenMoveSetSet &move_set_set);

  bool isSolved() const;

  bool getForcedStackMoveSet(CSeahavenMoveSet &move_set, bool &unsolvable);
  bool getForcedPileMoveSet(CSeahavenMoveSet &move_set);

  bool stacksLockEachOther(CSeahavenStack *stack1, CSeahavenStack *stack2);

  void getMoves(CSeahavenMoveSetSet &move_set_set);

  bool processMoves(CSeahavenMoveSetSet &move_set_set, int depth);

  void getStackToStackMoves(CSeahavenMoveSetSet &move_set_set);
  void getStackToWorkMoves (CSeahavenMoveSetSet &move_set_set);
  void getWorkToStackMoves (CSeahavenMoveSetSet &move_set_set);

  bool moveStackToStackValid(CSeahavenStack *stack1, CSeahavenStack *stack2) const;
  bool moveStackToWorkAreaValid(CSeahavenStack *stack1) const;
  bool moveStackToPileValid(CSeahavenStack *stack1) const;
  bool moveWorkAreaToStackValid(CSeahavenWorkArea *work_area, CSeahavenStack *stack2) const;
  bool moveWorkAreaToPileValid(CSeahavenWorkArea *work_area) const;

  void moveStackToStack(CSeahavenMoveSetSet &move_set_set, CSeahavenStack *stack1,
                        CSeahavenStack *stack2, int num);
  void moveStackToPile(CSeahavenMoveSetSet &move_set_set, CSeahavenStack *stack,
                       CSeahavenPile *pile, int num);
  void moveStackToWork(CSeahavenMoveSetSet &move_set_set, CSeahavenStack *stack, int num);
  void moveWorkToStack(CSeahavenMoveSetSet &move_set_set,
                       std::vector<CSeahavenWorkArea *> work_areas,
                       CSeahavenStack *stack, int num);
  void moveWorkToPile(CSeahavenMoveSetSet &move_set_set,
                      std::vector<CSeahavenWorkArea *> work_areas,
                      CSeahavenPile *pile, int num);
  void moveWorkToPile(CSeahavenMoveSetSet &move_set_set, CSeahavenWorkArea *work_area,
                      CSeahavenPile *pile);

  void addStackToStack(CSeahavenMoveSet *move_set, CSeahavenStack *stack1,
                       CSeahavenStack *stack2, int num);
  void addStackToPile(CSeahavenMoveSet *move_set, CSeahavenStack *stack,
                      CSeahavenPile *pile, int num);
  void addStackToWork(CSeahavenMoveSet *move_set, CSeahavenStack *stack, int num);
  void addWorkToStack(CSeahavenMoveSet *move_set,
                      CSeahavenWorkArea *work_area, CSeahavenStack *stack);
  void addWorkToStack(CSeahavenMoveSet *move_set,
                      std::vector<CSeahavenWorkArea *> work_areas,
                      CSeahavenStack *stack, int num);
  void addWorkToPile(CSeahavenMoveSet *move_set,
                     CSeahavenWorkArea *work_area, CSeahavenPile *pile);

  void save(CFile &file);
  void restore(CFile &file);

  bool checkState();

  void print(std::ostream &os) const;

  friend std::ostream &operator<<(std::ostream &os, const CSeahaven &seahaven);

 private:
  void deleteStates();

  bool solve1(CSeahavenMoveSetSet &move_set_set, int depth);

/*
  bool isKingMoveStackToStack(CSeahavenStack *stack);
  bool isKingMoveWorkToStack(CSeahavenStack *stack);
*/

 private:
  static bool debug_;

  using StateListList = std::vector<StateList>;
  using NumStates     = std::vector<int>;

  CCardDeck            *deck_           { nullptr };
  CSeahavenStackMgr    *stack_mgr_      { nullptr };
  int                   num_stacks_     { 0 };
  CSeahavenPileMgr     *pile_mgr_       { nullptr };
  int                   num_piles_      { 0 };
  CSeahavenWorkAreaMgr *work_area_mgr_  { nullptr };
  int                   num_work_areas_ { 0 };
  int                   iterations_     { 0 };
  StateListList         states_;
  NumStates             num_states_;
};

#endif
