#include "xhextris.h"

char xhextrisExposeEvent(XExposeEvent *ev)
{
  DEBUGPRINT(("Exp: '%d'\n",ev->window));;
  return ((ev->count) ? 0 : 'r');
}

char xhextrisKeyPress(XKeyPressedEvent *ev)
{
  KeySym key;
  XComposeStatus compose;
  char buffer[8];
  int  bufsize=8;

  XLookupString(ev,buffer,bufsize, &key, &compose);
  DEBUGPRINT(("KeyEvent: '%s'\n", buffer));
  return (buffer[(strncmp(buffer, "\024[", 2) ? 0 : 3)]);
}

char xhextrisButtonPress(XButtonEvent *ev)
{
  int w = ev->x;
  XEvent xe;

  while (1) {
    w=xhextrisSetspeed(w);
    XMaskEvent(dpy, ButtonPressMask|ButtonReleaseMask|ButtonMotionMask,&xe);
    if (xe.type == MotionNotify)
      {w= xe.xmotion.x;}
    else if (xe.type == ButtonRelease) break;          
  }
  button_speed=w+(CURSOR_H>>1);
  return 0;
}

char xhextrisNotifyProperty(XPropertyEvent *event)
{
  /* Add n lines */
  xhextris_querry_string = XInternAtom(dpy, "xhextris_querry_string", False);
  /* Score */
  xhextris_score = XInternAtom(dpy, "xhextris_score", False);
  if(event->atom==xhextris_querry_string) {
    xhextrisAddNRowInBottom(event);
  } else if(event->atom==xhextris_score){
    xhextrisScore(event);
  }
  return 0;
}

void xhextrisScore(XPropertyEvent *event){

  Atom cur_type;
  int cur_format;
  long nb_items;
  long bytes;
  char *data[256];
  char buf[20];
  int status,score,pos,indice,i=0, sc=0;
  listIP *li = list_IP;

  printf("xhextris score\n");

  status = XGetWindowProperty(dpy,
    wincur,
    xhextris_score,
    0,         
    64,        
    False,     
    AnyPropertyType,  
    &cur_type,
    &cur_format,
    &nb_items,
    &bytes,       
    data);

  if (status != Success) {
    fprintf(stderr, "status = %d\n", status);
    exit(1);
  }

  score = atoi(getParametre("score=",data[0]));
  pos = posC(data[0],' ');
  indice = strlen(data[0]);

  char *ip = (char *) malloc(indice-pos);
  strncpy(ip,data[0]+pos+1,indice-pos-1);
  printf("IP:%s\n",ip);

  while(li){
    if(!strcmp(li->ip,ip)){
      li->score = score;
      sprintf(buf,"%d",score);
      XClearWindow(dpy,window_score);
      /* IP */
      XDrawString(dpy, window_score, gc, 50, (i+2) * HEXWIDTH,li->ip, strlen(li->ip));
      /* Score */
      XDrawString(dpy, window_score, gc, WINDOWSCOREWIDTH-55, (i+2) * HEXWIDTH,buf, strlen(buf));
      break;
    }
    i++;
    li = li->next;
  }

  XGrabServer(dpy);
  XSetSelectionOwner(dpy, xhextris_score, wincur, CurrentTime);
  XUngrabServer(dpy);

}

void xhextrisAddNRowInBottom(XPropertyEvent *event){

  Atom cur_type;
  int cur_format;
  long nb_items;
  long bytes;
  char *data[256];
  int status;

  printf("xhextris add row in bottom\n");

  status = XGetWindowProperty(dpy,
    wincur,
    xhextris_querry_string,
    0,         
    64,        
    False,     
    AnyPropertyType,  
    &cur_type,
    &cur_format,
    &nb_items,
    &bytes,       
    data);

  if (status != Success) {
    fprintf(stderr, "status = %d\n", status);
    exit(1);
  }

  int nb_lines = atoi(getParametre("nb_lines=",data[0]));
  char *color = getParametre("color=",data[0]);
  /*char *ip = getAddrIP(data[0]);
    char *login = getLogin(data[0]);*/

  addBottomLines(color,nb_lines);

  XGrabServer(dpy);
  XSetSelectionOwner(dpy, xhextris_querry_string , wincur, CurrentTime);
  XUngrabServer(dpy);

}

char* getParametre(char *param,char *message){
  char *p = message;
  static char buf[15];
  int nb_char = 0;
  if(p!=NULL && strstr(message,param)!=NULL){
    p = strstr(message,param);
    p = p + strlen(param);
    while(*p!=' ')
    {
       buf[nb_char] = *p;
       p++;
        nb_char++;
    }
  }
  buf[nb_char] = '\0';
  return buf;
}

char* getAddrIP(char *str){
  int pos = posC(str, ' ');
  int indice = posC(str, '/');
  char *addr= (char *) malloc(indice-pos);
  strncpy(addr,str+pos+1,indice-pos-1);
  return addr;
}

char* getLogin(char *str){
  int pos = posC(str, '/');
  int indice = strlen(str);
  char *login = (char *) malloc(indice-pos);
  printf("%d-%d-%s\n",indice-pos-2,indice,str);
  strncpy(login,str+pos+1,indice-pos);
  return login;
}

void addBottomLines(char *color, int nb_lines){
  int line;
  int row;
  int position;
  char *p;
  char *q;
  /* Toutes les cellules de 0 à  MAXROW - nb_lignes sont remontées */
  for (line = 0; line < MAXROW - nb_lines; line++){
    for (row = 0; row < MAXCOLUMN; row++) {
      p=&grid[line][row];
      q=&grid[line+nb_lines][row];
      if (*p != *q) { 
        *p = *q; 
        xhextrisHex(line,row,*p);
      }
    }
  }

  srand(time(NULL));
  /* Ajout des trous et NO_PIECE */
  for (line = MAXROW - nb_lines; line <MAXROW; line++){
    position = random()%MAXCOLUMN;
    for (row = 0; row < MAXCOLUMN; row++) {
      unsigned char *p=&grid[line][row];
      if(row != position)
        *p=searchColor(color);
      else
        *p=NOPIECE;
      xhextrisHex(line,row,*p);
    }
  }
}

int searchColor(char * color){
  if(color){
    char color_buf[25];
    strcpy(color_buf,color);
    strcat(color_buf,"1");
    int i;
    for(i=0;i<NUMBEROFPIECES+2;i++){
      if(strcmp(color_buf,PieceNamedColors[i])==0){
        return i;
      }
    }
  }
  return 2;
}

int posC(const char *s, char c){
  unsigned int i;
  for (i = 0; s[i] != '\0'; i++) {
    if (s[i] == c) {
      return i;
    }
  }
  return -1;
}
