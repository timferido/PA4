#include "Node.h"

// comparison operamaketor for node
bool Node::operator<(const Node& other) {    
    return dist < other.dist;
}

// Node::~Node() {}