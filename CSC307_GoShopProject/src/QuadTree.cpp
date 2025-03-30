#include "../include/QuadTree.h"
#include <functional>
using namespace std;
// QuadTree Implementation for nearest neighbor search

QuadTree::QuadTree(double minx, double miny, double maxx, double maxy) {
    // Create root node covering the entire region
    root = new QuadNode(minx, miny, maxx, maxy);
    count = 0;
}

QuadTree::~QuadTree() {
    // Recursively delete all nodes to free memory
    destroyNode(root);
}

void QuadTree::destroyNode(QuadNode* node) {
    if (node == nullptr) return;
    // Post-order traversal deletion of children
    destroyNode(node->NW);
    destroyNode(node->NE);
    destroyNode(node->SW);
    destroyNode(node->SE);
    delete node;
}

// Insert a new point (store) into the QuadTree
bool QuadTree::insert(double x, double y, const string& name) {
    // Ensure the point lies within the root boundary
    if (x < root->minX || x > root->maxX || y < root->minY || y > root->maxY) {
        cerr << "QuadTree: Point (" << x << "," << y << ") is out of the boundary.\n";
        return false;
    }
    // Check if a store with the same name already exists
    QuadNode* existing = findByName(root, name);
    if (existing != nullptr) {
        cerr << "QuadTree: A store named '" << name << "' already exists at ("
             << existing->x << "," << existing->y << ").\n";
        return false;
    }
    // Insert recursively into the quadtree
    bool inserted = insertNode(root, x, y, name);
    // if (inserted) {
    //     cout << "Inserted store '" << name << "' at (" << x << "," << y << ").\n";
    // }
    return inserted;
}

// Recursive helper for insertion
bool QuadTree::insertNode(QuadNode* node, double x, double y, const string& name) {
    // If this node is a leaf (no children)
    if (node->isLeaf()) {
        if (node->name == "") {
            // Empty leaf: place the store here
            node->x = x;
            node->y = y;
            node->name = name;
            count++;
            return true;
        } else {
            // Leaf already contains a store. We need to subdivide this leaf into four children.
            if (node->x == x && node->y == y) {
                // Exactly same coordinates as existing store
                cerr << "QuadTree: A store already exists at coordinates (" << x << "," << y << ").\n";
                return false;
            }
            // Calculate midpoints to split current node region into four quadrants
            double midX = (node->minX + node->maxX) / 2.0;
            double midY = (node->minY + node->maxY) / 2.0;
            // Create four child quadrants (NW, NE, SW, SE)
            node->NW = new QuadNode(node->minX, midY, midX, node->maxY);
            node->NE = new QuadNode(midX, midY, node->maxX, node->maxY);
            node->SW = new QuadNode(node->minX, node->minY, midX, midY);
            node->SE = new QuadNode(midX, node->minY, node->maxX, midY);
            // Temporarily store the existing point's data
            double oldX = node->x;
            double oldY = node->y;
            string oldName = node->name;
            node->name.clear(); // clear current node's name as it's now an internal node
            // Insert the old store into an appropriate child quadrant
            if (oldX <= midX) {
                if (oldY <= midY) {
                    insertNode(node->SW, oldX, oldY, oldName);
                } else {
                    insertNode(node->NW, oldX, oldY, oldName);
                }
            } else {
                if (oldY <= midY) {
                    insertNode(node->SE, oldX, oldY, oldName);
                } else {
                    insertNode(node->NE, oldX, oldY, oldName);
                }
            }
            // Now insert the new store into the appropriate child quadrant
            if (x <= midX) {
                if (y <= midY) {
                    return insertNode(node->SW, x, y, name);
                } else {
                    return insertNode(node->NW, x, y, name);
                }
            } else {
                if (y <= midY) {
                    return insertNode(node->SE, x, y, name);
                } else {
                    return insertNode(node->NE, x, y, name);
                }
            }
        }
    } else {
        // This node has children, delegate insertion to correct quadrant
        double midX = (node->minX + node->maxX) / 2.0;
        double midY = (node->minY + node->maxY) / 2.0;
        if (x <= midX) {
            if (y <= midY) {
                return insertNode(node->SW, x, y, name);
            } else {
                return insertNode(node->NW, x, y, name);
            }
        } else {
            if (y <= midY) {
                return insertNode(node->SE, x, y, name);
            } else {
                return insertNode(node->NE, x, y, name);
            }
        }
    }
}

// Remove a store by name
bool QuadTree::remove(const string& name) {
    // Find the node with this store name to get its coordinates
    QuadNode* node = findByName(root, name);
    if (node == nullptr) {
        cerr << "QuadTree: Store '" << name << "' not found.\n";
        return false;
    }
    // Use the found coordinates to remove the store
    double x = node->x;
    double y = node->y;
    return remove(x, y);
}

