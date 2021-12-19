
project3: graph.o libgraph.a project3.o
	gcc -Wall -g project3.c -o project3 -lgraph -L.

graph.o: graph.c graph.h
	gcc -Wall -c graph.c -o graph.o

project3.o: project3.c graph.h
	gcc -Wall -g -c project3.c
	
libgraph.a: graph.o
	ar rcs libgraph.a graph.o

clean:
	rm *.o
