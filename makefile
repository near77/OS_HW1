all:
	g++ -o linda -fopenmp linda.cpp
dbg:
	g++ -o linda -fopenmp linda.cpp -D dbg

