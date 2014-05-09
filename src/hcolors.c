/*  See header.h for licence */

#include "xhextris.h"


int xhextrisColors(int numberofpieces)
{

  int i;
  
  for (i=0;i<=numberofpieces+1;i++)
    {
      if (!XAllocNamedColor(dpy, 
			    DefaultColormap(dpy, DefaultScreen(dpy)),
			    PieceNamedColors[i],
			    &PieceColors[i],
			    &PieceColors[i]))
	return i+1;
    }

  return 0;
}
