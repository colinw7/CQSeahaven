#include <CQSeahaven.h>
#include <CQImage.h>
#include <CQFont.h>
#include <QApplication>

int
main(int argc, char **argv)
{
  QApplication app(argc, argv);

  CQImage::setPrototype();

  CQFontMgrInst->setPrototype();

  CQSeahaven seahaven;

  seahaven.show();

  return app.exec();
}
