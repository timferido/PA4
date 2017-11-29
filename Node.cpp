#include "Node.h"

//comparison operator for node
bool Node::operator<(const Node& other) {    
    return actorName < other.actorName;
}

Node::~Node() {}