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
#include <algorithm>

// Maybe include some data structures here
#include <vector>
#include <string>

using namespace std;


class Node {
public:
    unordered_map<string,string> adj;
    string actorName;
    int dist;
    int index;
    int prev;
    
    public:
        Node(string actor) : actorName(actor), dist(0), index(0), prev(0) {};
	~Node();
};

class ActorGraph {
    protected:

        // Maybe add class data structure(s) here
	std::unordered_map<std::string,std::vector<std::string>> movieMap;

	vector<Node*> graph;

	
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

	void printAdj(string name);

	int countAdj(string name);

	
};


#endif // ACTORGRAPH_H
