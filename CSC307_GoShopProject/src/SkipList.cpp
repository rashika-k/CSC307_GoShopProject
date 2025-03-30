#include "../include/SkipList.h"
using namespace std;
// SkipList Implementation: Random level skip list for quick search/insert

const int SkipList::MAX_LEVEL = 6;
const float SkipList::P = 0.5f;

SkipList::SkipList() : level(0) {
    // Initialize random seed for random level generation
    srand(static_cast<unsigned>(time(nullptr)));
    // Create header node with key INT_MIN as a sentinel value
    head = new Node(INT_MIN, "", MAX_LEVEL);
    // Initialize forward pointers of header to nullptr
    for (int i = 0; i <= MAX_LEVEL; ++i) {
        head->forward[i] = nullptr;
    }
}

SkipList::~SkipList() {
    // Delete all nodes starting from the header
    Node* current = head->forward[0];
    while (current != nullptr) {
        Node* next = current->forward[0];
        delete current;
        current = next;
    }
    // Delete the header node
    delete head;
}

int SkipList::randomLevel() const {
    int lvl = 0;
    // Increase level with probability P for each level
    while (((double) rand() / RAND_MAX) < P && lvl < MAX_LEVEL) {
        lvl++;
    }
    return lvl;
}

SkipList::Node* SkipList::findNode(int key) const {
    Node* current = head;
    // Start from highest level and move down levels
    for (int i = level; i >= 0; --i) {
        while (current->forward[i] && current->forward[i]->key < key) {
            current = current->forward[i];
        }
    }
    // Move to the next node at level 0 (possibly the target)
    current = current->forward[0];
    if (current && current->key == key) {
        return current;
    }
    return nullptr;
}

// Search for key in SkipList
bool SkipList::search(int key, string &outValue) const {
    Node* node = findNode(key);
    if (node) {
        outValue = node->value;
        return true;
    }
    return false;
}

// Insert key and value into SkipList
bool SkipList::insert(int key, const string& value) {
    // Track nodes that need to update their forward pointers (update path)
    vector<Node*> update(MAX_LEVEL + 1);
    Node* current = head;
    // Find position for insertion on each level
    for (int i = level; i >= 0; --i) {
        while (current->forward[i] && current->forward[i]->key < key) {
            current = current->forward[i];
        }
        update[i] = current;
    }
    current = current->forward[0];
    // If key already exists, do not insert (or optionally update)
    if (current && current->key == key) {
        cerr << "SkipList: Key " << key << " already exists.\n";
        return false;
    }
    // Generate random level for the new node
    int newLevel = randomLevel();
    if (newLevel > level) {
        // If new node's level is higher, update header forward pointers for levels in between
        for (int i = level + 1; i <= newLevel; ++i) {
            update[i] = head;
        }
        level = newLevel;
    }
    // Create the new node
    Node* newNode = new Node(key, value, newLevel);
    // Insert the new node by adjusting forward pointers at each level
    for (int i = 0; i <= newLevel; ++i) {
        newNode->forward[i] = update[i]->forward[i];
        update[i]->forward[i] = newNode;
    }
    return true;
}

// Update value for existing key in SkipList
bool SkipList::update(int key, const string& newValue) {
    Node* node = findNode(key);
    if (node) {
        node->value = newValue;
        return true;
    }
    cerr << "SkipList: Key " << key << " not found for update.\n";
    return false;
}

// Remove key from SkipList
bool SkipList::remove(int key) {
    vector<Node*> update(MAX_LEVEL + 1);
    Node* current = head;
    // Find the node and keep track of nodes at each level that point to it
    for (int i = level; i >= 0; --i) {
        while (current->forward[i] && current->forward[i]->key < key) {
            current = current->forward[i];
        }
        update[i] = current;
    }
    current = current->forward[0];
    // If target key is not present
    if (!current || current->key != key) {
        cerr << "SkipList: Key " << key << " not found for deletion.\n";
        return false;
    }
    // Adjust pointers at each level to bypass the node being removed
    for (int i = 0; i <= level; ++i) {
        if (update[i]->forward[i] != current) break;
        update[i]->forward[i] = current->forward[i];
    }
    // Free the node memory
    delete current;
    // Reduce level if the highest level is now empty
    while (level > 0 && head->forward[level] == nullptr) {
        level--;
    }
    return true;
}

// Display all key-value pairs in SkipList
void SkipList::displayList() const {
    cout << "SkipList contents (level 0):\n";
    Node* node = head->forward[0];
    while (node != nullptr) {
        cout << "  Aisle " << node->key << " -> " << node->value << "\n";
        node = node->forward[0];
    }
}
