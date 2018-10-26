/*
 * Graph.cpp
 *
 * Grafo com funcao para verificacao de ciclo para teste de conflito de seriabilidade
 *
 * Fonte: https://www.geeksforgeeks.org/detect-cycle-in-a-graph/
 *
 */

#include "Graph.h"
#include<iostream>
#include <list>
#include <limits.h>
using namespace std;

Graph::Graph(int V)
{
    this->V = V;
    adj = new list<int>[V];
}

//void Graph::setVertices(int V)
//{
//    this->V = V;
//    adj = new list<int>[V];
//}

void Graph::addEdge(int v, int w)
{
    adj[v].push_back(w); // Add w to v’s list.
}

// This function is a variation of DFSUytil() in https://www.geeksforgeeks.org/archives/18212
bool Graph::isCyclicUtil(int v, bool visited[], bool *recStack)
{
    if(visited[v] == false)
    {
        // Mark the current node as visited and part of recursion stack
        visited[v] = true;
        recStack[v] = true;

        // Recur for all the vertices adjacent to this vertex
        list<int>::iterator i;
        for(i = adj[v].begin(); i != adj[v].end(); ++i)
        {
            if ( !visited[*i] && isCyclicUtil(*i, visited, recStack) )
                return true;
            else if (recStack[*i])
                return true;
        }

    }
    recStack[v] = false;  // remove the vertex from recursion stack
    return false;
}

// Returns true if the graph contains a cycle, else false.
// This function is a variation of DFS() in https://www.geeksforgeeks.org/archives/18212
bool Graph::isCyclic()
{
    // Mark all the vertices as not visited and not part of recursion
    // stack
    bool *visited = new bool[V];
    bool *recStack = new bool[V];
    for(int i = 0; i < V; i++)
    {
        visited[i] = false;
        recStack[i] = false;
    }

    // Call the recursive helper function to detect cycle in different
    // DFS trees
    for(int i = 0; i < V; i++)
        if (isCyclicUtil(i, visited, recStack))
            return true;

    return false;
}

//int main()
//{
//    // Create a graph given in the above diagram
////	GraphDirected g(3);
////    g.addEdge(0, 1);
////    g.addEdge(0, 2);
////    g.addEdge(1, 2);
////    g.addEdge(2, 0);
////    g.addEdge(2, 3);
////    g.addEdge(3, 3);
//
//	GraphDirected g(5);
//	    g.addEdge(1, 0);
//	    g.addEdge(0, 2);
//	    g.addEdge(2, 0);
//	    g.addEdge(0, 3);
//	    g.addEdge(3, 4);
//
//    if(g.isCyclic())
//        cout << "Graph contains cycle";
//    else
//        cout << "Graph doesn't contain cycle";
//    return 0;
//}
