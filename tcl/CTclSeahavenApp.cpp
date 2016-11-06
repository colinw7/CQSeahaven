#include "std_c++.h"
#include "CTclApp/CTclApp.h"
#include "CTclSeahaven.h"

#include "CTclSeahaven.k"

class CTclSeahavenApp;

class CTclSeahavenApp : public CTclApp {
 private:
  CTclSeahavenCmd *cmd_;

 public:
  CTclSeahavenApp(int argc, char **argv);
 ~CTclSeahavenApp();

 protected:
  void addCommands();

  string getTclStr();
};

int
main(int argc, char **argv)
{
  CTclSeahavenApp app(argc, argv);
}

CTclSeahavenApp::
CTclSeahavenApp(int argc, char **argv) :
 CTclApp(argc, argv), cmd_(NULL)
{
  init();
}

CTclSeahavenApp::
~CTclSeahavenApp()
{
  delete cmd_;
}

void
CTclSeahavenApp::
addCommands()
{
  cmd_ = new CTclSeahavenCmd(this);
}

string
CTclSeahavenApp::
getTclStr()
{
  return CTclSeahaven_TK;
}
