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

#include <stdio.h>
#include <strings.h>
#include <sys/types.h>
#include <sys/dir.h>
#include <sys/time.h>
#include <sys/time.h>
#include <pwd.h>
#include <unistd.h>
#include <stdlib.h>
#include <locale.h>
#include <getopt.h>
#include <X11/Xutil.h>
#include <X11/Xlib.h>

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

#ifdef DEBUG
#define DEBUGPRINT(x) {printf x;fflush(NULL);}
#else
#define DEBUGPRINT(x)
#endif

