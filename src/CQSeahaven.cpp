#include <CQSeahaven.h>
#include <CQSeahavenCanvas.h>

#include <QApplication>

#include "ui_CQSeahaven.h"

CQSeahaven::
CQSeahaven()
{
  Ui::CQSeahaven ui;

  ui.setupUi(this);

  canvas_ = new CQSeahavenCanvas(ui.canvas);

  connect(ui.action_New_Game, SIGNAL(activated(int)), canvas_, SLOT(newGame()));
  connect(ui.action_Solve, SIGNAL(activated(int)), canvas_, SLOT(solve()));

  QVBoxLayout *vlayout = new QVBoxLayout();

  vlayout->setMargin(0);
  vlayout->setSpacing(0);

  ui.canvas->setLayout(vlayout);

  vlayout->addWidget(canvas_);
}

CQSeahaven::
~CQSeahaven()
{
}
