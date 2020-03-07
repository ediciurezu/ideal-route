#include "utility.h"
#include "tasks.h"

int indexOfClient(char *name, Client *client, int nr)
{
	int i;

	for (i = 0; i < nr; i++)
		if (!strcmp(name, client[i].name)) return i;

	return -1;

}

int indexOfStation(char *name, Station *station, int nr)
{
	int i;

	for (i = 0; i < nr; i++)
		if (!strcmp(name, station[i].name)) return i;

	return -1;

}

stationGraph *allocStationGraph(int vertex)
{
	int i;

	stationGraph *g = (stationGraph*)malloc(sizeof(stationGraph));
	if (g == NULL)
	{
		fprintf(stderr, "Memory allocation error!\n");
		exit(1);
	}

	g->V = vertex;
	g->Adj = (int**)malloc(g->V * sizeof(int*));

	for (i = 0; i < g->V; i++)
		g->Adj[i] = (int*)calloc(g->V, sizeof(int));

	return g;
}

clientGraph *allocClientGraph(int vertex)
{
	int i;

	clientGraph *g = (clientGraph*)malloc(sizeof(clientGraph));
	if (g == NULL)
	{
		fprintf(stderr, "Memory allocation error!\n");
		exit(1);
	}

	g->V = vertex;
	g->Adj = (int**)malloc(g->V * sizeof(int*));

	for (i = 0; i < g->V; i++)
		g->Adj[i] = (int*)calloc(g->V, sizeof(int));

	return g;
}

stationGraph* readGraph(FILE *file)
{
	stationGraph *g;
	int vertex;

	fscanf(file, "%d", &vertex);
	g = allocStationGraph(vertex);

	g->station = (Station*)malloc(g->V * sizeof(Station));

	int i, j;

	for (i = 0; i < g->V; i++)
	{
		g->station[i].name = (char*)malloc(50);
		fscanf(file, "%s", g->station[i].name);
		g->station[i].name = (char*)realloc(g->station[i].name, strlen(g->station[i].name) + 1);

		fscanf(file, "%d", &g->station[i].nrClients);

		g->station[i].clientGraph = allocClientGraph(g->station[i].nrClients);

		g->station[i].client = (Client*)malloc(g->station[i].clientGraph->V * sizeof(Client));

		for (j = 0; j < g->station[i].clientGraph->V; j++)
		{
			g->station[i].client[j].name = (char*)malloc(50);
			fscanf(file, "%s", g->station[i].client[j].name);
			g->station[i].client[j].name = (char*)realloc(g->station[i].client[j].name, strlen(g->station[i].client[j].name) + 1);

			fscanf(file, "%d", &g->station[i].client[j].time);

			fscanf(file, "%d", &g->station[i].client[j].sum);
		}

		fscanf(file, "%d", &g->station[i].clientGraph->E);

		char *client1 = (char*)malloc(50);
		char *client2 = (char*)malloc(50);
		int cost;

		for (j = 0; j < g->station[i].clientGraph->E; j++)
		{
			fscanf(file, "%s%s%d", client1, client2, &cost);

			int x = indexOfClient(client1, g->station[i].client, g->station[i].nrClients);
			int y = indexOfClient(client2, g->station[i].client, g->station[i].nrClients);
			g->station[i].clientGraph->Adj[x][y] = g->station[i].clientGraph->Adj[y][x] = cost;
		}

		free(client1);
		free(client2);
	}

	fscanf(file, "%d", &g->E);

	char *station1 = (char*)malloc(50);
	char *station2 = (char*)malloc(50);
	int cost;

	for (i = 0; i < g->E; i++)
	{
		fscanf(file, "%s%s%d", station1, station2, &cost);
		g->Adj[indexOfStation(station1, g->station, g->V)][indexOfStation(station2, g->station, g->V)] = cost;
		g->Adj[indexOfStation(station2, g->station, g->V)][indexOfStation(station1, g->station, g->V)] = cost;
	}

	free(station1);
	free(station2);

	return g;
}

