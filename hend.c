/*  See header.h for licence */

#include "xhextris.h"


void xhextrisEnd(int n)
{
  if (gc) XFreeGC (dpy, gc);
  if (hexgc) XFreeGC (dpy, hexgc);
  if (win) XDestroyWindow (dpy, win);
  if (n>0)
    { fprintf(stderr, 
	      WINDOWNAME ": cannot alloc color %s.\n",
	      PieceNamedColors[n-1]);
    }
  XCloseDisplay (dpy);
  exit(n);
}
