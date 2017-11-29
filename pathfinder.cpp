/* Author: Timothy Ferido, Kent Nguyen
Date: 11-28-17
File: pathfinder.cpp
Description: This file is the pathfinder executable program which
            finds the shortest path between two actors.
*/

#include <iostream>
#include "ActorGraph.h"
#include <fstream>
#include <queue>
#include <string>
#include <sstream>

using namespace std;

int main(int argc, char* argv[]) {

    //load from file
    ActorGraph graph;
    bool test = graph.loadFromFile(argv[1], false);
    
    graph.createGraph();
	
    // Initialize the file stream
    ifstream infile(argv[3]);
    ofstream outfile;

    outfile.open(argv[4]);

    //read from file
    while (infile) {
        string s;

        // get the next line
        if (!getline( infile, s )) break;

        // if (!have_header) {
            // skip the header
            // have_header = true;
            // continue;
        // }

        istringstream ss( s );
        vector <string> record;

        while (ss) {
			string next;
			
            // get the next string before hitting a tab character and put it in 'next'
            if (!getline( ss, next, '\t' )) break;

            record.push_back( next );
        }

        if (record.size() != 2) {
            // we should have exactly 2 columns
            continue;
        }

        string actor_start(record[0]);
        string actor_end(record[1]);

        //write to output file
        outfile << graph.findPath(actor_start,actor_end, argv[2]) << '\n';
    }

    return 0;
}