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
#include <queue>
#include <unordered_map>
#include <string>
#include <utility>
#include <vector>
#include <stack>
#include <algorithm>
#include "ActorGraph.h"


using namespace std;

ActorGraph::ActorGraph(void) {}

ActorGraph::~ActorGraph() {

	//parse through graph deleting nodes

		auto itr = graph.begin();
		auto end = graph.end();

		while (itr != end) {
			// delete *itr;

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
            auto found = graph.find(*itractor);

            Node* temp;

            if (found != graph.end()) {
                temp = (*found).second;
            }
            else {
                temp = new Node(*itractor);
				temp->dist = 32767;
				graph.insert(make_pair(*itractor, temp));
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
		if (((*itractor).second)->actorName == name) {
			break;
		}
		itractor++;
	}


	//actor was found
	if (itractor != endactor) {

		//print the contents of adj map
		auto itradj = (((*itractor).second)->adj).begin();
		auto endadj = (((*itractor).second)->adj).end();

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

	auto found = graph.find(name);
   int size = 0;

	if (found != graph.end()) {
		size = ((found->second)->adj).size();
   }

   return size;
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

	string path = "";	//will hold the path to return

		//re initialize all nodes fields
		for (auto nodeitr = graph.begin(); nodeitr != graph.end(); nodeitr++) {
			nodeitr->second->done = false;
			nodeitr->second->dist = 32767;
			nodeitr->second->prev = nullptr;
		}

	priority_queue<pair<int,Node*>, vector<pair<int,Node*>>, greater<pair<int,Node*>>> pq;   //initialize priority queue 
	auto found = graph.find(actor_start);
	Node* begin = found->second;  //find actor in graph
	begin->dist = 0;    //set distance to 0 for that node
	// begin->done = false;
	// begin->prev = nullptr;
	pq.push(make_pair(0,begin)); //enqueue the first node 
	int c = 0;
    
    //write to the string
	path = "(" + ((pq.top()).second)->actorName + ")";



	while (!pq.empty()) {

		//dq node v from front of q
		Node* v = (pq.top()).second;
		pq.pop();

		if (!v->done) { //if v is not done
			v->done = true;
			//for each of v's neighbors
			for (auto itr = v->adj.begin(); itr != v->adj.end(); itr++) {
				Node* w = (graph.find((*itr).first))->second;   //current neighbor

				if (weighted) {
					c = v->dist + edgeWeight((*itr).second);
				}
				else {
					c = v->dist + 1;
				}

				if (c < w->dist) {
					w->prev = v;
					w->dist = c;
					pq.push(make_pair(c,w));
				}
			}
		}
	}

    //stack the path
    stack <pair<Node*, string>> pathstack;
    
    //find the last actor
    auto endactor = graph.find(actor_end);
    
    //set the curr node to the end actor
    Node* curr = endactor->second;
    Node* prev;
	string movieyear;
	unordered_map<string,string> adjList;
    
    //get the path backwards
    while (curr->actorName != actor_start) {
        
        //get previous node
		prev = curr->prev;
		
		//get adjacency list
		adjList = prev->adj;

		auto x = adjList.find(curr->actorName);	
        
        //get movieyear by finding curr actor in prev actor adj list
        // movieyear = (prev->adj.find(curr->actorName))->second;
		movieyear = x->second;
		
        istringstream ss( movieyear );
        vector <string> record;

        while (ss) {
            string next;
        
            // get the next string before hitting a tab character and put it in 'next'
            if (!getline( ss, next, '\t' )) break;

            record.push_back( next );
        }
        
        movieyear = record[0] + "#@" + record[1];
        
        //push onto stack
        pathstack.push(make_pair(curr, movieyear));
        
        //set prev to curr
        curr = curr->prev;
    }
    
    //pop stack to get the path from start to end
    while (!pathstack.empty()) {
        
		auto actormovie = pathstack.top();
		pathstack.pop();

		// (actormovie.first)->done = false;
		// (actormovie.first)->dist = 32767;
		// (actormovie.first)->prev = nullptr;
        
        path += "--[" + actormovie.second + "]-->(" + (actormovie.first)->actorName + ")";
    }
    
	//now return the path
	return path;
}

