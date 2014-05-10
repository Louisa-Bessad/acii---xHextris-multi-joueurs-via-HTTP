#include "xhextris.h"
#include <time.h>
int main(int argc, char **argv){

	Display *dpy =  NULL;
	
	int rand_value;
	int num_window;
	
	if(argc != 2){
		fprintf(stderr, "usage: %s [num]\n", argv[0]);
		exit(-1);
	}
	
	num_window = atoi(argv[1]);
	dpy = XOpenDisplay(NULL);
	
	if (!dpy) {
		fprintf(stderr, "Erreur dpy\n");
		exit(-1);
	}
	
	XEvent event;
	event.type = KeyPress;
	event.xkey.window = (Window) numWindow;
	event.xkey.display = dpy;
	event.xkey.type = KeyPress;
	event.xkey.state = 0x10;
	event.xkey.keycode = 0x58;

	srandom(time(NULL));
	
	while(1){
	
		rand_value = random()%15;
		sleep(rand_value);
		printf("Perturbation du client\n");
		XSendEvent(dpy,(Window)numWindow, False, KeyPressMask,&event);
		XFlush(dpy);
		
	}
	
	return 0;

}
