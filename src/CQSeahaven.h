#ifndef CQSeahaven_H
#define CQSeahaven_H

#include <QMainWindow>

class CQSeahavenCanvas;

class CQSeahaven : public QMainWindow {
  Q_OBJECT

 private:
  CQSeahavenCanvas *canvas_;

 public:
  CQSeahaven();

  virtual ~CQSeahaven();
};

#endif
