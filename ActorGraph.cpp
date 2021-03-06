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
#include "util.h"


using namespace std;




/*-----------------------------------------------------------------------------
Function: ActorGraph
Description: constructor
Input: none
Output: none
-----------------------------------------------------------------------------*/
ActorGraph::ActorGraph(void) {}




/*-----------------------------------------------------------------------------
Function: ~ActorGraph
Description: destructor
Input: none 
Output: none
-----------------------------------------------------------------------------*/
ActorGraph::~ActorGraph() {

    //parse through graph deleting nodes with iterators
    auto itr = graph.begin();
    auto end = graph.end();

    //delete Nodes
    while (itr != end) {
        delete itr->second;
        itr++;
    }        
}




/*-----------------------------------------------------------------------------
Function: createGraph 
Description: this function populates the graph data structure (adjlist) with
all the nodes and their adjacent nodes 
Input: none
Output: none
-----------------------------------------------------------------------------*/
void ActorGraph::createGraph(void) {

    //count edges in the graph
    int edgeCount = 0;

    //iterate through movies
    auto itrmovie = movieMap.begin();
    auto endmovie = movieMap.end();

    //use while loop to iterate
    while (itrmovie != endmovie)
    {
        //first loop to establish actor nodes in graph
        for (auto itractor = (itrmovie->second).begin(); itractor != (itrmovie->second).end(); ++itractor) {

            //check if actor is in graph
            auto found = graph.find(*itractor);

            //create temporary Node
            Node* temp;

            //if found in graph, set actor to temp
            if (found != graph.end()) {
                temp = (*found).second;
            }
            //if not found in graph, create new Node with actor and insert
            else {
                temp = new Node(*itractor);
                temp->dist = 32767;
                graph.insert(make_pair(*itractor, temp));
            }
        
            //add the actor to the adjacency list of current actor node
            auto e = (itrmovie->second).end();
            for(auto i=(itrmovie->second).begin();i!=e;++i) {
                //check if actor is itself
                if (*i != *itractor) {
                    //add other actors in curent actors adjList
                    temp->adj.insert(make_pair(*i, (*itrmovie).first));
                    edgeCount++;
                }
            }
        }
        //increment the movie
        itrmovie++;
    }
    
    //message
    cout << "#nodes: " << graph.size() << '\n';
    cout << "#movies: " << movieMap.size() << '\n';
    cout << "#edges: " << edgeCount << '\n';
}




