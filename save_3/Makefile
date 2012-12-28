#---------------------------------
# INITIALISATION DES VARIABLES 
#---------------------------------

# Indiquer le compilateur
CXX= gcc

# Les chemins ou se trouvent les fichiers a inclure
INCLUDE= -I/usr/include

# Options de compilation.
CXXFLAGS=  $(INCLUDE) -c -Wno-deprecated 

# Les chemins ou se trouvent les librairies
LIBRARY_PATH= -L/usr/lib -lpthread 

# Options pour le linker.
LDFLAGS= $(LIBRARY_PATH) -g -o

# Les librairies avec lesquelles on va effectuer l'édition de liens
LIBS=  

# Les fichiers sources de l'application
FILES= main.c circuit.c voiture.c system.c equipe.c section.c stand.c menu.c

#-----------
# LES CIBLES
#-----------
exe :  $(FILES:.c=.o)
	$(CXX) $(LDFLAGS) exe $(FILES:.c=.o) $(LIBS)

.PHONY : clean
clean:
	/bin/rm $(FILES:.c=.o) exe

#-----------------------------------------------------------------------------
# LES REGLES DE DEPENDANCE. Certaines sont implicites mais je recommande d'en 
# mettre une par fichier source. 
#-----------------------------------------------------------------------------
main.o:main.c main.h voiture.h equipe.h circuit.h system.h menu.h
voiture.o:voiture.c voiture.h equipe.h
equipe.o:equipe.c equipe.h voiture.h
system.o:system.c system.h
circuit.o:circuit.c circuit.h section.h
section.o:section.c section.h voiture.h
stand.o:stand.c stand.h voiture.h
menu.o:menu.c menu.h system.h

#---------------------------------
# REGLES DE COMPILATION IMPLICITES
#---------------------------------
%.o : %.c ; $(CXX) $(CXXFLAGS) $*.c

