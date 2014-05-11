#include <string.h>
/*
 * hextris 
 * V1 Copyright 1990: David Markley,dm3e@+andrew.cmu.edu, dam@cs.cmu.edu
 * V2 Copyright 2003: Emmanuel.Saint-James@ufr-info-p6.jussieu.fr
 * 
 * Permission to use, copy, modify, and distribute, this software and its
 * documentation for any purpose is hereby granted without fee, provided that
 * the above copyright notice appear in all copies and that both that
 * copyright notice and this permission notice appear in supporting
 * documentation, and that the name of the copyright holders be used in
 * advertising or publicity pertaining to distribution of the software with
 * specific, written prior permission, and that no fee is charged for further
 * distribution of this software, or any modifications thereof.  The copyright
 * holder make no representations about the suitability of this software for
 * any purpose.  It is provided "as is" without express or implied warranty.
 *
 * THE COPYRIGHT HOLDER DISCLAIM ALL WARRANTIES WITH REGARD TO THIS SOFTWARE,
 * INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO
 * EVENT SHALL THE COPYRIGHT HOLDERS BE LIABLE FOR ANY SPECIAL, INDIRECT OR
 * CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE,
 * DATA, PROFITS, QPA OR GPA, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE 
 * OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
 * PERFORMANCE OF THIS SOFTWARE.
 */
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/dir.h>
#include <sys/time.h>
#include <pwd.h>
#include <unistd.h>
#include <stdlib.h>
#include <locale.h>
#include <getopt.h>
#include <X11/Xutil.h>
#include <X11/Xlib.h>
#include <X11/Xresource.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h> 

/* initConnexion */
#define DEFAULTVERSION "HTTP/1.1"
#define DEFAULTSERVER "127.0.0.1"
#define DEFAULTPROTOCOL "http"
#define DEFAULTSCRIPT "SurVersion.php"
#define CLENGTH "Content-Length: "
#define CTYPE "Content-Type: "
#define CT_AXWFU "application/x-www-form-urlencoded"
#define CT_MFD "multipart/form-data"
#define LINELENGTH 512

/* Nombre de pièces */
#define NUMBEROFPIECES 10
/* Absence de piève */
#define NOPIECE 0
/* Bordure de la grille */
#define BORDERPIECE (NUMBEROFPIECES+1)

/* Nombre d'hexagones par pièce */
#define NUMBEROFHEX 4

/* Nombre de lignes. */
#define MAXROW 26

/* Nombre de colonnes */
#define MAXCOLUMN 13

/* Largeur d'un hexagone */
#define HEXWIDTH 20

/* Windows Sizes. */
#define BORDERLENGTH 20
#define BORDERSUB 2
#define WINDOWIDTH (COORDCOL(MAXCOLUMN+3)+BORDERLENGTH)

/* Coordonnées graphiques des lignes et colonnes logique */

#define COORDCOL(c) ((((c)>>1)*((2*20)-8)+(((c)&1)*(20-4)))+BORDERLENGTH)
#define COORDLIG(l,c) (((l)*(HEXWIDTH-1)) + (((c)& 1) * 10) + BORDERLENGTH)

/* Intervalle entre deux pièces: 1 seconde maximum */
#define SPEED 1000000

/* Accelération toutes les 10 lignes complétées */
#define ACCEL(x,y) (x/((y/10)+1))

/* Temps de levée de rideau: 2/1000 de seconde par hexagone */
#define CURTAIN 2000

/* dimension du curseur réglant la vitesse */
#define CURSOR_Y (HEXWIDTH)
#define CURSOR_H HEXWIDTH
#define SPEEDBUTTON ((WINDOWIDTH-(BORDERLENGTH<<1)-(CURSOR_H<<1))>>1)

/* Nom de la police des hexagones */
#define HEXFONTNAME "hex20" 

/* Nom de la police des test */
#define FONTNAME "-misc-fixed-bold-r-normal--13-120-75-75-c-80-iso8859-1" /*8x13B"*/

/* Nombre de joueurs mémorisés */
#define MAXHIGHSCORES 10

/* Taille maximum d'un nom de joueur */
#define MAXUSERIDLENGTH 9

/* Nombre de record par joueur */
#define MAXUSERHIGHS 1

/* Nom de la fenetre */
#define WINDOWNAME "xhextris"

/* The name on the icon. */
#define ICONNAME "xhextris"

/* Definition de piece. */

typedef struct piece_s
{
  unsigned char type;
  unsigned char rotation;
  unsigned char row;
  unsigned char column;
} piece_t;