/*-----------------------------------------------------------------------------
Function: loadFromFile
Description: this function creates a full graph with all nodes and edges from
given input
Input: const char* in_filename - the file to be read in
bool use_weighted_edges - whether or not to calculate considering weight or not
Output: bool - success indicator
-----------------------------------------------------------------------------*/
bool ActorGraph::loadFromFile(const char* in_filename, bool use_weighted_edges) 
{
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
            
            // get the next string before hitting a tab character and put it in
            // 'next'
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
        //if not, then create a new pair with titleyear and push actor onto 
        //vector

    if (movieMap.find(titleyear) == movieMap.end()) {
        std::vector<std::string> cast;
        auto pear = pair<string,vector<string>>(titleyear,cast);
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




/*-----------------------------------------------------------------------------
Function: ACloadFromFile
Description: creates an empty graph and populates moviemap for the given input
Input: const char* in_filename - file to be read in
Output: bool - success
-----------------------------------------------------------------------------*/
bool ActorGraph::ACloadFromFile(const char* in_filename) {
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
            
            // get the next string before hitting a tab character and put it in
            // 'next'
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
        
        
        
        /*--------------POPULATE movieMap-------------------*/
            
        //create a new pair with titleyear and cast if not found
        if (movieMap.find(titleyear) == movieMap.end()) {
            std::vector<std::string> cast;
            auto pear = std::pair<std::string,vector<string>>(titleyear,cast);
            movieMap.insert(pear);
        } 
        
        //find the movie again and push the actor onto cast 
        auto currMovie = movieMap.find(titleyear);
        (currMovie->second).push_back(actor_name);
        
        
        
        /*-----------------POPULATE graph--------------*/
        //populate an empty graph by not adding actors to adjList
        if (graph.find(actor_name) == graph.end()) {
            Node* temp = new Node(actor_name);
            temp->dist = 32767;
            graph.insert(make_pair(actor_name, temp));
        }

        
        
        /*-----------------POPULATE ACmovieMap--------------*/
        //local variables
        auto yearBucket = ACmovieMap.find(movie_year);

        //if the year exists, add movie to ACmovieMap of that year
        if (yearBucket != ACmovieMap.end()) {
            yearBucket->second.insert(movie_title);
            
        //if not, create new year and insert movie into that year
        } else {
            unordered_set<string> m;                    //create new set 
            m.insert(movie_title);                      //insert first movie into set
            ACmovieMap.insert(make_pair(movie_year, m));//first yearBucket
        }
    }

    //infile check
    if (!infile.eof()) {
        cerr << "Failed to read " << in_filename << "!\n";
        return false;
    }
    infile.close();
    
    //output message
    cout << "#nodes: " << graph.size() << '\n';
    cout << "#movies: " << movieMap.size() << '\n';
    cout << "#edges: 0\n\n";

    return true;
}




/*-----------------------------------------------------------------------------
Function: countAdj
Description: counts the number of unique adjacencies for the given node
Input: string name
Output: int - the unique adjacencies
-----------------------------------------------------------------------------*/
int ActorGraph::countAdj(string name) {

    auto found = graph.find(name);
    auto aNode = found->second;
    int size = 0;
    unordered_set<string> count;        //create unordered set 

    if (found != graph.end()) {
        //loop through multimap adjlist and count only
        //unique actors
        
        for (auto a = (aNode->adj).begin(); a != (aNode->adj).end(); a++) {
            count.insert(a->first);
        }
    }
    return (count.size());
}




/*-----------------------------------------------------------------------------
Function: edgeWeight
Description: calculates the weight of the edge based on the movie year
Input: string movieyear
Output: int - the weight 
-----------------------------------------------------------------------------*/
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

    int year(stoi (record[1]));

    return 1 + (2015 - year);
}




/*-----------------------------------------------------------------------------
Function: findPath
Description: this function finds the shortest path between two actors 
Input: string actor_start, string actor_end
Output: string - this is the path including the movies and intermediate actors
-----------------------------------------------------------------------------*/
string ActorGraph::findPath(string actor_start, string actor_end, bool weighted) 
{
    //OUTPUT WHICH PAIR IS BEING FOUND    
    cout << "Computing path for (" << actor_start <<") -> ("<<actor_end<<")\n";

    string path = "";    //will hold the path to return

    //stack to reset all the nodes fields when done
    stack <Node*> resetAll;

    /*---------------WEIGHTED---------------------*/
    if (weighted) {

        //initialize priority queue 
        priority_queue<pair<int,Node*>, vector<pair<int,Node*>>, NodePtrComp> pq;   
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
                        w->prevMovie = itr->second;
                        w->dist = c;
                        pq.push(make_pair(c,w));
                    }
                }
            }
        }
    } 
        
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
                    n->prevMovie = itr->second;
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
        
        //get movieyear by finding curr actor in prev actor adj list
        movieyear = curr->prevMovie;
        
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




