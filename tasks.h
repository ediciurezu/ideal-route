#ifndef TASKS_H
#define TASKS_H

#include "utility.h"

int connection(char *client1, char *client2, stationGraph *g);

void link (char *station, stationGraph *g, FILE *file);

stationGraph* routeBlock(char *station1, char *station2, stationGraph *g);

stationGraph* streetBlock(char *client1, char *client2, stationGraph *g);

stationGraph* deleteRoute(char *station1, char *station2, stationGraph *g);

stationGraph* deleteStreet(char *client1, char *client2, stationGraph *g);

stationGraph* addRoute(char *station1, char *station2, int cost, stationGraph *g);

stationGraph* addStreet(char *client1, char *client2, int cost, stationGraph *g);

void stationOrder(stationGraph *g, int value, FILE *file);

void stationPath(char *station1, char *station2, stationGraph *g, FILE *file);

void clientPath(char *client1, char *client2, stationGraph *g, FILE *file);

int minIndexSearch(clientGraph *g, int v, int *viz);

int minPathSearch(clientGraph *g, int *source);

int stationTime(char *station, stationGraph *g);

#endif