#include "tasks.h"
#include "dijkstra.h"

int connection(char *client1, char *client2, stationGraph *g)
{
	int idx;

	for (idx = 0; idx < g->V; idx++)
		if (indexOfClient(client1, g->station[idx].client, g->station[idx].nrClients) != -1) break;

	int x = indexOfClient(client1, g->station[idx].client, g->station[idx].nrClients);
	int y = indexOfClient(client2, g->station[idx].client, g->station[idx].nrClients);

	if (g->station[idx].clientGraph->Adj[x][y] && g->station[idx].clientGraph->Adj[x][y] != INF) return 1;
	return 0;
}

void link (char *station, stationGraph *g, FILE *file)
{
	int i;
	int x = indexOfStation(station, g->station, g->V);

	if (x == -1) { fprintf(file, "\n"); return; }

	for (i = 0; i < g->V; i++)
		if (g->Adj[x][i] && g->Adj[x][i] != INF) fprintf(file, "%s ", g->station[i].name);
	fprintf(file, "\n");
}

stationGraph* routeBlock(char *station1, char *station2, stationGraph *g)
{
	int x = indexOfStation(station1, g->station, g->V);
	int y = indexOfStation(station2, g->station, g->V);

	if (x == -1 || y == -1) return g;

	if (g->Adj[x][y]) 
		g->Adj[x][y] = g->Adj[y][x] = INF;

	return g;
}

stationGraph* streetBlock(char *client1, char *client2, stationGraph *g)
{
	int idx;

	for (idx = 0; idx < g->V; idx++)
		if (indexOfClient(client1, g->station[idx].client, g->station[idx].nrClients) != -1) break;

	int x = indexOfClient(client1, g->station[idx].client, g->station[idx].nrClients);
	int y = indexOfClient(client2, g->station[idx].client, g->station[idx].nrClients);

	if (x == -1 || y == -1) return g;

	if (g->station[idx].clientGraph->Adj[x][y])
		g->station[idx].clientGraph->Adj[x][y] = g->station[idx].clientGraph->Adj[y][x] = INF;

	return g;
}

stationGraph* deleteRoute(char *station1, char *station2, stationGraph *g)
{
	int x = indexOfStation(station1, g->station, g->V);
	int y = indexOfStation(station2, g->station, g->V);

	if (x == -1 || y == -1) return g;

	g->Adj[x][y] = g->Adj[y][x] = 0;

	return g;
}

stationGraph* deleteStreet(char *client1, char *client2, stationGraph *g)
{
	int idx;

	for (idx = 0; idx < g->V; idx++)
		if (indexOfClient(client1, g->station[idx].client, g->station[idx].nrClients) != -1) break;

	int x = indexOfClient(client1, g->station[idx].client, g->station[idx].nrClients);
	int y = indexOfClient(client2, g->station[idx].client, g->station[idx].nrClients);

	if (x == -1 || y == -1) return g;

	g->station[idx].clientGraph->Adj[x][y] = g->station[idx].clientGraph->Adj[y][x] = 0;

	return g;
}

stationGraph* addRoute(char *station1, char *station2, int cost, stationGraph *g)
{
	int x = indexOfStation(station1, g->station, g->V);
	int y = indexOfStation(station2, g->station, g->V);

	if (x == -1 || y == -1) return g;

	g->Adj[x][y] = g->Adj[y][x] = cost;

	return g;
}

stationGraph* addStreet(char *client1, char *client2, int cost, stationGraph *g)
{
	int idx;

	for (idx = 0; idx < g->V; idx++)
		if (indexOfClient(client1, g->station[idx].client, g->station[idx].nrClients) != -1) break;

	int x = indexOfClient(client1, g->station[idx].client, g->station[idx].nrClients);
	int y = indexOfClient(client2, g->station[idx].client, g->station[idx].nrClients);

	if (x == -1 || y == -1) return g;

	g->station[idx].clientGraph->Adj[x][y] = g->station[idx].clientGraph->Adj[y][x] = cost;

	return g;
}

void stationOrder(stationGraph *g, int value, FILE *file)
{
	int i, j;
	int cost, ok = 0;

	for (i = 0; i < g->V; i++)
	{
		cost = 0;

		for (j = 0; j < g->station[i].nrClients; j++)
			cost += g->station[i].client[j].sum;

		if (cost >= value) { fprintf(file, "%s ", g->station[i].name); ok = 1; }
	}

	if (ok) fprintf(file, "\n");
}

void stationPath(char *station1, char *station2, stationGraph *g, FILE *file)
{
	int x = indexOfStation(station1, g->station, g->V);
	int y = indexOfStation(station2, g->station, g->V);

	stationDijkstra(g, x, y, file);
}

void clientPath(char *client1, char *client2, stationGraph *g, FILE *file)
{
	int idx;

	for (idx = 0; idx < g->V; idx++)
		if (indexOfClient(client1, g->station[idx].client, g->station[idx].nrClients) != -1) break;

	int x = indexOfClient(client1, g->station[idx].client, g->station[idx].nrClients);
	int y = indexOfClient(client2, g->station[idx].client, g->station[idx].nrClients);

	clientsDijkstra(g, x, y, idx, file);
}

int minIndexSearch(clientGraph *g, int v, int *viz)
{
	int i, min = INF;
	int minIndex;

	for (i = 0; i < g->V; i++)
		if (g->Adj[v][i] != 0 && g->Adj[v][i] < min && !viz[i]) {min = g->Adj[v][i]; minIndex = i;}

	return minIndex;
}

int minPathSearch(clientGraph *g, int *source)
{
	int viz[g->V];
	int Sol[g->V];
	int cost = 0;

	int i;

	for (i = 0; i < g->V; i++)
	{
		viz[i] = 0;
	}

	for (i = 0; i < g->V - 1; i++)
	{
		Sol[i] = *source;
		viz[*source] = 1;
		int u = minIndexSearch(g, *source, viz);

		cost += g->Adj[*source][u];

		*source = u;
	}

	return cost;
}

int stationTime(char *station, stationGraph *g)
{
	int idx = indexOfStation(station, g->station, g->V);

	int i, min = INF;
	int minIndex;

	for (i = 0; i < g->station[idx].nrClients; i++)
		if (g->station[idx].client[i].time < min) { min = g->station[idx].client[i].time, minIndex = i; }

	int stationTime = min + minPathSearch(g->station[idx].clientGraph, &minIndex);

	return stationTime + g->station[idx].client[minIndex].time;
}