/*-----------------------------------------------------------------------------
Function: ACbfs
Description: finds the year in which two actors become connected by adding the
movies year by year and doing a breadth first search through current graph
Input: string actor_start, string actor_end
Output: string - first actor \t second actor \t year in which they connect
-----------------------------------------------------------------------------*/
string ActorGraph::ACbfs(string actor_start, string actor_end) {

    //output message
    cout << "Computing: " <<actor_start <<" -> "<<actor_end<<'\n';

    //local variables
    int currYear = 2016;
    bool connected = false;

    //find earliest year in acmoviemap
    for (auto i = ACmovieMap.begin(); i != ACmovieMap.end(); i++) {
        if ((i->first) < currYear)
            currYear = i->first;
    }

    //while the actors are not connected or not all movies 
    //have been added
    while (currYear < 2016) {

        //find year in acmoviemap
        auto yearBucket = ACmovieMap.find(currYear);    
        
        //go to next year if not in acmoviemap
        if (yearBucket == ACmovieMap.end()) { 
            currYear++; 
            continue; 
        }

        
        
        /*-------ADD CONNECTIONS FOR YEAR----------*/
        //parse through all movies for that year
        auto mitr = yearBucket->second.begin();
        auto mend = yearBucket->second.end();
        
        //use while loop to parse
        while (mitr != mend) {
            
            //get the cast
            auto m = movieMap.find(*mitr+"\t"+to_string(currYear));
            auto cast = m->second;
            
            //for each actor in cast
            for(auto j = cast.begin(); j != cast.end(); j++) {
                //find the actor in graph
                auto t = graph.find(*j);
                
                //for each actor in cast
                for(auto i = cast.begin(); i != cast.end(); i++) {
                    //check if actor is itself
                    if ( (*i) != (*j) ) {
                        //insert the adjacent actor to the curr actor's adjList
                        t->second->adj.insert(make_pair(*i, m->first));
                        // edgeCount++;
                    }
                }
            }
            //increment the movie iterator
            mitr++;
        }

        
        
        
        /*------BFS SEARCH FROM START ACTOR---------*/

        //use to reinitialize Nodes
        stack<Node*> resetAll;

        queue<Node*> q;                         //queue for BFS
        
        //prepare the queue by inserting starting actor for BFS
        auto found = graph.find(actor_start);
        auto s = found->second;
        q.push(s);
        s->dist = 0;
        
        //while queue not empty
        while (!q.empty()) {
            
            //get the next Node in line of queue
            auto curr = q.front();
            
            //push to resetAll to reinitialize later
            resetAll.push(curr);
            q.pop();
            
            //for each neighbor
            for (auto itr = curr->adj.begin(); itr != curr->adj.end(); itr++) {
                //find in graph
                auto n = graph.find(itr->first)->second;
                //set their data fields for BFS
                if (n->dist == 32767) {
                    n->dist = curr->dist+1;
                    n->prev = curr;
                    q.push(n);
                }
            }
        }
        
        //find the last actor
        auto endactor = graph.find(actor_end);
    
        //set the curr node to the end actor
        Node* curr = endactor->second;
    
        //get previous node
        curr = curr->prev;
        
        //get the path backwards
        while (curr != nullptr) {
            
            //if actor found
            if (curr->actorName == actor_start) {
                
                //now reset ALL THE NODES
                while (!resetAll.empty()) {
                    Node* c = resetAll.top();
                    resetAll.pop();
                    c->done = false;
                    c->dist = 32767;
                    c->prev = nullptr;
                }
                resetAdj();
                return actor_start+"\t"+actor_end+"\t"+to_string(currYear);   
            }
            //get previous node
            curr = curr->prev;       
        }

        //now reset ALL THE NODES
        while (!resetAll.empty()) {
            Node* c = resetAll.top();
            resetAll.pop();
            c->done = false;
            c->dist = 32767;
            c->prev = nullptr;
        }   
        currYear++;    //go to next year
    }
    resetAdj();
    return actor_start+"\t"+actor_end+"\t9999";
}




