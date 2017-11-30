#include <unordered_map>
#include <string>
#include <algorithm>

using namespace std;

#define NFNTY   2147483647

class Node {
public:
    unordered_map<string,string> adj;
    string actorName;
    int dist;
    int index;
    Node* prev;
    bool done;

    ///comparison class
    bool operator<(const Node& other);
    
    public:
        Node(string actor) : actorName(actor), dist(32767), index(0), prev(0), done(false) {};
	    // ~Node();
};