// Remove a store by coordinates
bool QuadTree::remove(double x, double y) {
    // Traverse the tree to find the target leaf that matches (x, y)
    QuadNode* current = root;
    QuadNode* target = nullptr;
    QuadNode* parent = nullptr;
    QuadNode** parentLink = nullptr;  // pointer to the parent's child pointer for current
    while (current != nullptr) {
        if (current->isLeaf()) {
            if (!current->name.empty() && current->x == x && current->y == y) {
                target = current;
            }
            break;
        }
        // Determine which quadrant to search next
        double midX = (current->minX + current->maxX) / 2.0;
        double midY = (current->minY + current->maxY) / 2.0;
        parent = current;
        if (x <= midX) {
            if (y <= midY) {
                parentLink = &(current->SW);
                current = current->SW;
            } else {
                parentLink = &(current->NW);
                current = current->NW;
            }
        } else {
            if (y <= midY) {
                parentLink = &(current->SE);
                current = current->SE;
            } else {
                parentLink = &(current->NE);
                current = current->NE;
            }
        }
    }
    if (target == nullptr) {
        cerr << "QuadTree: No store found at (" << x << "," << y << ").\n";
        return false;
    }
    // "Remove" the target by clearing its stored data (keep node structure for simplicity)
    string removedName = target->name;
    target->name.clear();
    count--;
    cout << "Removed store '" << removedName << "' from (" << x << "," << y << ").\n";
    // Note: We do not merge empty child nodes back to a leaf to simplify implementation
    return true;
}

// Find nearest store to a given (x, y) location
bool QuadTree::findNearest(double x, double y, string& nearestName, double& nearestX, double& nearestY, double& distance) const {
    if (count == 0) {
        cerr << "QuadTree: No stores in the quadtree.\n";
        return false;
    }
    double bestDist = numeric_limits<double>::max();
    QuadNode* bestNode = nullptr;
    // Start recursive search for nearest
    nearestNode(root, x, y, bestDist, bestNode);
    if (bestNode) {
        nearestName = bestNode->name;
        nearestX = bestNode->x;
        nearestY = bestNode->y;
        // Calculate actual Euclidean distance from target (x, y)
        distance = sqrt((nearestX - x) * (nearestX - x) + (nearestY - y) * (nearestY - y));
        return true;
    }
    return false;
}

// Recursive helper to find nearest neighbor in subtree
void QuadTree::nearestNode(QuadNode* node, double targetX, double targetY, double& bestDist, QuadNode*& bestNode) const {
    if (node == nullptr) return;
    // If this is a leaf with a store, check the distance to it
    if (node->isLeaf()) {
        if (!node->name.empty()) {
            // Calculate squared distance (to avoid sqrt for comparison)
            double dx = node->x - targetX;
            double dy = node->y - targetY;
            double distSq = dx * dx + dy * dy;
            if (distSq < bestDist) {
                bestDist = distSq;
                bestNode = node;
            }
        }
        return;
    }
    // Determine which quadrant the target falls into (primary search quadrant)
    double midX = (node->minX + node->maxX) / 2.0;
    double midY = (node->minY + node->maxY) / 2.0;
    QuadNode* primary = nullptr;
    QuadNode* other1 = nullptr;
    QuadNode* other2 = nullptr;
    QuadNode* other3 = nullptr;
    if (targetX <= midX) {
        if (targetY <= midY) {
            primary = node->SW;
            other1 = node->NW; other2 = node->SE; other3 = node->NE;
        } else {
            primary = node->NW;
            other1 = node->SW; other2 = node->NE; other3 = node->SE;
        }
    } else {
        if (targetY <= midY) {
            primary = node->SE;
            other1 = node->NE; other2 = node->SW; other3 = node->NW;
        } else {
            primary = node->NE;
            other1 = node->NW; other2 = node->SE; other3 = node->SW;
        }
    }
    // Search in the primary quadrant first (where the point lies)
    if (primary) {
        nearestNode(primary, targetX, targetY, bestDist, bestNode);
    }
    // A helper lambda to compute squared distance from target to a quadrant region
    auto distToRegion = [&](QuadNode* qnode) {
        if (!qnode) return numeric_limits<double>::infinity();
        double dx = 0.0, dy = 0.0;
        if (targetX < qnode->minX) {
            dx = qnode->minX - targetX;
        } else if (targetX > qnode->maxX) {
            dx = targetX - qnode->maxX;
        }
        if (targetY < qnode->minY) {
            dy = qnode->minY - targetY;
        } else if (targetY > qnode->maxY) {
            dy = targetY - qnode->maxY;
        }
        return dx * dx + dy * dy;
    };
    // Check other quadrants if their region could contain a closer point
    if (other1 && distToRegion(other1) < bestDist) {
        nearestNode(other1, targetX, targetY, bestDist, bestNode);
    }
    if (other2 && distToRegion(other2) < bestDist) {
        nearestNode(other2, targetX, targetY, bestDist, bestNode);
    }
    if (other3 && distToRegion(other3) < bestDist) {
        nearestNode(other3, targetX, targetY, bestDist, bestNode);
    }
}

// Find a node by store name (DFS traversal)
QuadTree::QuadNode* QuadTree::findByName(QuadNode* node, const string& name) const {
    if (node == nullptr) return nullptr;
    if (node->name == name) {
        return node;
    }
    // Recursively search all children
    QuadNode* found = findByName(node->NW, name);
    if (found) return found;
    found = findByName(node->NE, name);
    if (found) return found;
    found = findByName(node->SW, name);
    if (found) return found;
    return findByName(node->SE, name);
}

// Print all store locations in the QuadTree
void QuadTree::printLocations() const {
    cout << "Store locations (total " << count << "):\n";
    // Lambda for recursive traversal
    function<void(QuadNode*)> traverse = [&](QuadNode* node) {
        if (node == nullptr) return;
        if (!node->name.empty()) {
            cout << "  " << node->name << " (" << node->x << ", " << node->y << ")\n";
        }
        if (!node->isLeaf()) {
            traverse(node->NW);
            traverse(node->NE);
            traverse(node->SW);
            traverse(node->SE);
        }
    };
    traverse(root);
}
