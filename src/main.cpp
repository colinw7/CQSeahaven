#include <CQApp.h>
#include <CQSeahaven.h>

int
main(int argc, char **argv)
{
  CQApp app(argc, argv);

  CQSeahaven seahaven;

  seahaven.show();

  return app.exec();
}
