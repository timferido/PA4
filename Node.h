/* Author: Timothy Ferido, Kent Nguyen
Date: 12-8-17
File: Node.h
Description: The Node class contains the actor and its adjacency list including
            data fields to help generate paths through BFS or Dijkstra's.
*/

#include <unordered_map>
#include <string>
#include <algorithm>
#include <utility>

using namespace std;

/*
Class: Node
Data Fields:    adj - contains pairs of all of the actors and the connected
                    movie that this particular actor shares
                actorName - the name of the actor
                dist - distance away from the starting Node in the graph
                prev - contains a pointer to the previous Node in the graph
                    for path purposes
                done - used for weighted paths to signify if two actors are 
                    connected or not
                prevMovie - help specify the particular movie as the algorithm
                    traces back the path
*/
class Node {
public:
    unordered_multimap<string,string> adj;
    string actorName;
    int dist;
    Node* prev;
    bool done;
    string prevMovie;

    //comparison class
    bool operator<(const Node& other);
    
    public:
        //guarenteed initialization
        Node(string actor) : actorName(actor), dist(32767), prev(0),
            done(false) {};
};
