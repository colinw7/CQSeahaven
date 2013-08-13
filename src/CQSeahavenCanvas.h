#ifndef CQSeahavenCanvas_H
#define CQSeahavenCanvas_H

#include <CSeahaven.h>
#include <CImageLib.h>

#include <QWidget>

class QPaintEvent;
class QMouseEvent;

enum CSeahavenAreaType {
  NO_TYPE,
  STACK_TYPE,
  PILE_TYPE,
  WORK_AREA_TYPE
};

class CSeahavenStack;
class CSeahavenPile;
class CSeahavenWorkArea;
class CQDragWindow;

union CSeahavenArea {
  CSeahavenStack    *stack;
  CSeahavenPile     *pile;
  CSeahavenWorkArea *work_area;
};

class CQSeahavenCanvas : public QWidget {
  Q_OBJECT

 public:
  CQSeahavenCanvas(QWidget *parent);
 ~CQSeahavenCanvas();

  void paintEvent(QPaintEvent *event);

  void mousePressEvent(QMouseEvent *event);
  void mouseMoveEvent(QMouseEvent *event);
  void mouseReleaseEvent(QMouseEvent *event);

  void drawDragCards(QPainter *painter);

 public slots:
  void newGame();

  void solve();

 private:
  void drawStacks();
  void drawPiles();
  void drawWorkAreas();
  void drawCard(CCard *card, int x, int y);
  void drawOutline(int x, int y);

  CSeahavenStack    *getStack(int x, int y);
  CSeahavenPile     *getPile(int x, int y);
  CSeahavenWorkArea *getWorkArea(int x, int y);

 private:
  CSeahaven             seahaven_;
  CImagePtr             card_images_[52];
  int                   card_width_;
  int                   card_height_;
  int                   card_dy_;
  int                   stack_x_;
  int                   stack_y_;
  int                   pile_x_;
  int                   pile_y_;
  int                   work_x_;
  int                   work_y_;
  CQDragWindow         *dragWindow_;
  std::vector<CCard *>  drag_cards_;
  CSeahavenAreaType     drag_area_type_;
  CSeahavenArea         drag_area_;
  int                   drag_dx_;
  int                   drag_dy_;
  QPainter             *painter_;
};

#endif
