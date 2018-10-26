/*
 * Graph.h
 *
 * Grafo com funcao para verificacao de ciclo para teste de conflito de seriabilidade
 *
 * Fonte: https://www.geeksforgeeks.org/detect-cycle-in-a-graph/
 *
 */

#ifndef GRAPH_H_
#define GRAPH_H_
#include<iostream>
#include <list>
#include <limits.h>
using namespace std;

class Graph {
public:
	Graph(int V);   // Constructor
//	void setVertices(int V);
	void addEdge(int v, int w);   // to add an edge to graph
	bool isCyclic();    // returns true if there is a cycle in this graph
private:
	int V;    // No. of vertices
	list<int> *adj;    // Pointer to an array containing adjacency lists
	bool isCyclicUtil(int v, bool visited[], bool *rs);  // used by isCyclic()
};

#endif /* GRAPH_H_ */
