/* Author: Timothy Ferido, Kent Nguyen
Date: 12-8-17
File: actorconnections.cpp
Description: This file is an executable program which
            finds the year which both actors became connected.
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

    //run timer
    Timer t;
    t.begin_timer();

    //check if infile has header
    bool have_header = false;

    //load from file
    ActorGraph graph;
    bool test = graph.ACloadFromFile(argv[1]);
    
    //initialize the file stream
    ifstream infile(argv[2]);
    ofstream outfile;

    //open output file
    outfile.open(argv[3]);
    
    //output header to output file
    outfile << "Actor1    Actor2    Year" << endl;

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
            
            // get the next string before hitting a tab character and put it in 'next'
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

        //check to do bfs or ufind
        if (argv[4][0] == 'b') {
            outfile << graph.ACbfs(actor_start, actor_end) << '\n';
        } else {
            outfile << graph.ACufind(actor_start, actor_end) << '\n';
        }
    }

    //close the file
    outfile.close();

    //end timer
    cout << "Program ran in " << t.end_timer() << " nanoseconds\n\n";
    
    return 0;
}
