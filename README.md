# The Ideal Route of the Courier
May 2019
## Solution Description
For storing data, a graph of subway stations is used in which a vector of
stations is retained. Each vertex of the station graph represents the index in
the station vector. In the array are saved the attributes of each station, a
graph of clients and a vector of clients. Each vertex of the client graph also
represents the index in the clients vector. In the array the attributes of
every connection between the stations, respectively the clients of a station.
The data is read, marking in the adjacency matrix each edge between the two
vertices. 
For the "connection" it is checked in the adjacency matrix of the corresponding
client graph if there is a connection between the two vertices.
For the "link" all connections with the respective station are checked in the
adjacency matrix of the station graph.
For "route_block", the element in the matrix corresponding to the two stations
is marked with infinity.
For "street_block", the element in the matrix corresponding to the two clients
is marked with infinity. 
For "delete_route" the element in the matrix corresponding to the two stations
is marked with zero. 
For "delete_street" the element in the matrix corresponding to the two clients
is marked with zero.
For "add_route" and "add_street" the elements with the respective index are
marked in the adjancency matrix.
For "station_order" it is checked whether the amount paid at each station is
greater than or equal to the value sent.
For "station_path" and "client_path" Dijkstra's algorithm is used.
For "station_time" always start with the client who has the minimum time to the
subway station and succesively adds the minimum time from the clients with whom
he has a link and they have not been visited.

## Headers
* utility.h
* dijkstra.h
* tasks.h

## Explanation of functions
### utility.h
* `int indexOfClient(char *name, Client *client, int nr)`
return the client index from the array of clients

* `int indexOfStation(char *name, Station *station, int nr)`
return the station indes from the array of stations
* `stationGraph *allocStationGraph(int vertex)`
allocate memory for the station graph

* `clientGraph *allocClientGraph(int vertex)`
allocate memory for the client graph

* `stationGraph* readGraph(FILE *file)`
read all the data from the input file and add it to the vectors and the
corresponding graphs

* `void readTasks(FILE *fin, FILE *fout, stationGraph *g)`
read all the requirements in the input file and calls the necessary
functions to solve each one

* `void freeGraph(stationGraph *g)`
graph memory release function

### dijsktra.h
* `int minDistance(int *dist, int *sps, int vertex)`
returns the minimum distance index from the distance array

* `void printStations(int *pred, int v, stationGraph *g, FILE *file)`
Display the stations in the order in which they are traversed

* `void printClients(int *pred, int v, stationGraph *g, int idx, FILE *file)`
Display the clients in the order in which they are visited

* `void stationDijkstra(stationGraph *g, int source, int dest, FILE *file)`
Implement Dijkstra's algorithm for the station graph

* `void clientsDijkstra(stationGraph *g, int source, int dest, int idx, FILE *file)`
Implement Dijkstra's algorithm for the client graph

### tasks.h
Each function implements one of the tasks that the software can receive through
commands.

