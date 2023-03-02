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
    int v;
    int d;
    int t;
    int next;
    int last;
    Edge(int _v, int _d, int _t) : v(_v), d(_d), t(_t) {};
};

struct Neighbor {
    int v;
    int t;
    int d;
    Neighbor(int _v, int _d, int _t): v(_v), d(_d), t(_t) {};
};

// Index label entries
struct Label {
    int d;
    int t;
    Label(int _d, int _t): d(_d), t(_t) {};

    bool operator< (const Label &x) const {
        return d < x.d;
    }
};

// Triplet in priority queue Q
struct Triplet {
    int v;
    int d;
    int t;
    Triplet(int _v, int _d, int _t): v(_v), d(_d), t(_t) {};

    bool operator< (const Triplet &x) const {   // order by smallest d, then latest t
        return (d > x.d) || (d == x.d && t < x.t) || (d == x.d && t == x.t && v > x.v);
    }
};

// Quadraplet for batch processing T
struct Quad {
    int u;
    int v;
    int d;
    int t;
    Quad(int _u, int _v, int _d, int _t): u(_u), v(_v), d(_d), t(_t) {};

    bool operator< (const Quad &x) const {   // order by smallest d, then latest t
        return (d > x.d) || (d == x.d && t < x.t) || (d == x.d && t == x.t && u > x.u) || (d == x.d && t == x.t && u == x.u && v > x.v);
    }
};

class Graph{
  public:
    int max_v, max_e;
    int n, m, t_max, landmark, window;

    // directed or undirected
    bool directed;

    // graph (edge list)
    vector<Edge> edges;
    vector<int> graph_head;

    // degrees
    vector<int> d_in;
    vector<int> d_out;

    // vertex ID <-> vertex order (based on degree)
    vector<int> ID_order;
    vector<int> order_ID;

    // index construction
    priority_queue<Triplet> Q;

    // batch label insert
    priority_queue<Quad> T;

    // landmark index:
    vector<vector<vector<Label>>> index;

    Graph(string graph_file, string directed);
    void sort_by_degree();

    // graph construction
    void insert_edge(int u, int v, int d, int t);
    void insert(int u, int v, int d, int t, int m);

    // display
    void print_edge_list();
    void print_vertex_order();
    void print_index();
    void print_graph_head();
    void print_T();
    
    // query 
    int span_distance(int u, int v, int t);

    // index construction
    void set_graph_detail(string input_file);
    void construct();
    void construct_for_a_vertex(int u);
    void add_label(int u, int v, int d, int t);

    // dynamic graph
    void insert_stream(string graph_file);
    void construct_for_a_vertex_stream(int u);
    void check_dominate(int u, int v, int d, int t);
    void add_label_update(int u, int v, int d, int t);

    // tests
    void test_correctness();
    void calculate_landmark_size();

    // online
    int temporal_dijkstra(int u, int v, int t1, int t2);
    int min_distance(vector<int>&, vector<bool>&);
};