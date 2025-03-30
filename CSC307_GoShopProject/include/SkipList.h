#ifndef GOSHOP_SKIPLIST_H
#define GOSHOP_SKIPLIST_H

#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>  // for rand()
#include <ctime>    // for srand()
#include <climits>  // for INT_MIN
using namespace std;
class SkipList {
private:
    // Node structure for the Skip List
    struct Node {
        int key;
        string value;
        vector<Node*> forward; // forward pointers for each level
        Node(int k, const string& val, int level)
            : key(k), value(val), forward(level + 1, nullptr) {}
    };

    // Maximum allowed level for nodes
    static const int MAX_LEVEL;
    // Probability for level promotion (typically 0.5)
    static const float P;

    // Current highest level of any element in the skip list
    int level;
    // Pointer to header (sentinel) node
    Node* head;

    // Generate a random level for a new node based on probability P
    int randomLevel() const;
    // Find a node by key (internal use, returns nullptr if not found)
    Node* findNode(int key) const;

public:
    // Constructor: initialize skip list
    SkipList();
    // Destructor: free all nodes
    ~SkipList();

    // Insert a key-value pair into the skip list (returns false if key exists)
    bool insert(int key, const string& value);

    // Search for a key, output value in outValue if found
    bool search(int key, string &outValue) const;

    // Update the value for an existing key
    bool update(int key, const string& newValue);

    // Remove a key-value pair from the skip list
    bool remove(int key);

    // Display all elements (key and value) in the skip list (level 0 list)
    void displayList() const;
};

#endif // GOSHOP_SKIPLIST_H
