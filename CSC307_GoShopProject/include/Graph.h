#ifndef GOSHOP_GRAPH_H
#define GOSHOP_GRAPH_H

#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <limits>
#include <queue>
#include <utility>
using namespace std;
class Graph {
private:
    // Adjacency list representation: map from vertex label to vector of (neighbor, weight) pairs
    map<string, vector<pair<string, int>>> adjList;

public:
    // Add a vertex (location) to the graph.
    bool addVertex(const string& label);

    // Remove a vertex and all associated edges from the graph.
    bool removeVertex(const string& label);

    // Add an undirected edge (path) between src and dest with given weight (distance).
    bool addEdge(const string& src, const string& dest, int weight);

    // Remove an undirected edge (path) between src and dest.
    bool removeEdge(const string& src, const string& dest);

    // Update the weight (distance) of an existing edge between src and dest.
    bool updateEdge(const string& src, const string& dest, int newWeight);

    // Print the graph (list all vertices and their adjacent vertices with weights).
    void printGraph() const;

    // Find shortest path from start to end using Dijkstra’s algorithm.
    // Returns true if a path is found, and outputs the path and total distance.
    bool findShortestPath(const string& start, const string& end,
                          vector<string>& path, int& distance) const;
};

#endif // GOSHOP_GRAPH_H
