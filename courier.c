#include "utility.h"
#include "dijkstra.h"
#include "tasks.h"

int main(int argc, char **argv)
{
	FILE *fin = fopen(argv[1], "rt");
	FILE *fout = fopen(argv[2], "wt");

	stationGraph *Graph = readGraph(fin);

	readTasks(fin, fout, Graph);

	freeGraph(Graph);

	fclose(fin);
	fclose(fout);
	
	return 0;
}