void readTasks(FILE *fin, FILE *fout, stationGraph *g)
{
	int i, nrTasks;
	char *buff = (char*)malloc(30);

	fscanf(fin, "%d", &nrTasks);

	for (i = 0; i < nrTasks; i++)
	{
		fscanf(fin, "%s", buff);

		if (!strcmp(buff, "conexiune"))
		{
			char *client1 = malloc(30);
			char *client2 = malloc(30);

			fscanf(fin, "%s%s", client1, client2);

			if (connection(client1, client2, g))
				fprintf(fout, "OK\n");
			else fprintf(fout, "NO\n");

			free(client1);
			free(client2);
		}

		if (!strcmp(buff, "legatura"))
		{
			char *station = malloc(30);

			fscanf(fin, "%s", station);

			link(station, g, fout);

			free(station);
		}

		if (!strcmp(buff, "blocaj_tunel"))
		{
			char *station1 = malloc(30);
			char *station2 = malloc(30);

			fscanf(fin, "%s%s", station1, station2);

			g = routeBlock(station1, station2, g);

			free(station1);
			free(station2);
		}

		if (!strcmp(buff, "blocaj_strada"))
		{
			char *client1 = malloc(30);
			char *client2 = malloc(30);

			fscanf(fin, "%s%s", client1, client2);

			g = streetBlock(client1, client2, g);

			free(client1);
			free(client2);
		}

		if (!strcmp(buff, "adauga_ruta"))
		{
			char *station1 = malloc(30);
			char *station2 = malloc(30);
			int cost;

			fscanf(fin, "%s%s%d", station1, station2, &cost);

			g = addRoute(station1, station2, cost, g);

			free(station1);
			free(station2);
		}

		if (!strcmp(buff, "adauga_strada"))
		{
			char *client1 = malloc(30);
			char *client2 = malloc(30);
			int cost;

			fscanf(fin, "%s%s%d", client1, client2, &cost);

			g = addStreet(client1, client2, cost, g);

			free(client1);
			free(client2);
		}

		if (!strcmp(buff, "sterge_strada"))
		{
			char *client1 = malloc(30);
			char *client2 = malloc(30);

			fscanf(fin, "%s%s", client1, client2);

			g = deleteStreet(client1, client2, g);

			free(client1);
			free(client2);
		}
		
		if (!strcmp(buff, "sterge_ruta"))
		{
			char *station1 = malloc(30);
			char *station2 = malloc(30);

			fscanf(fin, "%s%s", station1, station2);

			g = deleteRoute(station1, station2, g);

			free(station1);
			free(station2);
		}

		if (!strcmp(buff, "comanda_statie"))
		{
			int value;

			fscanf(fin, "%d", &value);

			stationOrder(g, value, fout);
		}

		if (!strcmp(buff, "drum_metrou"))
		{
			char *station1 = malloc(30);
			char *station2 = malloc(30);

			fscanf(fin, "%s%s", station1, station2);

			stationPath(station1, station2, g, fout);

			free(station1);
			free(station2);
		}

		if (!strcmp(buff, "drum_strada"))
		{
			
			char *client1 = malloc(30);
			char *client2 = malloc(30);

			fscanf(fin, "%s%s", client1, client2);

			clientPath(client1, client2, g, fout);

			free(client1);
			free(client2);
		}

		if (!strcmp(buff, "timp_statie"))
		{
			char *station = malloc(30);

			fscanf(fin, "%s", station);

			fprintf(fout, "%d\n", stationTime(station, g));

			free(station);
		}
	}

	free(buff);
}

void freeMatrix(int **M, int n)
{
	int i;

	for ( i = 0; i < n; i++ )
		free(M[i]);

	free(M);
}

void freeGraph(stationGraph *g)
{
	int i, j;

	for (i = 0; i < g->V; i++)
	{
		free(g->station[i].name);

		for (j = 0; j < g->station[i].nrClients; j++)
			free(g->station[i].client[j].name);

		free(g->station[i].client);
		freeMatrix(g->station[i].clientGraph->Adj, g->station[i].nrClients);
		free(g->station[i].clientGraph);
	}

	free(g->station);
	freeMatrix(g->Adj, g->V);
	free(g);
}