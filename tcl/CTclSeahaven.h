#ifndef TCL_SEAHAVEN_H
#define TCL_SEAHAVEN_H

#include "CTclApp/CTclAppCanvas.h"

class CTclSeahaven;

class CTclSeahavenCmd : public CTclAppCanvasCmd {
 private:
  CTclSeahaven *seahaven_;

 public:
  CTclSeahavenCmd(CTclApp *app);
 ~CTclSeahavenCmd();

  string getCommandName () const { return "CTclSeahaven"; }
  string getCommandClass() const { return "CTclSeahaven"; }

  CTclAppCanvas *createInstance(CTclApp *app);
};

#endif
