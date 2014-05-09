/*  See header.h for licence */

/* Sauvegarde et restauration des meilleurs scores */

#include "xhextris.h"

char log_name [MAXUSERIDLENGTH];

void read_user()
{
    struct passwd  *pwent=getpwuid(getuid());
    char *p;

    if (pwent == (struct passwd *) NULL) {
      p = (char *)getenv("USER");
      if (p && (strlen(p)<MAXUSERIDLENGTH))
	strcpy(log_name,(p) ? p : "anon");
    } else {
      if (strlen(pwent->pw_name)<MAXUSERIDLENGTH)
	strcpy(log_name, pwent->pw_name); 
    }
}


/* Lecture du fichier des meilleurs scores. */

void read_high_scores(high_score_t high_scores[MAXHIGHSCORES])
{
    int i=0, j;
    FILE *high_score_file;
    char buffer[32];

    if ((high_score_file = fopen(HIGHSCOREFILE , "r")) == NULL) {
	fprintf(stderr,"xhextris: No previous score file.\n");
    } else {
      for (i = 0; i < MAXHIGHSCORES; i++) {
	fread(high_scores[i].userid,sizeof(char),MAXUSERIDLENGTH,
	      high_score_file);
	fread(buffer,sizeof(char),32,high_score_file);
	high_scores[i].score = atoi(buffer);
	fread(buffer,sizeof(char),32,high_score_file);
	high_scores[i].rows = atoi(buffer);
	fread(buffer,sizeof(char),32,high_score_file);
	high_scores[i].pieces = atoi(buffer);
	if (feof(high_score_file))
	  break;
      }
      fclose(high_score_file);
    }
    for (j = i; j < MAXHIGHSCORES; j++) {
	strcpy(high_scores[j].userid,"NONE");
	high_scores[j].score = 0;
	high_scores[j].rows = 0;
	high_scores[j].pieces = 0;
    }
}

/* Ecriture du meme. */

void write_high_scores(high_score_t high_scores[MAXHIGHSCORES])
{
    int i;
    FILE *high_score_file;
    char buffer[32];
    
    if ((high_score_file = fopen(HIGHSCOREFILE, "w")) == NULL) {
	fprintf(stderr,"xhextris: Can't open high score file.\n");
    }
    for (i = 0; i < MAXHIGHSCORES; i++) {
	fwrite(high_scores[i].userid,sizeof(char),MAXUSERIDLENGTH,
	       high_score_file);
	sprintf(buffer,"%ld",high_scores[i].score);
	fwrite(buffer,sizeof(char),32,high_score_file);
	sprintf(buffer,"%d",high_scores[i].rows);
	fwrite(buffer,sizeof(char),32,high_score_file);
	sprintf(buffer,"%d",high_scores[i].pieces);
	fwrite(buffer,sizeof(char),32,high_score_file);
    }
    fflush(high_score_file);
    fclose(high_score_file);
}


/* Test si un joueur a batu son propre record */
/* Si oui, celui-ci remplace le moins bon de ses MAXUSERHIGHS */

int is_high_score(int score, int rows, int pieces, high_score_t high_scores[MAXHIGHSCORES])
{
    int i,j, user_highs, added, equal, over;
    user_highs = added = over = equal = 0;
    for (i = 0; i < MAXHIGHSCORES; i++) {
	equal = (! strcmp(log_name,high_scores[i].userid));
	if (equal)
	    over = (++user_highs > MAXUSERHIGHS) ? 1 : 0;
	if (over && equal) {
	    for (j = i; j < (MAXHIGHSCORES - 1); j++) {
		strcpy(high_scores[j].userid, high_scores[j+1].userid);
		high_scores[j].score = high_scores[j+1].score;
		high_scores[j].rows = high_scores[j+1].rows;
		high_scores[j].pieces = high_scores[j+1].pieces;
	    }
	    strcpy(high_scores[MAXHIGHSCORES-1].userid, "NONE");
	    high_scores[MAXHIGHSCORES-1].score = 0;
	    high_scores[MAXHIGHSCORES-1].rows = 0;
	    high_scores[MAXHIGHSCORES-1].pieces = 0;
	    i--;
	    continue;
	}
	if ((high_scores[i].score <= score)
	    && ((equal && (user_highs == MAXUSERHIGHS))
	    || (user_highs < MAXUSERHIGHS)) && (! added))  {
	    if (! equal)
	      over = (++user_highs > MAXUSERHIGHS);
	    for (j = MAXHIGHSCORES - 1; j > i; j--) {
		strcpy(high_scores[j].userid, high_scores[j-1].userid);
		high_scores[j].score = high_scores[j-1].score;
		high_scores[j].rows = high_scores[j-1].rows;
		high_scores[j].pieces = high_scores[j-1].pieces;
	    }
	    strcpy(high_scores[i].userid, log_name);
	    high_scores[i].score = score;
	    high_scores[i].rows = rows;
	    high_scores[i].pieces = pieces;
	    added = 1;
	    }
    }
    return added;
}
