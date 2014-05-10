/*  See header.h for licence */

#include "xhextris.h"

unsigned long button_speed=SPEEDBUTTON;
unsigned long speed= 1000000000;
unsigned long score;
int total_pieces, rows=0, game_over = 2, end_all_games=1, saved=1;
unsigned char grid[MAXROW][MAXCOLUMN];
high_score_t high_scores[MAXHIGHSCORES];

/* Ceci est la définition des pièces, composées chacune de 4 hexagones.
 * Chaque pièce a donc six rotations (pas forcément distinctes) définies
 * par 4 paires de coordonnées décrivant le déplacement des 4 hexagones.
 * Toutefois, la quinconce imposée par les hexagones dédouble ces 4 paires.
 * Il y a donc 16 nombres par ligne, qu'on aurait pu découper en 2. 
 */

short shape[NUMBEROFPIECES*6][16]=
  {   { 0, 0,-1, 0, 1,-1, 1, 1,  -1, 0, 0,-1, 0, 1, 0, 0},	/* 00 */
      { 0, 0, 0,-1, 0, 1, 1, 0,  -1,-1,-1, 1, 1, 0, 0, 0},	/* 01 */
      { 0, 0,-1, 0, 1,-1, 1, 1,  -1, 0, 0,-1, 0, 1, 0, 0},	/* 02 */
      { 0, 0, 0,-1, 0, 1, 1, 0,  -1,-1,-1, 1, 1, 0, 0, 0},	/* 03 */
      { 0, 0,-1, 0, 1,-1, 1, 1,  -1, 0, 0,-1, 0, 1, 0, 0},	/* 04 */
      { 0, 0, 0,-1, 0, 1, 1, 0,  -1,-1,-1, 1, 1, 0, 0, 0},	/* 05 */
      { 0, 0,-1, 0, 1, 0, 2, 0,  -1, 0, 1, 0, 2, 0, 0, 0},	/* 10 */
      { 0, 0, 0,-1, 1, 1, 1, 2,  -1,-1, 0, 1, 1, 2, 0, 0},	/* 11 */
      { 0, 0, 1,-1, 0, 1,-1, 2,   0,-1,-1, 1,-1, 2, 0, 0},	/* 12 */
      { 0, 0,-2, 0,-1, 0, 1, 0,  -2, 0,-1, 0, 1, 0, 0, 0},	/* 13 */
      { 0, 0,-1,-2, 0,-1, 1, 1,  -1,-2,-1,-1, 0, 1, 0, 0},	/* 14 */
      { 0, 0, 1,-2, 1,-1, 0, 1,   1,-2, 0,-1,-1, 1, 0, 0},	/* 15 */
      { 0, 0,-1, 0, 1, 0, 1, 1,  -1, 0, 1, 0, 0, 1, 0, 0},	/* 20 */
      { 0, 0, 0,-1, 1, 1, 0, 1,  -1,-1, 0, 1,-1, 1, 0, 0},	/* 21 */
      { 0, 0, 1,-1, 0, 1,-1, 0,   0,-1,-1, 0,-1, 1, 0, 0},	/* 22 */
      { 0, 0, 0,-1,-1, 0, 1, 0,  -1,-1,-1, 0, 1, 0, 0, 0},	/* 23 */
      { 0, 0, 0,-1, 1,-1, 1, 1,   0,-1,-1,-1, 0, 1, 0, 0},	/* 24 */
      { 0, 0, 1,-1, 1, 0, 0, 1,   0,-1, 1, 0,-1, 1, 0, 0},	/* 25 */
      { 0, 0,-1, 0, 1,-1, 1, 0,  -1, 0, 0,-1, 1, 0, 0, 0},	/* 30 */
      { 0, 0, 0,-1, 1, 0, 1, 1,  -1,-1, 1, 0, 0, 1, 0, 0},	/* 31 */
      { 0, 0, 1,-1, 0, 1, 1, 1,   0,-1,-1, 1, 0, 1, 0, 0},	/* 32 */
      { 0, 0,-1, 0, 1, 0, 0, 1,  -1, 0, 1, 0,-1, 1, 0, 0},	/* 33 */
      { 0, 0, 0,-1,-1, 0, 1, 1,  -1,-1,-1, 0, 0, 1, 0, 0},	/* 34 */
      { 0, 0, 0,-1, 1,-1, 0, 1,  -1,-1, 0,-1,-1, 1, 0, 0},	/* 35 */
      {-1, 0, 0,-1, 1,-1, 1, 0,  -1, 0,-1,-1, 0,-1, 1, 0},	/* 40 */
      { 0,-1, 1,-1, 1, 0, 1, 1,  -1,-1, 0,-1, 1, 0, 0, 1},	/* 41 */
      { 1,-1, 1, 0, 1, 1, 0, 1,   0,-1, 1, 0, 0, 1,-1, 1},	/* 42 */
      { 1, 0, 1, 1, 0, 1,-1, 0,   1, 0, 0, 1,-1, 1,-1, 0},	/* 43 */
      { 1, 1, 0, 1,-1, 0, 0,-1,   0, 1,-1, 1,-1, 0,-1,-1},	/* 44 */
      { 0, 1,-1, 0, 0,-1, 1,-1,  -1, 1,-1, 0,-1,-1, 0,-1},	/* 45 */
      {-1, 0, 1, 1, 2, 1, 0, 0,  -1, 0, 0, 0, 0, 1, 1, 1},	/* 50 */
      { 0,-1, 0, 0, 0, 1, 0, 2,  -1,-1, 0, 0,-1, 1, 0, 2},	/* 51 */
      { 1,-1, 0, 0,-1, 0,-1, 1,   0,-1, 0, 0,-1, 0,-2, 1},	/* 52 */
      { 1, 0, 0, 0, 0,-1,-1,-1,   1, 0, 0, 0,-1,-1,-2,-1},	/* 53 */
      { 0,-2, 1,-1, 0, 0, 1, 1,   0,-2, 0,-1, 0, 0, 0, 1},	/* 54 */
      { 2,-1, 1, 0, 0, 0, 0, 1,   1,-1, 1, 0, 0, 0,-1, 1},	/* 55 */
      { 0, 0, 1,-1, 1, 0, 1, 1,   0, 0, 0,-1, 1, 0, 0, 1},	/* 60 */
      { 0, 0, 1, 0, 1, 1, 0, 1,   0, 0, 1, 0, 0, 1,-1, 1},	/* 61 */
      { 0, 0, 1, 1, 0, 1,-1, 0,   0, 0, 0, 1,-1, 1,-1, 0},	/* 62 */
      { 0, 0, 0, 1,-1, 0, 0,-1,   0, 0,-1, 1,-1, 0,-1,-1},	/* 63 */
      { 0, 0,-1, 0, 0,-1, 1,-1,   0, 0,-1, 0,-1,-1, 0,-1},	/* 64 */
      { 0, 0, 0,-1, 1,-1, 1, 0,   0, 0,-1,-1, 0,-1, 1, 0},	/* 65 */
      {-1, 0, 0, 0, 1, 0, 2, 1,  -1, 0, 0, 0, 1, 0, 1, 1},	/* 70 */
      { 0,-1, 0, 0, 1, 1, 0, 2,  -1,-1, 0, 0, 0, 1, 0, 2},	/* 71 */
      { 1,-1, 0, 0, 0, 1,-1, 1,   0,-1, 0, 0,-1, 1,-2, 1},	/* 72 */
      { 1, 0, 0, 0,-1, 0,-1,-1,   1, 0, 0, 0,-1, 0,-2,-1},	/* 73 */
      { 0,-2, 0,-1, 0, 0, 1, 1,   0,-2,-1,-1, 0, 0, 0, 1},	/* 74 */
      { 2,-1, 1,-1, 0, 0, 0, 1,   1,-1, 0,-1, 0, 0,-1, 1},	/* 75 */
      {-1, 0, 0, 0, 1, 0, 2,-1,  -1, 0, 0, 0, 1, 0, 1,-1},    /* 80 */
      { 0,-1, 0, 0, 1, 1, 2, 1,  -1,-1, 0, 0, 0, 1, 1, 1},	/* 81 */
      { 1,-1, 0, 0, 0, 1, 0, 2,   0,-1, 0, 0,-1, 1, 0, 2},	/* 82 */
      { 1, 0, 0, 0,-1, 0,-1, 1,   1, 0, 0, 0,-1, 0,-2, 1},	/* 83 */
      {-1,-1, 0,-1, 0, 0, 1, 1,  -2,-1,-1,-1, 0, 0, 0, 1},	/* 84 */
      { 0,-2, 1,-1, 0, 0, 0, 1,   0,-2, 0,-1, 0, 0,-1, 1},	/* 85 */
      {-1, 0, 0, 0, 1,-1, 2,-1,  -1, 0, 0, 0, 0,-1, 1,-1},	/* 90 */
      { 0,-1, 0, 0, 1, 0, 2, 1,  -1,-1, 0, 0, 1, 0, 1, 1},	/* 91 */
      { 1,-1, 0, 0, 1, 1, 0, 2,   0,-1, 0, 0, 0, 1, 0, 2},	/* 92 */
      { 1, 0, 0, 0, 0, 1,-1, 1,   1, 0, 0, 0,-1, 1,-2, 1},	/* 93 */
      { 1, 1, 0, 0,-1, 0,-1,-1,   0, 1, 0, 0,-1, 0,-2,-1},	/* 94 */
      { 0,-2, 0,-1, 0, 0, 0, 1,   0,-2,-1,-1, 0, 0,-1, 1}};   /* 95 */

