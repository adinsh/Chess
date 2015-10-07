all: chessprog

clean:
	-rm main.o chessprog

chessprog: xml_parser.o console.o gui.o main.o
	gcc  -o chessprog xml_parser.o console.o gui.o main.o -lm -std=c99 -pedantic-errors -g `sdl-config --libs`

main.o: xml_parser.c console.c gui.c main.c 
	gcc  -std=c99 -pedantic-errors -c -Wall -g -lm main.c `sdl-config --cflags`

console.o: console.c
	gcc  -std=c99 -pedantic-errors -c -Wall -g -lm console.c `sdl-config --cflags`

xml_parser.o: xml_parser.c
	gcc  -std=c99 -pedantic-errors -c -Wall -g -lm xml_parser.c `sdl-config --cflags`

gui.o: gui.c
	gcc  -std=c99 -pedantic-errors -c -Wall -g -lm gui.c `sdl-config --cflags` 
