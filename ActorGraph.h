/*
 * ActorGraph.h
 * Author: <YOUR NAME HERE>
 * Date:   <DATE HERE>
 *
 * This file is meant to exist as a container for starter code that you can use to read the input file format
 * defined in movie_casts.tsv. Feel free to modify any/all aspects as you wish.
 */

#ifndef ACTORGRAPH_H
#define ACTORGRAPH_H

#include <iostream>
#include <unordered_map>
#include <map>
#include <algorithm>
#include <utility>
// Maybe include some data structures here
#include <vector>
#include <string>
#include <queue>
#include <sstream>

#include "Node.h"

using namespace std;

class NodePtrComp {
public: 
    bool operator() (pair<int, Node*&> lhs, pair<int, Node*&> rhs) const {
        return (lhs.second) < (rhs.second);
    }
};

class ActorGraph {
    protected:

        // Maybe add class data structure(s) here
	std::unordered_map<std::string,std::vector<std::string>> movieMap;

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

    int countAdj(string name);
    
    // Node* find(string actor);

    int edgeWeight(string movieyear);

    string findPath(string actor_start, string actor_end, bool weighted);
};


#endif // ACTORGRAPH_H