#define GETROTATION(type,rotation,column)				\
  &shape[(type-1)*6+rotation][(column&1) ? 0 : (NUMBEROFHEX<<1)]

/* Choisir entre un tirage aléatoire ou invariant de la pièce suivante */

#define NEXTPIECE ((((int)random()) % NUMBEROFPIECES)+1) 
// #define NEXTPIECE ((total_pieces % NUMBEROFPIECES)+1) 

void new_piece(piece_t *npiece)
{
  total_pieces++;
  npiece->type = NEXTPIECE ;
  npiece->rotation = 0;
  npiece->row = 1;
  npiece->column = MAXCOLUMN / 2;
  DEBUGPRINT(("piece %d: %d\n", total_pieces, npiece->type ));
}

/*  visualisation de la pièce à venir, en dessous de la grille */

void show_next_piece(piece_t *npiece)
{
  int i;
  short *p=GETROTATION(npiece->type, 0, 0);

  for (i = 0; i < ( (NUMBEROFHEX)<<1); i += 2) {
    xhextrisHex(MAXROW+5+p[i], (MAXCOLUMN>>1)+p[i+1] ,npiece->type);
  }
}

/* Placement de la pièce en haut de la grille    */
/* et reflet de sa position dans la ligne en dessous,  */
/* dont tous les hexagones sont redessinés, mais une fois seulement */

