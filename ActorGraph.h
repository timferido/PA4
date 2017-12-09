/* Author: Timothy Ferido, Kent Nguyen
Date: 12-8-17
File: ActorGraph.h
Description: Contains the ActorGraph datafields and functions used to create a graph
            of actors and their connections between each other.
*/


#ifndef ACTORGRAPH_H
#define ACTORGRAPH_H

#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <map>
#include <algorithm>
#include <utility>
// Maybe include some data structures here
#include <vector>
#include <string>
#include <queue>
#include <sstream>
//timer
#include <chrono>


#include "Node.h"

using namespace std;

typedef unordered_map<string,string>::iterator ufnode;

/*
Class: NodePtrComp
Description: Compairs two pairs of the weighted dist and the actorNodes for weighted
*/
class NodePtrComp {
public: 
    bool operator() (pair<int, Node*> lhs, pair<int, Node*> rhs) const {
        return (lhs.first) > (rhs.first);
    }
};

class ActorGraph {
    protected:

        // Maybe add class data structure(s) here
    std::unordered_map<std::string,std::vector<std::string>> movieMap;
    std::unordered_map<int,std::unordered_set<std::string>> ACmovieMap;
    std::unordered_map<std::string,Node*> graph;

    
    public:
    

        ActorGraph(void);
        ~ActorGraph();

        // Maybe add some more methods here

        void createGraph(void);

        /** You can modify this method definition as you wish
         *
         * Load the graph from a tab-delimited file of actor->movie relationships.
         *
         * in_filename - input filename
         * use_weighted_edges - if true, compute edge weights as 1 + (2015 - movie_year), otherwise all edge weights will be 1
         *
         * return true if file was loaded sucessfully, false otherwise
         */
        bool loadFromFile(const char* in_filename, bool use_weighted_edges);
        bool ACloadFromFile(const char* in_filename);

        int countAdj(string name);
        
        // Node* find(string actor);

        int edgeWeight(string movieyear);
        string findPath(string actor_start, string actor_end, bool weighted);

        //actorconnection methods
        string ACbfs(string actor_start, string actor_end);
        string ACufind(string actor_start, string actor_end);

        /*utility*/
        void resetAdj(void);
        void begin_timer();
        long long end_timer();

        /*dijointset*/
        
        ufnode root(ufnode x, unordered_map<string,string> &m);
        
};


#endif // ACTORGRAPH_H
