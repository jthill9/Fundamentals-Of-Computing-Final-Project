# Makefile for asteroids
#   make: creates executable

make: project.cpp 
	g++ -std=c++11 project.cpp gfx.o -lX11 -o asteroids

clean:
	rm asteroids

