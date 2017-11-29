#include "Node.h"

// comparison operamaketor for node
bool Node::operator<(const Node& other) {    
    return actorName < other.actorName;
}

// Node::~Node() {}