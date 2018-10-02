all:
	gcc closestPair.c  -L/path/to/libs -lm -o submission

clean:
	rm -rf submission closestPair.o
