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

/*
Class: ActorGraph
Description: Contains all the actorNodes and the movieMaps needed to contruct
				a connected graph.
Data Fields: movieMap - hash mao of key of astring movieyear and value of a 
					vector of actors in the movie
			 ACmovieMap - hash map of key of an int year and a value of a 
					hash table full of movies
			 graph - hash map of key of string actor names and value of a 
					actorNode pointer
			 
*/
class ActorGraph {
    protected:
		std::unordered_map<std::string,std::vector<std::string>> movieMap;
		std::unordered_map<int,std::unordered_set<std::string>> ACmovieMap;
		std::unordered_map<std::string,Node*> graph;

    
    public:
		//constructor and deconstructor
        ActorGraph(void);
        ~ActorGraph();

		//create graph method
        void createGraph(void);

        //load the movieMaps from the file
        bool loadFromFile(const char* in_filename, bool use_weighted_edges);
        bool ACloadFromFile(const char* in_filename);

		//main methods
        int countAdj(string name);

		//pathfinder methods
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
