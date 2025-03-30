#include "include/Graph.h"
#include "include/SkipList.h"
#include "include/DisjointSet.h"
#include "include/QuadTree.h"

#include <iostream>
#include <string>
#include <limits>
using namespace std;
// Main program: Menu-driven demonstration of all data structures
int main() {
    // Create instances of each data structure
    Graph graph;
    SkipList skiplist;
    DisjointSet ds;
    QuadTree quadtree;

    // Seed with sample data for demonstration purposes

    // Graph: sample store layout (vertices and edges with distances)
    graph.addVertex("Entrance");
    graph.addVertex("Dairy");
    graph.addVertex("Produce");
    graph.addVertex("Checkout");
    graph.addVertex("Pharmacy");
    graph.addEdge("Entrance", "Dairy", 10);
    graph.addEdge("Entrance", "Produce", 20);
    graph.addEdge("Dairy", "Produce", 5);
    graph.addEdge("Dairy", "Pharmacy", 10);
    graph.addEdge("Produce", "Checkout", 5);
    graph.addEdge("Pharmacy", "Checkout", 5);
    graph.addEdge("Entrance", "Pharmacy", 15);

    // SkipList: sample aisle data (aisle number -> description or items)
    skiplist.insert(3, "Dairy: Milk, Cheese, Yogurt");
    skiplist.insert(5, "Produce: Apple, Banana, Orange");
    skiplist.insert(7, "Bakery: Bread, Donuts");

    // DisjointSet: sample items grouped by category
    ds.makeSet("milk");
    ds.makeSet("cheese");
    ds.makeSet("yogurt");
    ds.makeSet("apple");
    ds.makeSet("banana");
    ds.makeSet("bread");
    ds.makeSet("eggs");
    ds.unionSets("milk", "cheese");   // union dairy products
    ds.unionSets("milk", "yogurt");   // union dairy products
    ds.unionSets("apple", "banana");  // union fruits

    // QuadTree: sample Walmart store locations (with coordinates)
    quadtree.insert(10, 20, "Walmart Supercenter A");
    quadtree.insert(-15, 5, "Walmart Supercenter B");
    quadtree.insert(5, -10, "Walmart Neighborhood C");

    cout << "GoShop Demonstration\n";
    cout << "------------------------------------\n";

    bool running = true;
    while (running) {
        cout << "\nMain Menu:\n";
        cout << "1. Navigate the store (Graph)\n";
        cout << "2. Skip List (Aisle Data)\n";
        cout << "3. Item Grouping (Skip List)\n";
        cout << "4. Nearest Store Location(Quadtree)\n";
        cout << "0. Exit\n";
        cout << "Enter choice: ";
        int choice;
        if (!(cin >> choice)) {
            // Handle non-integer input
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }

        if (choice == 0) {
            running = false;
        }
        else if (choice == 1) {
            // Graph operations submenu
            bool back = false;
            while (!back) {
                cout << "\nStore Navigation(Graph):\n";
                cout << "1. Add new store location\n";
                cout << "2. Connect two stores(path between locations)\n";
                cout << "3. Remove store\n";
                cout << "4. Remove path between stores\n";
                cout << "5. Update Distance between stores\n";
                cout << "6. View  full store map\n";
                cout << "7. Find Shortest shopping route\n";
                cout << "0. Back to Main Menu\n";
                cout << "Enter  your choice: ";
                int gChoice;
                if (!(cin >> gChoice)) {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    continue;
                }
                string label, label2;
                int weight;
                switch (gChoice) {
                    case 1:
                        cout << "Enter new store name: ";
                        getline(cin >> ws, label);
                        graph.addVertex(label);
                        break;
                    case 2:
                        cout << "Enter source store name: ";
                        getline(cin >> ws, label);
                        cout << "Enter destination store name: ";
                        getline(cin >> ws, label2);
                        cout << "Enter distance: ";
                        cin >> weight;
                        graph.addEdge(label, label2, weight);
                        break;
                    case 3:
                        cout << "Enter store name to remove: ";
                        getline(cin >> ws, label);
                        graph.removeVertex(label);
                        break;
                    case 4:
                        cout << "Enter source store name: ";
                        getline(cin >> ws, label);
                        cout << "Enter destination store name: ";
                        getline(cin >> ws, label2);
                        graph.removeEdge(label, label2);
                        break;
                    case 5:
                        cout << "Enter source store name: ";
                        getline(cin >> ws, label);
                        cout << "Enter destination store name: ";
                        getline(cin >> ws, label2);
                        cout << "Enter new distance: ";
                        cin >> weight;
                        graph.updateEdge(label, label2, weight);
                        break;
                    case 6:
                        graph.printGraph();
                        break;
                    case 7: {
                        cout << "Enter start location: ";
                        getline(cin >> ws, label);
                        cout << "Enter end location: ";
                        getline(cin >> ws, label2);
                        vector<string> path;
                        int distance;
                        if (graph.findShortestPath(label, label2, path, distance)) {
                            cout << "Shortest path from " << label << " to " << label2
                                 << " (distance " << distance << "): ";
                            for (size_t i = 0; i < path.size(); ++i) {
                                cout << path[i];
                                if (i < path.size() - 1) cout << " -> ";
                            }
                            cout << "\n";
                        } else {
                            cout << "No valid path found. Check the store name\n";
                        }
                        break;
                    }
                    case 0:
                        back = true;
                        break;
                    default:
                        cout << "!Invalid choice. Try again.\n";
                        break;
                }
            }
        }
        else if (choice == 2) {
            // Skip List operations submenu
            bool back = false;
            while (!back) {
                cout << "\nSkip List Menu (Aisle Data):\n";
                cout << "1. Add new Aisle with item (Insert Aisle Data)\n";
                cout << "2. Search Aisle Item\n";
                cout << "3. Update Aisle Data\n";
                cout << "4. Remove Aisle Data\n";
                cout << "5. Display All Aisle Data\n";
                cout << "0. Back to Main Menu\n";
                cout << "Enter choice: ";
                int sChoice;
                if (!(cin >> sChoice)) {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    continue;
                }
                int aisle;
                string info;
                string outInfo;
                switch (sChoice) {
                    case 1:
                        cout << "Enter aisle number: ";
                        cin >> aisle;
                        cout << "Enter aisle information (e.g., items or description): ";
                        getline(cin >> ws, info);
                        skiplist.insert(aisle, info);
                        break;
                    case 2:
                        cout << "Enter aisle number to search: ";
                        cin >> aisle;
                        if (skiplist.search(aisle, outInfo)) {
                            cout << "Found: Aisle " << aisle << " -> " << outInfo << "\n";
                        } else {
                            cout << "Aisle " << aisle << " not found.\n";
                        }
                        break;
                    case 3:
                        cout << "Enter aisle number to update: ";
                        cin >> aisle;
                        cout << "Enter new information: ";
                        getline(cin >> ws, info);
                        skiplist.update(aisle, info);
                        break;
                    case 4:
                        cout << "Enter aisle number to remove: ";
                        cin >> aisle;
                        skiplist.remove(aisle);
                        break;
                    case 5:
                        skiplist.displayList();
                        break;
                    case 0:
                        back = true;
                        break;
                    default:
                        cout << "Invalid choice. Try again.\n";
                        break;
                }
            }
        }
        else if (choice == 3) {
            // Disjoint Set operations submenu
            bool back = false;
            while (!back) {
                cout << "\nItem Grouping (Disjoint Set):\n";
                cout << "1. Add new Item (new set)\n";
                cout << "2. Group Items into Category\n";
                cout << "3. Check item category\n";
                cout << "4. Display All Item Groups\n";
                cout << "5. Remove Item\n";
                cout << "6. Update Item Name\n";
                cout << "0. Back to Main Menu\n";
                cout << "Enter choice: ";
                string x,y;
                int dChoice;
                if (!(cin >> dChoice)) {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    continue;
                }
                string item1, item2;
                string rep;
                switch (dChoice) {
                    case 1:
                        cout << "Enter item name: ";
                        getline(cin >> ws, item1);
                        ds.makeSet(item1);
                        break;
                    case 2:
                        cout << "Enter first item name: ";
                        getline(cin >> ws, item1);
                        cout << "Enter second item name: ";
                        getline(cin >> ws, item2);
                        ds.unionSets(item1, item2);
                        break;
                    case 3:
                        cout << "Enter item name: ";
                        getline(cin >> ws, item1);
                        if (ds.find(item1, rep)) {
                            cout << "Representative of '" << item1 << "' is '" << rep << "'.\n";
                        }
                        break;
                    case 4:
                        ds.printSets();
                        break;
                    case 5:
                        cout << "Enter item name to remove: ";
                        cin >> x;
                        ds.removeItem(x);
                        break;
                    case 6:
                        cout << "Enter existing item name: ";
                        cin >> x;
                        cout << "Enter new item name: ";
                        cin >> y;
                        ds.updateItem(x, y);
                        break;
                    case 0:
                        back = true;
                        break;
                    default:
                        cout << "Invalid choice. Try again.\n";
                        break;
                }
            }
        }
        else if (choice == 4) {
            // QuadTree operations submenu
            bool back = false;
            while (!back) {
                cout << "\nQuadTree Menu (Nearest Store Location):\n";
                cout << "1.Add new Store\n";
                cout << "2. Remove Store\n";
                cout << "3. Locate Nearest Store\n";
                cout << "4. Display All available store\n";
                cout << "0. Back to Main Menu\n";
                cout << "Enter choice: ";
                int qChoice;
                if (!(cin >> qChoice)) {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    continue;
                }
                string storeName;
                double x, y;
                switch (qChoice) {
                    case 1:
                        cout << "Enter store name: ";
                        getline(cin >> ws, storeName);
                        cout << "Enter X coordinate: ";
                        cin >> x;
                        cout << "Enter Y coordinate: ";
                        cin >> y;
                        quadtree.insert(x, y, storeName);
                        break;
                    case 2:
                        cout << "Enter store name to remove: ";
                        getline(cin >> ws, storeName);
                        quadtree.remove(storeName);
                        break;
                    case 3: {
                        cout << "Enter your location X: ";
                        cin >> x;
                        cout << "Enter your location Y: ";
                        cin >> y;
                        string nearestName;
                        double nearestX, nearestY, dist;
                        if (quadtree.findNearest(x, y, nearestName, nearestX, nearestY, dist)) {
                            cout << "Nearest store is '" << nearestName << "' at ("
                                 << nearestX << ", " << nearestY << "), distance " << dist << ".\n";
                        } else {
                            cout << "No store locations available.\n";
                        }
                        break;
                    }
                    case 4:
                        quadtree.printLocations();
                        break;
                    case 0:
                        back = true;
                        break;
                    default:
                        cout << "Invalid choice. Try again.\n";
                        break;
                }
            }
        }
        else {
            cout << "Invalid choice. Try again.\n";
        }
    }

    cout << "Exiting GoShop. Have a nice day!\n";
    return 0;
}
