/*  See header.h for licence */

#include "xhextris.h"


int xhextrisColors(int numberofpieces){

  int i;
  XrmValue value;
  char type[64];
  char *return_type[64];
  
  if (xrm_database){
    /* background_color exists */
    if (XrmGetResource(xrm_database,"xhextris.background","xhextris.background",return_type, &value)) {
      PieceNamedColors[0] = (char *) malloc (value.size+1);
      strncpy(PieceNamedColors[0], value.addr, value.size);   
    } else {
      XrmPutStringResource(&xrm_database, "xhextris.background", PieceNamedColors[0]); 
    }
			  
    for (i=1;i<=numberofpieces+1;i++){
      if (XrmGetResource(xrm_database,type,type, return_type, &value)) {
	PieceNamedColors[i] = (void *)malloc(value.size+1);
	strncpy(PieceNamedColors[i], value.addr, value.size);
      } else {
	XrmPutStringResource(&xrm_database, type, PieceNamedColors[i]);
      }
    }
  }
  
  for (i=0;i<=numberofpieces+1;i++){
    if (!XAllocNamedColor(dpy, DefaultColormap(dpy, DefaultScreen(dpy)),PieceNamedColors[i],&PieceColors[i],&PieceColors[i]))
      return i+1;
  }

  return 0;
}