void init_piece(piece_t *piece)
{
  int i;
  int pos[MAXCOLUMN];
  short *p=GETROTATION(piece->type, piece->rotation, piece->column);

  for (i = 0; i < MAXCOLUMN; i++) { pos[i] = 0;} 

  for (i = 0; i <( (NUMBEROFHEX)<<1) ; i += 2) {
    DEBUGPRINT(("init_piece\n"));
    xhextrisHex(piece->row+(p[i]), piece->column+(p[i+1]), piece->type);
    xhextrisHex((MAXROW + 2),  piece->column+(p[i+1]), piece->type);
    pos[piece->column+(p[i+1])] = 1;
  }

  for (i = 0; i < MAXCOLUMN; i++)
    { if (!pos[i]) xhextrisHex(MAXROW + 2, i, NOPIECE);}
}

/* Placement de la pièce dans sa nouvelle position		*/
/* Chaque hexagone de la nouvelle est recherché dans l'ancienne.*/
/* S'il y figure, on évite de le redessiner.			*/
/* Symétriquement, chaque hexagone de l'ancienne trouvé dans la nouvelle */
/* n'a pas besoin d'etre effacé.					*/
/* Si upos !=0 la nouvelle position est refletée sur la ligne du bas */

void place_piece(piece_t *old, piece_t *new, int upos)
{
  int i, j, c, r, diff;
  int pos[MAXCOLUMN];
  int efface[NUMBEROFHEX<<1];
  short *p=GETROTATION(old->type, old->rotation, old->column);
  short *q=GETROTATION(new->type, new->rotation, new->column);

  if (upos) { for (i = 0; i < MAXCOLUMN; i++) { pos[i] = 0;} }
  for (i = 0; i < ((NUMBEROFHEX)<<1); i += 2) { efface[i] = 0 ; }
  for (i = 0; i < ((NUMBEROFHEX)<<1); i += 2) {
    c=new->column+q[i+1];
    r=new->row+q[i];
    pos[c] = 1;
    diff = 1;
    for (j = 0; j < ((NUMBEROFHEX)<<1); j += 2) {
      if ((r ==  old->row+p[j]) && (c == old->column+p[1+j])) {
	diff = 0;
	efface[j]=1;
	break;
      }
    }
    if (diff) {
      DEBUGPRINT(("place_piece %d %d\n", c ,r));
      xhextrisHex(r, c, new->type);
      if (upos) {
	DEBUGPRINT(("place_pos %d\n",c));
	xhextrisHex((MAXROW + 2), c, new->type);
      }
    }
  }
  if (upos) {
    for (i = 0; i < MAXCOLUMN; i++)
      { if (!pos[i]) xhextrisHex(MAXROW + 2, i, NOPIECE);} 
  } 
  c=old->column;
  r=old->row;
  for (i = 0; i < ( (NUMBEROFHEX)<<1); i += 2) {
    if (!efface[i]) {
      DEBUGPRINT(("place_pos 0 %d\n",c));
      xhextrisHex(r+p[i], c+p[i+1], NOPIECE);
    }
  }
}
   
