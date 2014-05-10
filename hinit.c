/*  See header.h for licence */

#include "xhextris.h"

int xhextrisInit(char *argv[], int argc)
{

  if (! (dpy = XOpenDisplay(NULL))) {
    fprintf(stderr, "%s: dispositif %s injoignable\n", argv[0],
              XDisplayName(NULL));
    exit(-1);
  }

  black = BlackPixel(dpy, DefaultScreen(dpy));
  white = WhitePixel(dpy, DefaultScreen(dpy));

  win = XCreateSimpleWindow(dpy,DefaultRootWindow(dpy),0,0,
			    WINDOWIDTH,
			    (MAXROW+8)*HEXWIDTH,
			    0, white, black);

  wincur = XCreateSimpleWindow(dpy, win,
			       BORDERLENGTH, 
			       CURSOR_Y+(BORDERLENGTH>>1),
			       COORDCOL(MAXCOLUMN+3)-BORDERLENGTH, 
			       (CURSOR_H+(BORDERSUB<<1)),
			       0,
			       black,
			       white			       );
  XSelectInput(dpy, win,  KeyPressMask | ExposureMask );
  XSelectInput(dpy, wincur,
	       ButtonPressMask|ButtonReleaseMask|ButtonMotionMask);
  XStoreName(dpy, win, WINDOWNAME);
  XMapWindow(dpy, win);

  return(ConnectionNumber(dpy));
}
