/* Author: Timothy Ferido, Kent Nguyen
Date: 12-8-17
File: pathfinder.cpp
Description: This file is the pathfinder executable program which
            finds the shortest path between two actors.
*/

#include <iostream>
#include "ActorGraph.h"
#include "util.h"
#include <fstream>
#include <queue>
#include <string>
#include <sstream>
#include <algorithm>

using namespace std;

int main(int argc, char* argv[]) {

    bool have_header = false;

    //load from file
    ActorGraph graph;
    bool test = graph.loadFromFile(argv[1], false);
    
    //create the graph
    graph.createGraph();
    
    //initialize the file stream
    ifstream infile(argv[3]);
    ofstream outfile;

    //open output file
    outfile.open(argv[4]);
    
    //output beginning line
    outfile << "(actor)--[movie#@year]-->(actor)--..." << endl;

    //read from file
    while (infile) {
        string s;

        // get the next line
        if (!getline( infile, s )) break;

        if (!have_header) {
            // skip the header
            have_header = true;
            continue;
        }

        istringstream ss( s );
        vector <string> record;

        while (ss) {
            string next;
            
            // get the next string before hitting a tab and put it in 'next'
            if (!getline( ss, next, '\t' )) break;

            record.push_back( next );
        }

        if (record.size() != 2) {
            // we should have exactly 2 columns
            continue;
        }

        //get the starting actor and the ending actor
        string actor_start(record[0]);
        string actor_end(record[1]);
        
        //write path to output file
        if (argv[2][0] == 'w') {
            outfile << graph.findPath(actor_start,actor_end, true) << '\n';
        } else {
            outfile << graph.findPath(actor_start,actor_end, false) << '\n';
        }
    }

    //close output file
    outfile.close();
    
    return 0;
}
