FLAGS = -O2
CC = g++
GL = -lglut -lGL -lGLU 

ALL = prac2 ninot_neu carga_objeto carga_legoman

all: $(ALL)

ninot_neu: ninot_neu.c
	$(CC) $(FLAGS) ninot_neu.c -o ninot_neu $(GL) 

carga_objeto: carga_objeto.c
	$(CC) $(FLAGS) carga_objeto.c model.cpp -o carga_objeto $(GL) 

carga_legoman: carga_legoman.c
	$(CC) $(FLAGS) carga_legoman.c model.cpp -o carga_legoman $(GL) 

prac2: prac2.c
	$(CC) $(FLAGS) prac2.c model.cpp -o prac2 $(GL)

clean:
	rm $(ALL)
