#include <unordered_map>
#include <string>
#include <algorithm>
#include <utility>

using namespace std;


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
