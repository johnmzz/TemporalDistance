#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <string>
#include <cstring>
#include <vector>
#include <stack>
#include <queue>
#include <map>
#include <set>
#include <algorithm>
#include <ctime>
#include <cfloat>
#include <numeric>
#include <climits>
#include <chrono>
#include <unordered_map>

using namespace std;

struct Edge {
    int u;
    int v;
    int8_t d;
    int16_t t;
    Edge(int _u, int _v, int8_t _d, int16_t _t) : u(_u), v(_v), d(_d), t(_t) {};
};

struct Neighbor {
    int v;
    int8_t d;
    int16_t t;
    Neighbor(int _v, int8_t _d, int16_t _t): v(_v), d(_d), t(_t) {};
};

// Index label entries
struct Label {
    int8_t d;
    int16_t t;
    Label(int8_t _d, int16_t _t): d(_d), t(_t) {};

    bool operator< (const Label &x) const {
        return d < x.d;
    }
};

// Triplet in priority queue Q
struct Triplet {
    int v;
    int8_t d;
    int16_t t;
    Triplet(int _v, int8_t _d, int16_t _t): v(_v), d(_d), t(_t) {};

    bool operator< (const Triplet &x) const {   // order by smallest d, then latest t
        return (d > x.d) || (d == x.d && t < x.t) || (d == x.d && t == x.t && v > x.v);
    }
};

class Graph{
  public:
    int n, m, t_min, t_max, d_min, d_max, landmark;

    // directed or undirected
    bool directed;

    // edge list (ordered by time)
    vector<Edge> edges;

    // graph
    vector<vector<Neighbor>> graph;
    //vector<vector<Neighbor>> r_graph;

    // in-degree and out-degree
    vector<int> in_degree;
    vector<int> out_degree;

    // vertex ID <-> vertex order (based on degree)
    vector<int> ID_order;
    vector<int> order_ID;

    // index construction
    priority_queue<Triplet> Q;

    // landmark index:
    vector<vector<vector<Label>>> index;

    Graph(string graph_file, string directed);
    void sort_by_degree(vector<vector<Neighbor>> &g);

    // display
    void print_graph();
    void print_edge_list();
    void print_vertex_order();
    void print_index();
    
    // query 
    int span_distance(int u, int v, int16_t t);

    // index construction
    void set_landmark(string input_file);
    void construct();
    void construct_for_a_vertex(int u);
    void add_label(int u, int v, int8_t d, int16_t t);

    // tests
    void test_correctness();
    void calculate_landmark_size();

    // online
    int temporal_dijkstra(int u, int v, int t1, int t2);
    int min_distance(vector<int>&, vector<bool>&);
};