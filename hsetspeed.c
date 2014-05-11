/*  See header.h for licence */

#include "xhextris.h"

/* This file contains the X I/O handling routines for hextris. */


int xhextrisSetspeed(int w)
{
  char buf[32];

  /* centrer autour de la souris */
  w-=(CURSOR_H>>1);
  if (w<=0)
    w=1;
  else { if (w> (COORDCOL(MAXCOLUMN+3)-BORDERLENGTH-(CURSOR_H)))
      w=(COORDCOL(MAXCOLUMN+3)-BORDERLENGTH-(CURSOR_H));
  }
  XClearWindow(dpy,wincur);
  XDrawImageString(dpy, wincur, hexgc, w, BORDERSUB,  "}", 1);
  speed= (int)((SPEED*w)/((SPEEDBUTTON)<<1));
  if (speed >= SPEED) speed=SPEED-1;
  sprintf(buf,"Chute en 0,%06ld secondes",speed);
  XSetForeground(dpy,gc,WhitePixel(dpy, DefaultScreen(dpy)));
  XClearArea(dpy,win,0,0, WINDOWIDTH,CURSOR_H,False);
  XDrawImageString(dpy, win, gc, BORDERLENGTH<<1,CURSOR_H<<2, buf, strlen(buf));
  return(w);
}

