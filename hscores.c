#include "xhextris.h"

#define DISPLAYTEXT(x, y, text)				\
  XDrawString(dpy, win, gc, x, y, text, strlen(text));

void xhextrisScores(int score, int rows)
{
  char scores[256];
  int l,s,a,d;
  XCharStruct xcs;

  XUnmapWindow(dpy,wincur); // precaution

  XClearArea(dpy,win,
	     0,
	     ((MAXROW+8)>>1)*((2*HEXWIDTH)-8),
	     COORDCOL(MAXCOLUMN+3)+BORDERLENGTH,
	     (100)*((2*HEXWIDTH)-8),
	     False);
  sprintf(scores,"Pièces:%3d", total_pieces);
  l = MAXROW*(HEXWIDTH+4);
  DISPLAYTEXT(1,l,scores);
  sprintf(scores,"Lignes:%3d", rows);
  XTextExtents(textfont, scores, strlen(scores), &s,&a,&d,&xcs);
  DISPLAYTEXT(WINDOWIDTH-xcs.width, l, scores);
  sprintf(scores,"Score:%12d", score);
  XTextExtents(textfont, scores, strlen(scores), &s,&a,&d,&xcs);
  XClearArea(dpy,win,0,0,WINDOWIDTH,xcs.ascent+xcs.descent,False);
  DISPLAYTEXT((WINDOWIDTH-xcs.width)>>1,xcs.ascent,scores);
}
