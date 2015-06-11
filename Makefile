CFLAGS = -I ./include
LFLAGS = $(LIB)  -lX11 -lGL -pthread -lm #-lXrandr

all: main test

main: main.cpp	
	g++ main.cpp -Wall -Wextra -L/usr/X11R6/lib -lglut  -lGL -lGLU -lm  -o main

test: test.cpp
	g++ $(CFLAGS) test.cpp -Wall -Wextra $(LFLAGS) -I/usr/X11R6/include -L/usr/X11R6/lib  -o test

clean:
	rm -f main 
	rm -f test
	rm -f *.o