/*-----------------------------------------------------------------------------
Function: ACufind
Description: finds the year in which two actors become connected by joining the
sets of node connections year by year and checking if they have the same root
Input: string actor_start, string actor_end
Output: string - first actor \t second actor \t year in which they connect
-----------------------------------------------------------------------------*/
string ActorGraph::ACufind(string actor_start, string actor_end) {
cout << "Computing: " <<actor_start <<" -> "<<actor_end<<'\n';

    //local variables
    int currYear = 2016;
    unordered_map<string, string> uptree;

    //find earliest year in acmoviemap
    for (auto i = ACmovieMap.begin(); i != ACmovieMap.end(); i++) {
        if ((i->first) < currYear)
            currYear = i->first;
    }

    //while the actors are not connected or not all movies 
    //have been added

    while (currYear < 2016) {

        //find year in acmoviemap
        auto yearBucket = ACmovieMap.find(currYear);    
        
        //go to next year if not in acmoviemap
        if (yearBucket == ACmovieMap.end()) { 
            currYear++; 
            continue; 
        }

        /*-------ADD CONNECTIONS FOR YEAR----------*/

        //parse through all movies for that year
        auto mitr = yearBucket->second.begin();
        auto mend = yearBucket->second.end();
        while (mitr != mend) {
            auto m = movieMap.find(*mitr+"\t"+to_string(currYear));
            auto cast = m->second;
            
            //get first actor
            string firstactor = *(cast.begin());
            ufnode first = uptree.find(firstactor);
            if (first == uptree.end()) {
                uptree.insert(make_pair(firstactor, ""));
                first = uptree.find(firstactor);
            }
            
            for(auto j = cast.begin(); j != cast.end(); j++) {
                //union to first actor
                if (*j != firstactor) {
                    auto curr = uptree.find(*j);
                    if (curr == uptree.end()) {
                        uptree.insert(make_pair(*j, firstactor));
                    } else {
                        //get to root and join with firstactor
                        auto a = root(curr, uptree);
                        auto b = root(first, uptree);
                    }
                }
            }   
            mitr++;
        }
        
        /*------COMPRESS ALL PATHS---------*/
        
        //iterate through all actors
        for (auto i = uptree.begin(); i != uptree.end(); i++) {
            
            auto curr = uptree.find(i->first);
            auto path = curr;
            string toRoot = curr->second;
            
            //go to the root from the actor
            while (toRoot != "") {
                path = uptree.find(toRoot);
                toRoot = path->second;
            }
            
            //set the root as curr's parent
            curr->second = toRoot;
        }

        /*------CONDUCT UNION FIND FOR CONNECTIONS---------*/

        //find the wanted actors in map
        auto itract1 = uptree.find(actor_start);
        auto itract2 = uptree.find(actor_end);
        
        //check if both in the map
        if (itract1 != uptree.end() && itract2 != uptree.end()) {
            
            //get their parent
            string actor1par = itract1->second;
            string actor2par = itract2->second;
            
            //compare parents
            if (actor1par == actor2par) {
                return actor_start+"\t"+actor_end+"\t"+to_string(currYear);
            }
        }
        currYear++;    //go to next year
    }
    return actor_start+"\t"+actor_end+"\t9999";
}




/*-----------------------------------------------------------------------------
Function: resetAdj
Description: utility function that clears the adjacency lists of all nodes
Input: none
Output: none
-----------------------------------------------------------------------------*/
void ActorGraph::resetAdj(void) {
    for(auto i = graph.begin();i!=graph.end();i++){
        i->second->adj.clear();
    }
}




/*-----------------------------------------------------------------------------
Function: begin_timer
Description: starts the time recording when called
Input: none
Output: none
-----------------------------------------------------------------------------*/
void Timer::begin_timer()
{
    start = std::chrono::high_resolution_clock::now();
}




/*-----------------------------------------------------------------------------
Function: end_timer
Description: returns the current time when called in reference to the beginning
Input: none
Output: long - a value to the total time
-----------------------------------------------------------------------------*/
long long Timer::end_timer()
{   
    std::chrono::time_point<std::chrono::high_resolution_clock> end;
    end = std::chrono::high_resolution_clock::now();   
    return (long long)std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
}




/*-----------------------------------------------------------------------------
Function: root
Description: this function returns a pointer to the root node of the curr node
by traversing all the way up
Input: ufnode x - the node to find the root of
Output: unordered_map<string,string> &m a reference of the hash table backing
the disjoint set.
-----------------------------------------------------------------------------*/
ufnode ActorGraph::root(ufnode x, unordered_map<string,string> &m) {
    auto curr = m.find(x->first);
    string p = x->second;
    while (p != "") {
        curr = m.find(p);
        p = curr->second;
    }
    return curr;
}