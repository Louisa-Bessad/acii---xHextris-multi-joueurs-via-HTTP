#       Universite Pierre & Marie Curie
#       Master d'informatique
#       Architecture des Clients Internet
#       Travaux Diriges 6:  Typographie en X-window
#       Emmanuel Saint-James

# Attention, ceci doit etre un chemin absolu accessible au serveur X
CC ?= cc
CFLAGS ?= -g -O
BASE=~/
CONNU=$(BASE)M1/S2/acii/acii---xHextris-multi-joueurs-via-HTTP/
SOL=$(CONNU)
F=/tmp

B=xhextris
M=Makefile
H=header.h

D1=hevent
D2=hcolors
D3=hcurtain
D4=hend
D5=hscores
D6=hhex
D7=hhomepage
D8=hsetspeed
D9=hfont
D10=hinit
D11=hevents

G=grille
S=scores
A=globalx

ALLD=$(CONNU)$(G).c $(CONNU)$(S).c $(CONNU)$(A).c

ALLS=$(SOL)$(D1).c $(SOL)$(D2).c $(SOL)$(D3).c $(SOL)$(D4).c $(SOL)$(D5).c $(SOL)$(D6).c $(SOL)$(D7).c $(SOL)$(D8).c $(SOL)$(D9).c $(SOL)$(D10).c $(SOL)$(D11).c 

ALLO=$(G).o $(S).o $(A).o $(D1).o $(D2).o $(D3).o $(D4).o $(D5).o $(D6).o $(D7).o $(D8).o $(D9).o $(D10).o $(D11).o

O=-Wall -O4
I=-I/usr/X11R6/include -I$(CONNU)
L=-L/usr/X11R6/lib
l=-lX11

$(B):   $(ALLO) $F/hex20.pcf
	${CC} ${CFLAGS} $L $l $(ALLO) -o $(B)

%.o:	$(CONNU)%.c $(B).h
	${CC} ${CFLAGS} $O $I -c $< -o $@

%.o:	$(SOL)%.c $(B).h
	${CC} ${CFLAGS} $O $I -c $< -o $@

$(F)/hex20.pcf: $(CONNU)hex20.bdf
	@if [ -d $F ] ; then : ; else mkdir $F ; fi
	echo $F
	bdftopcf <  $(CONNU)hex20.bdf > $@
	@chmod 444 $@
	mkfontdir $F

test:
	./$(B) -a

raz:
	rm -rf $(ALLO) $(B) $(B).h $F

other:
	gcc $I -o hexagere hexagere.c ${L} ${l}
	gcc $I -o relais_lignes relais_lignes.c  ${L} ${l}

# Construction automatique du .h en fonction de l'evolution des fichiers
# Si rien de neuf, reprendre le vieux pour ne pas recompiler a chaque fois


$(B).h: $(CONNU)$(H) $(M) $(ALLD) $(ALLS)
	@if [ -f $@ ] ; then mv $@ $@.old ; else > $@.old ;  fi
	@(if [ "`uname`" == "Darwin" ]; then echo '#include <strings.h>' ; else echo '#include <string.h>'; fi) >> $@ 
	@grep -v '^#include *<string.*h>' $(CONNU)$(H) >>  $@
	@echo "#define HEXFONTDIR \"$F\"" >> $@
	@echo "#define HIGHSCOREFILE \"$(B)-$(S)\"" >> $@
	@awk -F[=\;{] '/^[a-zA-Z]/ {print "extern " $$1 ";"}' $(ALLD) $(ALLS) >> $@
	@if diff $@ $@.old >/dev/null;then mv $@.old $@  ; else rm $@.old; fi

# Construction de l'index des fonctions utilisees:
# toutes celles ayant au moins une majuscule et une minuscules
# et pas celle portant le nom de l'application

$(B).idx:       $(ALLS) $(M)
	@awk -F'/\\*' '{print $$1}' $(ALLS) | \
	tr "{}[]().,;:?<>#\t' =\!+%|\*&^-" "\012" | grep "^[A-Z].*[a-z]" | \
	grep -v $(B) | sort  -u | sed 's/True//g;/./s/^/\\par /g' > $(B).idx
	@wc  $(B).idx
