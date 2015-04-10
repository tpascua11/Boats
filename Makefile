all: main

main: main.cpp	
	g++ main.cpp -Wall -Wextra -L/usr/X11R6/lib -lglut  -lGL -lGLU -lm  -o main

clean:
	rm -f main 
	rm -f *.o

