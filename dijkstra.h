#ifndef DIJKSTRA_H
#define DIJKSTRA_H

#include "utility.h"

int minDistance(int *dist, int *sps, int vertex);

void printStations(int *pred, int v, stationGraph *g, FILE *file);

void printClients(int *pred, int v, stationGraph *g, int idx, FILE *file);

void stationDijkstra(stationGraph *g, int source, int dest, FILE *file);

void clientsDijkstra(stationGraph *g, int source, int dest, int idx, FILE *file);

#endif