/* Definition de score. */

typedef struct high_score_s
{
  char userid[MAXUSERIDLENGTH];
  unsigned long score;
  int rows;
  int pieces;
} high_score_t;

typedef struct listIP{       
  char *login;
  char *ip;
  char *color;   
  unsigned long score;
  struct listIP *next;
} listIP;

#define WINDOWSCOREWIDTH 200

#ifdef DEBUG
#define DEBUGPRINT(x) {printf x;fflush(NULL);}
#else
#define DEBUGPRINT(x)
#endif

#define HEXFONTDIR "/tmp"
#define HIGHSCOREFILE "xhextris-scores"
extern unsigned long button_speed;
extern unsigned long speed;
extern unsigned long score;
extern int total_pieces, rows;
extern unsigned char grid[MAXROW][MAXCOLUMN];
extern high_score_t high_scores[MAXHIGHSCORES];
extern short shape[NUMBEROFPIECES*6][16];
extern void new_piece(piece_t *npiece);
extern void show_next_piece(piece_t *npiece);
extern void init_piece(piece_t *piece);
extern void place_piece(piece_t *old, piece_t *new, int upos);
extern int check_piece(piece_t *tpiece, unsigned char grid[MAXROW][MAXCOLUMN]);
extern void drop_piece(piece_t *piece);
extern void redraw_grid();
extern void shift_redraw_grid(int start);
extern void shift_offset_redraw_grid(int start);
extern int check_rows(unsigned char grid[MAXROW][MAXCOLUMN]);
extern void update_drop(piece_t *npiece, piece_t *piece);
extern void update_left(piece_t *piece, piece_t tpiece);
extern void update_right(piece_t *piece, piece_t tpiece);
extern void update_rotateCCW(piece_t *piece, piece_t tpiece);
extern void update_rotateCW(piece_t *piece, piece_t tpiece);
extern void update_fall(piece_t *piece, piece_t tpiece);
extern void do_choice(char choice, piece_t *npiece, piece_t *piece);
extern void redraw_game(piece_t *npiece, piece_t *piece);
extern void new_game(piece_t *npiece, piece_t *piece);
extern void choose_choice(int go, piece_t *npiece, piece_t *piece);
extern int InitConnexion(char *serveur, int port);
extern void sendGeneClientRequests();
extern void double_speed();
extern int main(int argc, char **argv);
extern char log_name [MAXUSERIDLENGTH];
extern void read_user();
extern void read_high_scores(high_score_t high_scores[MAXHIGHSCORES]);
extern void write_high_scores(high_score_t high_scores[MAXHIGHSCORES]);
extern int is_high_score(int score, int rows, int pieces, high_score_t high_scores[MAXHIGHSCORES]);
extern Display *dpy;
extern Window win;
extern Window wincur;
extern GC gc;
extern GC hexgc;
extern GC pixgc;
extern XFontStruct *textfont;
extern Pixmap fond;
extern Pixmap black;
extern Pixmap white;
extern XColor PieceColors[NUMBEROFPIECES+2];
extern char *PieceNamedColors[NUMBEROFPIECES+2];
extern char *help[] ;
extern listIP *list_IP;
extern XrmDatabase xrm_database;
extern int nb_joueurs;
extern Atom xhextris_querry_string;
extern int cleared_rows;
extern Window window_score;
extern Atom xhextris_score;
extern char xhextrisEvent ();
extern int xhextrisColors(int numberofpieces);
extern void xhextrisEnd(int n);
extern void xhextrisScores(int score, int rows);
extern void xhextrisHex(int row, int column, int type) ;
extern void xhextrisHomepage();
extern int xhextrisSetspeed(int w);
extern GC xhextrisFont(char *name, char *dir, unsigned long fg, unsigned long bg);
extern int xhextrisInit(char *argv[], int argc);
extern char xhextrisExposeEvent(XExposeEvent *ev);
extern char xhextrisKeyPress(XKeyPressedEvent *ev);
extern char xhextrisButtonPress(XButtonEvent *ev);
extern char xhextrisNotifyProperty(XPropertyEvent *event);
extern void xhextrisScore(XPropertyEvent *event);
extern void xhextrisAddNRowInBottom(XPropertyEvent *event);
extern char* getParametre(char *param,char *message);
extern char* getAddrIP(char *str);
extern char* getLogin(char *str);
extern void addBottomLines(char *couleur, int nb_lines);
extern int searchColor(char * couleur);
extern int posC(const char *s, char c);
