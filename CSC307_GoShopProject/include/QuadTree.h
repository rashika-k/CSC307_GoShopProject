#ifndef GOSHOP_QUADTREE_H
#define GOSHOP_QUADTREE_H

#include <iostream>
#include <string>
#include <cmath>
#include <limits>
using namespace std;
class QuadTree {
private:
    // QuadTree node structure representing a region and possibly a store location
    struct QuadNode {
        double x, y;           // coordinates of a store (if this is a leaf with a store)
        string name;           // name of the store at this node (empty if none)
        double minX, minY;     // boundaries of this node's region (min corner)
        double maxX, maxY;     // boundaries of this node's region (max corner)
        QuadNode* NW;
        QuadNode* NE;
        QuadNode* SW;
        QuadNode* SE;
        QuadNode(double minx, double miny, double maxx, double maxy)
            : x(0), y(0), name(""), minX(minx), minY(miny), maxX(maxx), maxY(maxy),
              NW(nullptr), NE(nullptr), SW(nullptr), SE(nullptr) {}
        bool isLeaf() const {
            // A node is a leaf if it has no children.
            return (NW == nullptr && NE == nullptr && SW == nullptr && SE == nullptr);
        }
    };

    QuadNode* root;
    int count;  // number of stores (points) in the quadtree

    // Recursive helper to insert a point into the subtree rooted at 'node'
    bool insertNode(QuadNode* node, double x, double y, const string& name);
    // Recursive helper to find the nearest neighbor in subtree
    void nearestNode(QuadNode* node, double targetX, double targetY,
                     double& bestDist, QuadNode*& bestNode) const;
    // Recursive helper to find a node by store name
    QuadNode* findByName(QuadNode* node, const string& name) const;
    // Recursive helper to delete all nodes (used in destructor)
    void destroyNode(QuadNode* node);

public:
    // Constructor: initialize QuadTree with given boundary (default boundary if not specified)
    QuadTree(double minx = -100.0, double miny = -100.0, double maxx = 100.0, double maxy = 100.0);
    // Destructor: free all nodes
    ~QuadTree();

    // Insert a store location with coordinates (x, y) and store name
    bool insert(double x, double y, const string& name);

    // Remove a store by name (returns true if removed)
    bool remove(const string& name);
    // Remove a store by exact coordinates (returns true if removed)
    bool remove(double x, double y);

    // Find the nearest store to the given (x, y) location.
    // Returns true if a store is found, and outputs the nearest store's name, coordinates, and distance.
    bool findNearest(double x, double y, string& nearestName, double& nearestX, double& nearestY, double& distance) const;

    // Print all stores and their coordinates in the QuadTree
    void printLocations() const;
};

#endif // GOSHOP_QUADTREE_H
