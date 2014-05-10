#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <X11/Xutil.h>
#include <X11/Xlib.h>
#include <X11/Xatom.h>

int main (int argc, char *argv[]){  
   
   Display *dpy;
   Window window_dest;
   Atom xhextris_querry_string;
   
   char buf[256];
   printf ("Content-type: text/html\n");
   strcpy(buf,getenv ("QUERY_STRING"));
   strcat(buf," ");
   strcat(buf,getenv ("REMOTE_ADDR"));
  
  if (!(dpy = XOpenDisplay(NULL))) {
    printf("unreachable host\n");
    exit(-1);
  }

  xhextris_querry_string = XInternAtom(dpy, "xhextris_querry_string", True);
  XGrabServer(dpy);
  window_dest = XGetSelectionOwner(dpy, xhextris_querry_string); 
  XSetSelectionOwner(dpy, xhextris_querry_string, 0, CurrentTime); 
  XUngrabServer(dpy);
  printf("Window window_dest:%d\n",(int) window_dest);
  XChangeProperty(dpy,
  	window_dest,
  	xhextris_querry_string,
  	XA_STRING,
  	8,
  	PropModeReplace,
  	buf,
  	strlen(buf));
  XFlush(dpy);

  return (EXIT_SUCCESS);
}
