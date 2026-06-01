Flags = -Wpedantic -pedantic-errors -std=c++23
SDL_FLAGS = $(shell sdl2-config --cflags --libs)
CPP = g++

chip8 : main.o chip8.o
	$(CPP) $(Flags) -o chip8 main.o chip8.o $(SDL_FLAGS)

main.o : main.cpp chip8.h 
	$(CPP) -c $(Flags) $(SDL_FLAGS) main.cpp -o main.o

chip8.o : chip8.cpp chip8.h 
	$(CPP) -c $(Flags) chip8.cpp -o chip8.o