/* Controle si un pièce peut se mettre à un certain endroit */

int check_piece(piece_t *tpiece, unsigned char grid[MAXROW][MAXCOLUMN])
{
  int i;
  int r=tpiece->row;
  int c=tpiece->column;
  short *p=GETROTATION(tpiece->type, tpiece->rotation, c);

  for (i = 0; i < ((NUMBEROFHEX)<<1); i += 2) {
    if (r+p[i] >= MAXROW) return 1;  /* en dessous de la grille ! */
    if (c+p[i+1] >= MAXCOLUMN) return 1; /* à droite de la grille ! */
    if (c+p[i+1] < 0) return 1;	 /* à gauche de la grille ! */
    if (grid[r+p[i]][c+p[i+1]] != NOPIECE) return 1; /* occupé !*/
  }

  return 0;
}

/* Inscrit la position finale de la piece dans la grille */

void drop_piece(piece_t *piece)
{
  int i;
  int row= piece->row;
  int col= piece->column;
  int type=piece->type;
  short *p=GETROTATION(type, piece->rotation, col);
  DEBUGPRINT(("drop[]%d\n",type));

  for (i = 0; i < ( (NUMBEROFHEX)<<1); i += 2) {
    grid[row+p[i]][col+p[1+i]] = type;
  }
}

/* Redessine la grille, avec sa bordure sur 3 cotés  */

