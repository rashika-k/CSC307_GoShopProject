#include "../include/DisjointSet.h"
#include <vector>
using namespace std;
// Disjoint Set (Union-Find) Implementation with path compression and union by rank

string DisjointSet::findSet(const string& x) {
    if (parent.find(x) == parent.end()) return x;
    if (parent[x] != x) {
        parent[x] = findSet(parent[x]);
    }
    return parent[x];
}

bool DisjointSet::makeSet(const string& x) {
    if (parent.find(x) != parent.end()) {
        cerr << "DisjointSet: Element '" << x << "' already exists.\n";
        return false;
    }
    parent[x] = x;
    rank[x] = 0;
    active[x] = true;
    return true;
}

bool DisjointSet::find(const string& x, string& outRepresentative) {
    if (parent.find(x) == parent.end() || !active[x]) {
        cerr << "DisjointSet: Element '" << x << "' not found or removed.\n";
        return false;
    }
    outRepresentative = findSet(x);
    return true;
}

bool DisjointSet::unionSets(const string& x, const string& y) {
    if (!active[x] || !active[y]) {
        cerr << "DisjointSet: One or both elements are removed.\n";
        return false;
    }
    string rootX = findSet(x);
    string rootY = findSet(y);
    if (rootX == rootY) return false;

    if (rank[rootX] < rank[rootY]) {
        parent[rootX] = rootY;
    } else if (rank[rootX] > rank[rootY]) {
        parent[rootY] = rootX;
    } else {
        parent[rootY] = rootX;
        rank[rootX]++;
    }
    return true;
}

bool DisjointSet::removeItem(const string& x) {
    if (active.find(x) == active.end() || !active[x]) {
        cerr << "DisjointSet: Item not found or already removed.\n";
        return false;
    }
    active[x] = false;
    return true;
}

bool DisjointSet::updateItem(const string& oldName, const string& newName) {
    if (!active[oldName]) {
        cerr << "DisjointSet: Cannot update non-existing item.\n";
        return false;
    }
    string rep;
    if (!find(oldName, rep)) return false;

    makeSet(newName);
    unionSets(newName, rep);
    removeItem(oldName);
    return true;
}

void DisjointSet::printSets() {
    map<string, vector<string>> sets;
    for (auto& kv : parent) {
        if (!active[kv.first]) continue;
        string root = findSet(kv.first);
        sets[root].push_back(kv.first);
    }
    cout << "Disjoint Set contents:\n";
    for (auto& kv : sets) {
        cout << "  Set representative: " << kv.first << " -> { ";
        for (size_t i = 0; i < kv.second.size(); ++i) {
            cout << kv.second[i];
            if (i < kv.second.size() - 1) cout << ", ";
        }
        cout << " }\n";
    }
}
