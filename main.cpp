/* Author: Timothy Ferido, Kent Nguyen
Date: 11-16-17
File: main.cpp
Description: This file is the main executable program
*/

#include <iostream>
#include "ActorGraph.h"
#include <fstream>

using namespace std;

int main(int argc, char* argv[]) {

    //load from file
    ActorGraph graph;
    bool test = graph.loadFromFile(argv[1], false);
    
    graph.createGraph();
	
    // Initialize the file stream
    ifstream infile(argv[2]);
    ofstream outfile;

    outfile.open(argv[3]);

    outfile << "Number of Neighbors" << endl;

    bool have_header = false;

    //trackers
    int currActorIdx = 0;

    // keep reading lines until the end of file is reached
    while (infile) {
        string s;

        // get the next line
        if (!getline( infile, s )) break;

        if (!have_header) {
            // skip the header
            have_header = true;
            continue;
        }
	
	outfile << graph.countAdj(s) << endl; 
    }
	

    if (!infile.eof()) {
        cerr << "Failed to read " << argv[2] << "!\n";
        return false;
    }
    infile.close();
    outfile.close();
    
    return 0;
}

