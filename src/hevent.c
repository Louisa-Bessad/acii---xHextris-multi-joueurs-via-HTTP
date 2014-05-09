#include "xhextris.h"

char xhextrisEvent ()
{
  XEvent report;

 if (!XPending(dpy)) 
   return 0;
 else {
   XNextEvent(dpy, &report);
   switch (report.type) {
   case Expose: return xhextrisExposeEvent(&report.xexpose);
   case KeyPress: return xhextrisKeyPress(&report.xkey);
   case ButtonPress: return xhextrisButtonPress(&report.xbutton);
   default:return 0;
   }
 }
}
