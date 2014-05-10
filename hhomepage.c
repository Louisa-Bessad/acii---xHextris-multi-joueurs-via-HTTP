#include "xhextris.h"

#define DISPLAYTEXT(x, y, text) \
 XDrawString(dpy, win, gc, x, y, text, strlen(text));

void xhextrisHomepage()
{
    int y, i, x;
    static char *header[] = {"Ordre","Joueur","Score","Ligne", "Pièce"};
    char message[16];

    XClearWindow(dpy,win);
    XMapWindow(dpy,wincur);

    for (i = 0, y=((CURSOR_H<<2))+32; i < 10; i++,y+=16)
      DISPLAYTEXT(50, y, help[i]);

    y += 32;

    for (i = 0, x=35; i < 5; i++) {
      DISPLAYTEXT(x, y, header[i]);
      x+=strlen(header[i])*9;
    }

    y += 16;

    for (i=0;
	 i<((MAXHIGHSCORES > MAXROW) ? MAXROW : MAXHIGHSCORES); 
	 i++,y+=16) {
      x=35;
      sprintf(message,"%3d",i+1);
      DISPLAYTEXT(x, y, message);
      x+=strlen(header[0])*10;
      DISPLAYTEXT(x, y,(high_scores[i].userid));
      x+=strlen(message)*10;
      sprintf(message,"%*ld",
	      (int) strlen(header[2]),
	      high_scores[i].score);
      DISPLAYTEXT(x, y,(message));
      x+=strlen(message)*10;
      sprintf(message,"%*d",
	      (int) strlen(header[3]),
	      high_scores[i].rows);
      DISPLAYTEXT(x, y, message);
      x+=strlen(message)*10;
      sprintf(message,"%*d",
	       (int) strlen(header[4]),
	      high_scores[i].pieces);
      DISPLAYTEXT(x, y, message);
    }
}
