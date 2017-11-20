/*
 * ActorGraph.cpp
 * Author: Timothy Ferido, Kent Nguyen
 * Date:   11-16-17
 *
 * This file is meant to exist as a container for starter code that you can use to read the input file format
 * defined in movie_casts.tsv. Feel free to modify any/all aspects as you wish.
 */

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include "ActorGraph.h"

using namespace std;

ActorGraph::ActorGraph(void) {}

vector<Node*> ActorGraph::createGraph(void) {
    //iterate through the movie multiset
    auto itr = movieMap.begin();
    auto end = movieMap.end();
    
    string actor = "";
    Node* temp;
    
    while (itr != end)
    {   
        bool actorExist = false;
        string actor;
        string actorName;

        //for each actor in the movie
        auto range = movieMap.equal_range((*itr)->first));
        for (auto itractor = range.first; itractor != range.second; ++itractor) {
            actor = itractor->second;
            
            //check if actor is in graph
            auto itrgraph = graph.begin();
            auto endgraph = graph.end();
            
            while (itrgraph != endgraph) {
                
                //if yes, add other actors with movie and year into adj
                if ((*itrgraph).actorName == actor)
                {
                    actorExist = true;
                    temp = itrgraph;
                    break;
                }
                
                itrgraph++;
            }
            
            //if no, create new Node and insert to vector
            if (!actorExist) {
                temp = new Node(actor);
            }
            
            //add other actors with movie and year into adj
            auto rangeadj = movieMap.equal_range((*itr)->first));
            for (auto itractoradj = rangeadj.first; itractoradj != rangeadj.second; ++ itractoradj) {
                
                if ((*itractoradj).first != actor)
                    temp->adj.insert(make_pair<string,string>((*itractoradj).first, *itr);
                    
            }
            
        }
        
    
        
    
        itr++;
    }
}

bool ActorGraph::loadFromFile(const char* in_filename, bool use_weighted_edges) {
    // Initialize the file stream
    ifstream infile(in_filename);

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

        istringstream ss( s );
        vector <string> record;

        while (ss) {
            string next;

            // get the next string before hitting a tab character and put it in 'next'
            if (!getline( ss, next, '\t' )) break;

            record.push_back( next );
        }

        if (record.size() != 3) {
            // we should have exactly 3 columns
            continue;
        }

        string actor_name(record[0]);
        string movie_title(record[1]);
        int movie_year = stoi(record[2]);

        // we have an actor/movie relationship, now what?
        // update the graph
        string titleyear = movie_title +"\t" + record[2];
        
        std::pair<std::string,std::string> pear (titleyear,actor_name);

        movieMap.insert(pear);
        
    }

    if (!infile.eof()) {
        cerr << "Failed to read " << in_filename << "!\n";
        return false;
    }
    infile.close();
    
    

    return true;
}
