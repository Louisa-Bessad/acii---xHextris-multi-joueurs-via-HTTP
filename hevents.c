#include "xhextris.h"

char xhextrisExposeEvent(XExposeEvent *ev)
{
  DEBUGPRINT(("Exp: '%d'\n",ev->window));;
  return ((ev->count) ? 0 : 'r');
}

char xhextrisKeyPress(XKeyPressedEvent *ev)
{
  KeySym key;
  XComposeStatus compose;
  char buffer[8];
  int  bufsize=8;

  XLookupString(ev,buffer,bufsize, &key, &compose);
  DEBUGPRINT(("KeyEvent: '%s'\n", buffer));
  return (buffer[(strncmp(buffer, "\024[", 2) ? 0 : 3)]);
}

char xhextrisButtonPress(XButtonEvent *ev)
{
  int w = ev->x;
  XEvent xe;

  while (1) {
    w=xhextrisSetspeed(w);
    XMaskEvent(dpy, ButtonPressMask|ButtonReleaseMask|ButtonMotionMask,&xe);
    if (xe.type == MotionNotify)
      {w= xe.xmotion.x;}
    else if (xe.type == ButtonRelease) break;          
  }
  button_speed=w+(CURSOR_H>>1);
  return 0;
}