void redraw_grid()
{
  int  j, i;

  for (i = MAXROW - 1; i >= 0; i--)
    for (j = 0; j < MAXCOLUMN; j++)
      xhextrisHex(i, j, grid[i][j]);

  for (i = 0; i <= MAXROW; i++) {
    xhextrisHex(i,-1,BORDERPIECE); 
    xhextrisHex(i,MAXCOLUMN,BORDERPIECE); 
  }

  for (i = 0; i < MAXCOLUMN; i++) xhextrisHex(MAXROW,i,BORDERPIECE);
}

/* Efface la ligne passée en argument, et décale le reste de la grille */

void shift_redraw_grid(int start)
{
  int row, column;
  unsigned char *p,*q;

  DEBUGPRINT(("shift_redraw:%d\n", start));
  for (row = start; row > 0; row--)
    for (column = 0; column < MAXCOLUMN; column++) {
      p=&grid[row][column];
      q=&grid[row-1][column];
      if (*p != *q) { *p = *q; xhextrisHex(row,column,*p); }
    }
  for (column = 0; column < MAXCOLUMN; column++) {
    if (grid[0][column] != NOPIECE) {
      grid[0][column] = NOPIECE;
      xhextrisHex(0,column,NOPIECE);
    }
  }
}

/* Meme chose, pour les lignes impaires  */

void shift_offset_redraw_grid(int start)
{
  int row, column, r;
  unsigned char *p,*q;

  DEBUGPRINT(("shift_offset:%d\n", start));
  for (row = start; row > 1; row--)
    for (column = 0; column < MAXCOLUMN; column++) {
      r=row-(column & 1);
      p=&grid[r][column];
      q=&grid[r-1][column];
      if (*p != *q) { *p = *q; xhextrisHex(r,column,*p); }
    }
  for (column = 0; column < MAXCOLUMN; column++) {
    if (grid[1-(column & 1)][column] != NOPIECE) {
      grid[1-(column & 1)][column] = NOPIECE;
      xhextrisHex(1-(column & 1),column,NOPIECE);
    }
  }
}

/* Recherche des lignes complètes, paires ou impaires */

int check_rows(unsigned char grid[MAXROW][MAXCOLUMN])
{
  int row, column, clear, clear_off, total_clear=0;

  for (row=MAXROW-1; row >= 0; row=(!(clear||clear_off))? row-1:row) {
    clear = 1;
    for (column = 0; column < MAXCOLUMN; column++)
      if (grid[row][column] == NOPIECE) {clear = 0; break;}
    if (clear) { shift_redraw_grid(row); total_clear++;}
    clear_off = 1;
    for (column = 0; column < MAXCOLUMN; column++)
      if (grid[row-(column & 1)][column] == NOPIECE) {clear_off=0; break;}
    if (clear_off) {shift_offset_redraw_grid(row); total_clear++;}
  }
  return total_clear;
}

/* Chute normale de pièce, avec contrôle d'arrivée */

void update_drop(piece_t *npiece, piece_t *piece)
{
  int clean_rows;
  piece_t tpiece = *piece;
  tpiece.row++;
  if (check_piece(&tpiece,grid)) {
    /* impossible de descendre plus bas */
    drop_piece(piece);
    clean_rows = check_rows(grid);
    score += (int)((clean_rows<<8) / button_speed);
    rows += clean_rows;
    xhextrisScores(score,rows);
    *piece=*npiece;
    new_piece(npiece);
    show_next_piece(npiece);
    init_piece(piece);
    game_over=(tpiece.row < NUMBEROFHEX-1);
  } else {
    /* plus molle sera la chute */
    piece->row=tpiece.row;
    tpiece.row--;
    place_piece(&tpiece,piece, 0);
    game_over=0; // ne sert qu'au premier passage
  }
}

void update_left(piece_t *piece, piece_t tpiece)
{  
  piece->column--;
  if (! check_piece(piece,grid))
    place_piece(&tpiece,piece, 1);
  else {
    piece->row -= (1-((piece->column & 1)*2));
    if (! check_piece(piece,grid)) {
      place_piece(&tpiece,piece, 1);
    } else {
      piece->column = tpiece.column;
      piece->row = tpiece.row;
    }
  }
}

