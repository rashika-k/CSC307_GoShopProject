#ifndef GOSHOP_DISJOINTSET_H
#define GOSHOP_DISJOINTSET_H

#include <iostream>
#include <string>
#include <map>
#include <vector>
using namespace std;

class DisjointSet {
private:
    map<string, string> parent;
    map<string, int> rank;
    map<string, bool> active; // new: track if an item is removed

    string findSet(const string& x);

public:
    bool makeSet(const string& x);
    bool find(const string& x, string& outRepresentative);
    bool unionSets(const string& x, const string& y);
    bool removeItem(const string& x);             // new
    bool updateItem(const string& oldName, const string& newName); // new
    void printSets();
};

#endif // GOSHOP_DISJOINTSET_H
