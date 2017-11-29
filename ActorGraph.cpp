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

ActorGraph::~ActorGraph() {

	//parse through graph deleting nodes

		auto itr = graph.begin();
		auto end = graph.end();

		while (itr != end) {
			delete *itr;

			itr++;
		}
		
}

void ActorGraph::createGraph(void) {
  
	//iterate through movies
	auto itrmovie = movieMap.begin();
	auto endmovie = movieMap.end();

	while (itrmovie != endmovie)
	{
		//first loop to establish actor nodes in graph
		for (auto itractor = (itrmovie->second).begin(); itractor != (itrmovie->second).end(); ++itractor) {
		
		bool actorExist = false;


           	//check if actor is in graph
            	auto itrgraph = graph.begin();
            	auto endgraph = graph.end();


		Node* temp;

            	while (itrgraph != endgraph) {
                
                	//if yes, add other actors with movie and year into adj
                	if ((*itrgraph)->actorName == *itractor)
                	{
                    		actorExist = true;
                    		temp = *itrgraph;
                    		break;
                	}
                
                itrgraph++;
            	}
            
            	//if no, create new Node and insert to vector
            	if (!actorExist) {
                	temp = new Node(*itractor);
			graph.push_back(temp);
            	}
		
		

		//add the actor to the adjacency list of current actor node
		//check if actor is itself
		//nested loop to add all the other actors to adjacency list
		for(auto itrcast = (itrmovie->second).begin(); itrcast != (itrmovie->second).end(); ++itrcast) {
			//check if actor is itself
			if (*itrcast != *itractor)
				temp->adj.insert(std::pair<std::string,std::string>(*itrcast, (*itrmovie).first));
		}
	

		}
		itrmovie++;
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
        
	//check if titleyear exists in the movieMap
		//if so, then add actor to the corresponding vector
		//if not, then create a new pair with titleyear and push actor onto vector

	if (movieMap.find(titleyear) == movieMap.end()) {
		std::vector<std::string> cast;
		auto pear = std::pair<std::string,std::vector<std::string>>(titleyear,cast);
		movieMap.insert(pear);
	} 
	//find the movie again and push the actor onto cast 
	auto currMovie = movieMap.find(titleyear);
	(currMovie->second).push_back(actor_name);
    }

    if (!infile.eof()) {
        cerr << "Failed to read " << in_filename << "!\n";
        return false;
    }
    infile.close();

    return true;
}


void ActorGraph::printAdj(string name) {

	//find actor in the graph
	auto itractor = graph.begin();
	auto endactor = graph.end();

	while (itractor != endactor) {
		
		//compare the name field of curr actor to param
		if ((*itractor)->actorName == name) {
			break;
		}
		itractor++;
	}


	//actor was found
	if (itractor != endactor) {

		//print the contents of adj map
		auto itradj = ((*itractor)->adj).begin();
		auto endadj = ((*itractor)->adj).end();

		//loop through the adjacency list
		while (itradj != endadj) {
			
			//print each actor name
			cout << itradj->first << '\t' << itradj->second << '\n';

			itradj++;
		}
	} else {
	//actor not found
		cout << "actor does not exist.\n";	
	}
}

int ActorGraph::countAdj(string name) {
	
	//find actor in the graph
	auto itractor = graph.begin();
	auto endactor = graph.end();

	while (itractor != endactor) {
		
		//compare the name field of curr actor to param
		if ((*itractor)->actorName == name) {
			break;
		}
		itractor++;
	}

	//actor was found
	if (itractor != endactor) {

		// return the count of adj
		return (*itractor)->adj.size();
	} else {
	//actor not found
		cout << "actor does not exist.\n";
		return 0;	
	}


}


Node* ActorGraph::find(string actor) {

	//iterate through graph
	
	for (auto itr = graph.begin(); itr != graph.end(); itr++) {
		if ((*itr)->actorName == actor) 
			return *itr;
	}

}

int ActorGraph::edgeWeight(string movieyear) {
    string my = movieyear;

    istringstream ss( my );
    vector <string> record;

    while (ss) {
        string next;
        
        // get the next string before hitting a tab character and put it in 'next'
        if (!getline( ss, next, '\t' )) break;

        record.push_back( next );
    }

    // if (record.size() != 2) {
    //     // we should have exactly 3 columns
    //     continue;
    // }

    int year(stoi (record[1]));

    return 1 + (2015 - year);
}

string ActorGraph::findPath(string actor_start, string actor_end, bool weighted) {

	priority_queue<int, Node, NodePtrComp> pq;   //initialize priority queue 
	Node* begin = find(actor_start);  //find actor in graph
	begin->dist = 0;    //set distance to 0 for that node
	pq.push_back(0, begin); //enqueue the first node 

	while (!pq.empty()) {
		//dq node v from front of q
		Node* v = pq.top();
		pq.pop();

		if (!v->done) { //if v is not done
			v->done = true;
			//for each of v's neighbors
			for (auto itr = v->adj.begin(); itr != v->adj.end(); itr++) {
				Node* w = find((*itr).first);   //current neighbor
				int c;
				if (weighted) {
					c = v->dist + edgeWeight((*itr).second);
				}
				else {
					c = v->dist + 1;
				}

				if (c < w->dist) {
					w->prev = v;
					w->dist = c;
					pq.push_back(c, w);
				}
			}
		}
	}
}