void update_right(piece_t *piece, piece_t tpiece)
{
  piece->column++;
  if (! check_piece(piece,grid)) /* a droite sur la meme ligne */
    place_piece(&tpiece,piece, 1);
  else {
    piece->row -=(1-((piece->column & 1)*2)); /* a droite sur la ligne inf*/
    if (! check_piece(piece,grid)) {
      place_piece(&tpiece,piece, 1);
    } else {
      piece->column = tpiece.column;
      piece->row = tpiece.row;
    }
  }
}

void update_rotateCCW(piece_t *piece, piece_t tpiece)
{
  piece->rotation = (piece->rotation == 5) ? 0 : piece->rotation+1;
  if (! check_piece(piece,grid)) {
    place_piece(&tpiece,piece, 1);
  }else {
    piece->rotation = tpiece.rotation;
  }
}

void update_rotateCW(piece_t *piece, piece_t tpiece)
{
  piece->rotation = (!piece->rotation) ? 5 : piece->rotation-1;
  if (! check_piece(piece,grid)) {
    place_piece(&tpiece,piece, 1);
  }else {
    piece->rotation = tpiece.rotation;
  }
}

void update_fall(piece_t *piece, piece_t tpiece)
{
  tpiece.row++;
  while (! check_piece(&tpiece,grid)) {
    place_piece(piece,&tpiece,0);
    piece->row=tpiece.row;
    tpiece.row++;
  }
}

/* Aiguillage sur la touche frappée au clavier */

void do_choice(char choice, piece_t *npiece, piece_t *piece)
{
  switch (choice) {
  case '2':
    double_speed();
    break;
  case 'j': case 'J': case '4': case 'B':
    if (! game_over) {update_left(piece,*piece);}
    break;
  case 'l': case 'L': case '6': case 'C':
    if (! game_over) {update_right(piece,*piece);}
    break;
  case 'k': case 'K': case '5': case 'D':
    if (! game_over) {update_rotateCCW(piece,*piece);}
    break;
  case 'i': case 'I': case '8': case 'A':
    if (! game_over) {update_rotateCW(piece,*piece);}
    break;
  case ' ': case '0':
    if (! game_over) {update_fall(piece,*piece);}
    break;
  case 'q': case 'Q':
    game_over=1; 
    end_all_games=0;
    break;
  case 'r': case 'R':
    redraw_game(npiece, piece);
    break;
  case 'p': case 'P':
    game_over=2;
    redraw_game(npiece, piece);
    break;
  case 'u': case 'U':
    if (game_over==2) game_over=0;
    redraw_game(npiece, piece);
    break;
  case 'n': case 'N':
    if (game_over!=2){
      xhextrisScores(score, rows);
      xhextrisCurtain(grid);
    }
    new_game(npiece, piece);
    redraw_game(npiece,piece);
    break;
  }
}

/* réaffiche */

void redraw_game(piece_t *npiece, piece_t *piece)
{
  if (game_over==2) {
    xhextrisHomepage();
    xhextrisSetspeed(button_speed);
  } else {
    xhextrisScores(score,rows);
    redraw_grid();
    show_next_piece(npiece);
    init_piece(piece);
  }
}

/* Réinitialisation des variables globales. */

void new_game(piece_t *npiece, piece_t *piece)
{
  int row, column;
	
  for (row = 0; row < MAXROW; row++)
    for (column = 0; column < MAXCOLUMN; column++) {
      grid[row][column] =  NOPIECE;
    }
  game_over = saved = score = total_pieces=0;
  rows = 0;
  new_piece(piece);
  new_piece(npiece);
}

/* mode automatique */

void choose_choice(int go, piece_t *npiece, piece_t *piece)
{
  static char com[6]={'j','k','l','i',' ','j'};
  usleep(SPEED>>2);
  do_choice((go) ? 'n' : com[((int)random()) % 6], npiece, piece);
}

