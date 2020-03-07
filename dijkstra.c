#include "dijkstra.h"

int minDistance(int *dist, int *sps, int vertex)
{
	int i;
	int min = INF;
	int minIndex;

	for (i = 0; i < vertex; i++)
		if (sps[i] == 0 && dist[i] <= min)
			{
				min = dist[i];
				minIndex = i;
			}

	return minIndex;
}

void printStations(int *pred, int v, stationGraph *g, FILE *file)
{
	int i, n;
	int *path = (int*)malloc(g->V * sizeof(int));

	i = 0;

	while (v != -1)
	{
		path[i++] = v;
		v = pred[v];
	}

	n = i;

	for (i = n - 1; i >= 0; i--)
	{
		fprintf(file, "%s ", g->station[path[i]].name);
	}

	fprintf(file, "\n");

	free(path);
}

void printClients(int *pred, int v, stationGraph *g, int idx, FILE *file)
{
	int i, n;
	int *path = (int*)malloc(g->V * sizeof(int));

	i = 0;

	while (v != -1)
	{
		path[i++] = v;
		v = pred[v];
	}

	n = i;

	for (i = n - 1; i >= 0; i--)
	{
		fprintf(file, "%s ", g->station[idx].client[path[i]].name);
	}

	fprintf(file, "\n");

	free(path);
}

void stationDijkstra(stationGraph *g, int source, int dest, FILE *file)
{
	int i, j, u;
	int dist[g->V];
	int sps[g->V];
	int pred[g->V];

	for (i = 0; i < g->V; i++)
	{
		dist[i] = INF;
		sps[i] = 0;
		pred[i] = -1;
	}

	dist[source] = 0;

	for (j = 0; j < g->V - 1; j++)
	{
		u = minDistance(dist, sps, g->V);

		sps[u] = 1;

		for (i = 0; i < g->V; i++)
			if (sps[i] == 0 && g->Adj[u][i] != 0 && dist[u] != INF && dist[u] + g->Adj[u][i] < dist[i])
				{
					dist[i] = dist[u] + g->Adj[u][i];
					pred[i] = u;
				}
	}

	printStations(pred, dest, g, file);
}

void clientsDijkstra(stationGraph *g, int source, int dest, int idx, FILE *file)
{
	int i, j, u;
	int dist[g->station[idx].nrClients];
	int sps[g->station[idx].nrClients];
	int pred[g->station[idx].nrClients];

	for (i = 0; i < g->station[idx].nrClients; i++)
	{
		dist[i] = INF;
		sps[i] = 0;
		pred[i] = -1;
	}

	dist[source] = 0;

	for (j = 0; j < g->station[idx].nrClients - 1; j++)
	{
		u = minDistance(dist, sps, g->station[idx].nrClients);

		sps[u] = 1;

		for (i = 0; i < g->station[idx].nrClients; i++)
			if (sps[i] == 0 && g->station[idx].clientGraph->Adj[u][i] != 0 && dist[u] != INF && dist[u] + g->station[idx].clientGraph->Adj[u][i] < dist[i])
				{
					dist[i] = dist[u] + g->station[idx].clientGraph->Adj[u][i];
					pred[i] = u;
				}
	}

	printClients(pred, dest, g, idx, file);
}