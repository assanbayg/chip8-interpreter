Flags = -Wpedantic -pedantic-errors -std=c++23
CPP = g++


chip8 : main.o chip8.o
	$(CPP) $(Flags) -o chip8 main.o chip8.o 

main.o : main.cpp chip8.h 
	$(CPP) -c $(Flags) main.cpp -o  main.o

chip8.o : chip8.cpp chip8.h 
	$(CPP) -c $(Flags) chip8.cpp -o  chip8.o
