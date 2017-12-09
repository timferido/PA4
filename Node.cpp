/* Author: Timothy Ferido, Kent Nguyen
Date: 12-8-17
File: Node.cpp
Description: Has the overwritten comparison operator for comparing Nodes 
                less than each other
*/

#include "Node.h"

// comparison operator for node
bool Node::operator<(const Node& other) {    
    return dist < other.dist;
}