int InitConnexion(char *serveur, int port)
{
  int sock;
  struct sockaddr_in const *sin;
  char buf[LINELENGTH];
  char s_port[255];
  char *cause = NULL;  
  int i;
  int error;

  struct addrinfo addrHints, *addrRes; /* adresses désirées / récupérées */
  struct addrinfo *addrTmp;    /*adresse temporaire*/

  sprintf(s_port,"%i",port);  /*conversion du no de port en chaine de char*/
  memset(&addrHints, 0, sizeof(addrHints)); 
  addrHints.ai_family = AF_INET;  
  addrHints.ai_socktype = SOCK_STREAM;
 
  error = getaddrinfo(serveur, s_port, &addrHints, &addrRes);
  if (error) {
    printf("Client Error %d getaddrinfo error for host %s %s:\n", error,serveur, s_port); 
    printf("\t%s\n",gai_strerror(error));
    exit (EXIT_FAILURE);
  }
  i=0;
  /* searching for IP serveur */
  for (addrTmp = addrRes; addrTmp; addrTmp = addrTmp->ai_next) {
    i++;
    printf("socket no%d:\n\tdomain:%d type:%d protocole:%d\n",i,
	   addrTmp->ai_family, addrTmp->ai_socktype,addrTmp->ai_protocol);
    sin = (void *)addrTmp->ai_addr;
    if (inet_ntop(AF_INET, &sin->sin_addr, buf, sizeof(buf)) == NULL)
      printf(" inet_ntop : Echec \n");
    else
      printf("\t%s\n", buf);
  }
  /* connection  with the first socket */
  sock = -1;
  for (addrTmp = addrRes; addrTmp; addrTmp = addrTmp->ai_next) {
    sock = socket(addrTmp->ai_family, addrTmp->ai_socktype,addrTmp->ai_protocol);
    if (sock < 0) {
      cause = "socket";
      continue;}
    if (connect(sock, addrTmp->ai_addr, addrTmp->ai_addrlen) < 0) {
      cause = "connect";
      close(sock);
      sock = -1;
      continue;}
    break;} 
 
  if (sock < 0) {/*not reached*/
    printf("Erreur %s", cause);
  }
  return sock;
}

void sendGeneClientRequests(){
  listIP * list = list_IP;
  int sock;
  char request[1024];
  while(list){     
    sock = InitConnexion(list->ip,80);
    if (sock){
      sprintf(request,"GET /~%s/cgi-bin/relais_lignes?nombre_de_lignes=%d&couleur=%s HTTP/1.0\n",list->login,cleared_rows,PieceNamedColors[0]);
      printf("send resquest : %s",request);
      if(!write(sock,request,strlen(request))){
        printf("Erreur avec la socket du client %s\n",list->login);
        exit(1);
      }
      close(sock);
    }
    list=list->next;
  }
}


/* Augmentation de la vitesse par 2 */
void double_speed(){
  speed *= 2;
}

/* La boucle principale */

