#ifndef UTILITY_H
#define UTILITY_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define INF 9999999

typedef struct Client{
	char *name;
	int time;
	int sum;
} Client;

typedef struct clientGraph{
	int V, E;
	int **Adj;
} clientGraph;

typedef struct Station{
	char *name;
	int nrClients;
	Client *client;
	clientGraph *clientGraph;
} Station;

typedef struct stationGraph{
	int V, E;
	int **Adj;
	Station *station;
} stationGraph;

int indexOfClient(char *name, Client *client, int nr);

int indexOfStation(char *name, Station *station, int nr);

stationGraph *allocStationGraph(int vertex);

clientGraph *allocClientGraph(int vertex);

stationGraph* readGraph(FILE *file);

void readTasks(FILE *fin, FILE *fout, stationGraph *g);

void freeMatrix(int **M, int n);

void freeGraph(stationGraph *g);

#endif