#Dependencies: libglu1-mesa-dev  freeglut3 freeglut3-dev
FLAGS = -O2
CC = g++
GL = -lglut -lGL -lGLU 

ALL = prac3 

all: $(ALL)


prac3: prac3.c
	$(CC) $(FLAGS) prac3.c model.cpp -o prac3 $(GL)  


clean:
	rm $(ALL)
