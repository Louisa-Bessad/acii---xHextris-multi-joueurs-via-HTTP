  #include "xhextris.h"

  void xhextrisCurtain(unsigned char grid[MAXROW][MAXCOLUMN])
  {
    int j, i;

    for (j = 0; j < MAXCOLUMN-1; j+=2) {
      for (i = MAXROW - 1; i >= 0; i--){
        xhextrisHex(i, j, BORDERPIECE); XFlush(dpy);usleep(CURTAIN<<2);}
        for (i = 0; i < MAXROW; i++){
          xhextrisHex(i, j+1, BORDERPIECE); XFlush(dpy);usleep(CURTAIN<<2);}
        }
     if (j==MAXCOLUMN-1) 
      for (i = MAXROW - 1; i >= 0; i--)
       {xhextrisHex(i, j, BORDERPIECE); XFlush(dpy);usleep(CURTAIN);}
     sleep(1);
     for (j= MAXCOLUMN-1; j>0; j-=2) {
      for (i = MAXROW - 1; i >= 0; i--)
       {xhextrisHex(i, j, NOPIECE); XFlush(dpy);usleep(CURTAIN);}
     for (i = 0; i < MAXROW; i++) 
       {xhextrisHex(i, j-1, NOPIECE); XFlush(dpy);usleep(CURTAIN);}
   }
   if (j==0) {
    for (i = MAXROW - 1; i >= 0; i--)
     {xhextrisHex(i, 0, NOPIECE); XFlush(dpy);usleep(CURTAIN);}
  }
  sleep(1);
}
