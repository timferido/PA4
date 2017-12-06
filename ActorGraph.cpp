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
#include <unordered_set>
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

	//messages
	cout << "#nodes: " << graph.size() << '\n';
	cout << "#movies: " << movieMap.size() << '\n';
	int edgeCount = 0;

  
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
			if (*itrcast != *itractor) {
				temp->adj.insert(std::pair<std::string,std::string>(*itrcast, (*itrmovie).first));
				edgeCount++;
			}
		}
	
		//message
		cout << "#edges: " << edgeCount << '\n';

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


int ActorGraph::countAdj(string name) {

	auto found = graph.find(name);
	auto aNode = found->second;
	int size = 0;
	unordered_set<string> count;		//create unordered set 

	if (found != graph.end()) {
		// size = ((found->second)->adj).size();
		//loop through multimap adjlist and count only
		//unique actors
		
		for (auto actor = (aNode->adj).begin(); actor != (aNode->adj).end(); actor++) {
			count.insert(actor->first);
		}
	}

	return (count.size());
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

	//OUTPUT WHICH PAIR IS BEING FOUND	
	cout << "Computing path for (" << actor_start << ") -> (" << actor_end << ")\n";

	string path = "";	//will hold the path to return

	//stack to reset all the nodes fields when done
	stack <Node*> resetAll;

	/*---------------WEIGHTED---------------------*/
	if (weighted) {

		priority_queue<pair<int,Node*>, vector<pair<int,Node*>>, NodePtrComp> pq;   //initialize priority queue 
		auto found = graph.find(actor_start);
		Node* begin = found->second;  //find actor in graph
		begin->dist = 0;    //set distance to 0 for that node
		pq.push(make_pair(0,begin)); //enqueue the first node 
		int c = 0;
		
		//write to the string
		path = "(" + ((pq.top()).second)->actorName + ")";

		while (!pq.empty()) {

			//dq node v from front of q
			Node* v = (pq.top()).second;
			
			// //break out if actor_end is found
			// if (v->actorName == actor_end) {
			//     break;
			// }
			
			resetAll.push(v);
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
	} 
	/*---------------------WEIGHTED------------------*/
		
	/*---------------------UNWEIGHTED----------------*/
	else {
		
		queue<Node*> q;
		auto found = graph.find(actor_start);
		auto s = found->second;
		q.push(s);
		s->dist = 0;
		
		//write to the string
		path = "(" + q.front()->actorName + ")";
		

		while (!q.empty()) {
			auto curr = q.front();
			resetAll.push(curr);
			q.pop();
			for (auto itr = curr->adj.begin(); itr != curr->adj.end(); itr++) {
				auto n = graph.find(itr->first)->second;
				if (n->dist == 32767) {
					n->dist = curr->dist+1;
					n->prev = curr;
					q.push(n);
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
	unordered_multimap<string,string> adjList;
    
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
        
        path += "--[" + actormovie.second + "]-->(" + (actormovie.first)->actorName + ")";
	}
	
	//now reset ALL THE NODES
	while (!resetAll.empty()) {
		Node* c = resetAll.top();
		resetAll.pop();
		c->done = false;
		c->dist = 32767;
		c->prev = nullptr;
	}
    
	//now return the path
	return path;
}

