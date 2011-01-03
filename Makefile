#Projet OpenGL
CXX=g++
CC=gcc

UNAME := $(shell uname)

ifeq ($(UNAME), Linux)
LDFLAGS= -lglut -lGLU -lGLEW
CFLAGS= -Wall -I. -DLINUX
endif

ifeq ($(UNAME), Darwin)
LDFLAGS= -framework glew -framework GLUT -framework OpenGL -framework Cocoa
CFLAGS= -Wall -I. -DMAC
endif

EXE=projet

SRC= $(wildcard *.cpp)
OBJ= $(SRC:.cpp=.o)

$(EXE) : $(OBJ)
	$(CXX) $^ $(LDFLAGS) -o $(EXE)

#dependences des .h
main.o : terrain.h tga.h main.hpp shaders.hpp
terrain.o : terrain.h tga.h
tga.o : tga.h
shader.o : shader.hpp
object3d.o : object3d.h
objloader.o : objloader.h

#cible générique
%.o : %.cpp
	$(CXX) -c $(CFLAGS) $< -o $@
	
.PHONY : clean ultraclean

clean::
	rm -f *.o *~

ultraclean : clean
	rm -f $(EXE)

