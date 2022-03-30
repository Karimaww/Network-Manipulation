CC = gcc
CFLAGS = -Wall -Wextra -g

PROGRAMS = ChaineMain ReconstitueReseau HachageMain ArbreQuatMain GrapheMain MainTemps

all : $(PROGRAMS)

%.o: %.c %.h
	$(CC) $(CFLAGS) -c $< 

ChaineMain: ChaineMain.o Chaine.o SVGwriter.o
	$(CC) $(CFLAGS) -o $@ $^ -lm

HachageMain: HachageMain.o Hachage.o Reseau.o Chaine.o SVGwriter.o
	$(CC) $(CFLAGS) -o $@ $^ -lm

ArbreQuatMain: ArbreQuatMain.o ArbreQuat.o Reseau.o Chaine.o SVGwriter.o
	$(CC) $(CFLAGS) -o $@ $^ -lm

GrapheMain: GrapheMain.o Graphe.o Reseau.o Chaine.o SVGwriter.o Struct_File.o Struct_Liste.o
	$(CC) $(CFLAGS) -o $@ $^ -lm 

ReconstitueReseau: ReconstitueReseau.o Chaine.o Reseau.o Hachage.o ArbreQuat.o SVGwriter.o
	$(CC) $(CFLAGS) -o $@ $^ -lm

MainTemps: MainTemps.o Chaine.o Reseau.o Hachage.o ArbreQuat.o SVGwriter.o
	$(CC) $(CFLAGS) -o $@ $^ -lm 

clean: 
	rm -f *.o $(PROGRAMS)
