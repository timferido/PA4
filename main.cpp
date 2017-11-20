/*
Author: Timothy Ferido, Kent Nguyen
Date: 11-16-17
File: main.cpp
Description: This file is the main executable program
*/

#include <iostream>
#include "ActorGraph.h"

using namespace std;

int main(int argc, char* argv[]) {
    cout << "before everything" << endl;

    //load from file
    ActorGraph graph;
    bool test = graph.loadFromFile(argv[1], false);

    //test search
    auto range = graph.movieMap.equal_range("GONE GIRL   2014");
    cout << "before the loop" << endl;
    for (auto it = range.first; it != range.second; ++it) {
        cout << it->first << ' ' << it->second << '\n';
        cout << "in the loop" <<endl;
    }
    cout << " after the loop" << endl;

    return 0;
}

