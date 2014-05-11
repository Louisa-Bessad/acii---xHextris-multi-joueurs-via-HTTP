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
			    
  printf("Window: %d\n",(int)win);

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
	       ButtonPressMask|ButtonReleaseMask|ButtonMotionMask|PropertyChangeMask);

  /* Q3 */
  /* Window score creation */
  if(nb_joueurs > 0){
    window_score = XCreateSimpleWindow(
				       dpy,
				       DefaultRootWindow(dpy)
				       ,0,0,
				       WINDOWSCOREWIDTH,
				       (nb_joueurs+2)*HEXWIDTH,
				       0, white, black);
    XStoreName(dpy, window_score, "Scores");
    XMapWindow(dpy, window_score);
  }

  xhextris_querry_string = XInternAtom(dpy, "xhextris_querry_string", False);
  xhextris_score = XInternAtom(dpy, "xhextris_score", False);
  XGrabServer(dpy);
  XSetSelectionOwner(dpy, xhextris_querry_string, wincur, CurrentTime);
  XSetSelectionOwner(dpy, xhextris_score, wincur, CurrentTime);
  XUngrabServer(dpy);

  XStoreName(dpy, win, WINDOWNAME);
  XMapWindow(dpy, win);

  return(ConnectionNumber(dpy));
}
