#include "Graph.h"
#include <algorithm>  // for remove_if
#include <queue>
#include <limits>
using namespace std;

bool Graph::addVertex(const string& label) {
    if (adjList.find(label) != adjList.end()) {
        cerr << "Vertex '" << label << "' already exists.\n";
        return false;
    }
    adjList[label] = vector<pair<string, int>>();
    return true;
}

bool Graph::removeVertex(const string& label) {
    auto it = adjList.find(label);
    if (it == adjList.end()) {
        cerr << "Vertex '" << label << "' not found.\n";
        return false;
    }
    for (auto& kv : adjList) {
        if (kv.first == label) continue;
        auto& neighbors = kv.second;
        neighbors.erase(
            remove_if(neighbors.begin(), neighbors.end(),
                      [&](const pair<string, int>& p){ return p.first == label; }),
            neighbors.end()
        );
    }
    adjList.erase(label);
    return true;
}

bool Graph::addEdge(const string& src, const string& dest, int weight) {
    if (weight < 0) {
        cerr << "Edge weight cannot be negative.\n";
        return false;
    }
    if (adjList.find(src) == adjList.end() || adjList.find(dest) == adjList.end()) {
        cerr << "One or both vertices not found.\n";
        return false;
    }
    adjList[src].push_back({dest, weight});
    adjList[dest].push_back({src, weight});
    return true;
}

bool Graph::removeEdge(const string& src, const string& dest) {
    if (adjList.find(src) == adjList.end() || adjList.find(dest) == adjList.end()) {
        cerr << "One or both vertices not found.\n";
        return false;
    }
    bool removed = false;

    auto& srcNeighbors = adjList[src];
    for (auto it = srcNeighbors.begin(); it != srcNeighbors.end(); ++it) {
        if (it->first == dest) {
            srcNeighbors.erase(it);
            removed = true;
            break;
        }
    }

    auto& destNeighbors = adjList[dest];
    for (auto it = destNeighbors.begin(); it != destNeighbors.end(); ++it) {
        if (it->first == src) {
            destNeighbors.erase(it);
            removed = true;
            break;
        }
    }

    if (!removed) {
        cerr << "Edge '" << src << " - " << dest << "' not found.\n";
    }

    return removed;
}

bool Graph::updateEdge(const string& src, const string& dest, int newWeight) {
    if (adjList.find(src) == adjList.end() || adjList.find(dest) == adjList.end()) {
        cerr << "One or both vertices not found.\n";
        return false;
    }

    bool updated = false;

    for (auto& p : adjList[src]) {
        if (p.first == dest) {
            p.second = newWeight;
            updated = true;
            break;
        }
    }

    for (auto& p : adjList[dest]) {
        if (p.first == src) {
            p.second = newWeight;
            updated = true;
            break;
        }
    }

    if (!updated) {
        cerr << "Edge '" << src << " - " << dest << "' not found.\n";
    }

    return updated;
}

void Graph::printGraph() const {
    cout << "Graph vertices and their edges:\n";
    for (const auto& kv : adjList) {
        const string& vertex = kv.first;
        const auto& neighbors = kv.second;
        cout << "  " << vertex << ":";
        for (const auto& edge : neighbors) {
            cout << " -> [" << edge.first << ", w=" << edge.second << "]";
        }
        cout << "\n";
    }
}

bool Graph::findShortestPath(const string& start, const string& end,
                             vector<string>& path, int& distance) const {
    if (adjList.find(start) == adjList.end() || adjList.find(end) == adjList.end()) {
        cerr << "Start or end vertex not found.\n";
        return false;
    }

    map<string, int> dist;
    map<string, string> prev;

    for (const auto& kv : adjList) {
        dist[kv.first] = numeric_limits<int>::max();
    }
    dist[start] = 0;

    auto comp = [](const pair<int, string>& a, const pair<int, string>& b) {
        return a.first > b.first;
    };

    priority_queue<pair<int, string>, vector<pair<int, string>>, decltype(comp)> pq(comp);
    pq.push({0, start});

    map<string, bool> visited;

    while (!pq.empty()) {
        pair<int, string> top = pq.top();
        pq.pop();

        int distU = top.first;
        string u = top.second;

        if (visited[u]) continue;
        visited[u] = true;

        if (u == end) break;

        const auto& neighbors = adjList.at(u);
        for (size_t i = 0; i < neighbors.size(); ++i) {
            string v = neighbors[i].first;
            int w = neighbors[i].second;

            if (visited[v]) continue;
            if (distU + w < dist[v]) {
                dist[v] = distU + w;
                prev[v] = u;
                pq.push({dist[v], v});
            }
        }
    }

    if (dist[end] == numeric_limits<int>::max()) {
        return false;
    }

    path.clear();
    string cur = end;
    while (cur != start) {
        path.push_back(cur);
        cur = prev[cur];
    }
    path.push_back(start);
    reverse(path.begin(), path.end());

    distance = dist[end];
    return true;
}