int main(int argc, char **argv)
{
  struct timeval tp;
  struct timezone tzp;
  char *fontdir = HEXFONTDIR;
  int autom=0;
  char key=0;
  fd_set fdst;
  int i, sock, oldscore = -1;
  piece_t npiece, piece;
  char opt;

  /* Variables supp */
  char * background_color = NULL;
  listIP * ip_list = NULL;
  int options_pos = 1;
  int j;
    
  setlocale(LC_MESSAGES, "");
  { // traitement des options

    const char * help_msg = 
      "usage: %s [-h][-a][-Ffontdir][-b]\n"
      "-h, --help\t\t\tDisplay help message.\n"
      "-a, --auto\t\t\tComputer plays alone.\n"
      "-F, --font-dir string\tstring indicates which font directory to use.\n"
      "-b, --background string\tstring indicates which background_color to apply\n";/*Q1*/
	
    struct option options[] = {
      {"help", no_argument, NULL, 'h'},
      {"auto", no_argument, NULL, 'a'},
      {"font-dir", required_argument, NULL, 'F'},
      {"background", required_argument, NULL, 'b'},/*Q1*/
      {NULL, 0, NULL, 0}
    };

    //fprintf(stderr, "LALALALALALA\n");

    while ((opt = getopt_long (argc, argv, "haF:b:", options, NULL)) != -1)
      //fprintf(stderr, "GETOOOOOOOOOOOOOOOOOOPT\n");
      switch(opt){
      case 'h':
	printf(help_msg, argv[0]);
	exit(0);

      case 'a':
	autom = 1;
	break;

      case 'F':
	fprintf(stderr, "-F, optarg: %p\n", optarg);
	fontdir = strdup(optarg);
	options_pos += 2;
	break;

      /* Traitement d'une nouvelle option: background_color */
      case 'b':
	fprintf(stderr, "-b, optarg: %p\n", optarg);
	background_color = strdup(optarg);
	options_pos += 2;
	break;

      default:
	fprintf(stderr, help_msg, argv[0]); exit(1);
      }
  }// FIN traitement des options
	

  /* Récupération des couples IP/login */
  for(j=options_pos;j<argc;j++){
    ip_list = malloc(sizeof(listIP));
		
    ip_list->ip = (char*) malloc (256);
    ip_list->login = (char*) malloc (256);
		
    
    //fprintf(stderr, "STRSEP1\n");
    strcpy(ip_list->ip, strsep(&argv[j],"/"));
    //fprintf(stderr, "STRSEP2\n");
    strcpy(ip_list->login, strsep(&argv[j],"/"));
		
    ip_list->score=0;
    ip_list->next = list_IP;   
    list_IP=ip_list;
    nb_joueurs++;       
  }

  read_user();
  read_high_scores(high_scores);
  
  // creation de la connexion et des fenetres 
  sock = xhextrisInit(argv, argc);
  if (!sock) {
    fprintf(stderr,"xhextris: Can't guess socket connection.\n");
    exit(100);
  }
    
  /* Récupération BD */
  if(XResourceManagerString(dpy)){
    xrm_database = XrmGetStringDatabase(XResourceManagerString(dpy));
  }

  /* Aplication de la couleur de fond */
  if (background_color&&xrm_database){ 
    XrmPutStringResource(&xrm_database, "xhextris.background",background_color);
  }
    
  // creation des couleurs si possible
  i = xhextrisColors(NUMBEROFPIECES);
  if (i) xhextrisEnd(i);
  // creation du contexte graphique pour les pieces
  hexgc = xhextrisFont(HEXFONTNAME,
		       fontdir,
		       PieceColors[NOPIECE].pixel,
		       PieceColors[NOPIECE].pixel);

  // creation du contexte graphique pour les scores
  gc = xhextrisFont(FONTNAME, "", white, black);
  gettimeofday(&tp, &tzp);
  srandom((int)(tp.tv_usec)); 

  while(end_all_games) {
    oldscore = -1;
    while ((score != oldscore)) {
      oldscore = score;
      speed = ACCEL(speed,rows);
      tp.tv_sec = 0;
      tp.tv_usec = speed;
      FD_ZERO(&fdst);
      FD_SET(sock,&fdst);
      select(sock+1,&fdst,0,0,&tp);
      key = xhextrisEvent();
      if (key) do_choice(key,&npiece,&piece);
    }
    if (autom) choose_choice(game_over,&npiece,&piece);
    if (!game_over) update_drop(&npiece,&piece);
    if (cleared_rows){
      /* Gestion des lignes de Gene pour les clients */
      sendGeneClientRequests();
    }

    if (game_over && !saved) {
      if (is_high_score(score, rows, total_pieces, high_scores))
	write_high_scores(high_scores);
      saved=1;
    }
  }
	
  xhextrisEnd(0);
  
  exit(0);
}
