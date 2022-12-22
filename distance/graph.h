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
    int d;
    int t;
    Edge(int _u, int _v, int _d, int _t) : u(_u), v(_v), d(_d), t(_t) {};
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
    int t1;
    int t2;
    Label(int _d, int _t1, int _t2): d(_d), t1(_t1), t2(_t2){};
    
    bool operator< (const Label &x) const {
        return d < x.d;
    }
};

// Quadruplet in priority queue Q
struct Quad {
    int v;
    int d;
    int t1;
    int t2;
    Quad(int _v, int _d, int _t1, int _t2): v(_v), d(_d), t1(_t1), t2(_t2){};
    
    bool operator< (const Quad &x) const {  // In Priority Queue, order by d, then te-ts, then v (all in increasing order)
        return (d > x.d) || (d == x.d && t2 - t1 > x.t2 - x.t1) || (d == x.d && t2 - t1 == x.t2 - x.t1 && v > x.v);
    }
};

class Graph {
  public:
    // number of vertices and number of edges
    int n, m, t_min, t_max, d_min, d_max, landmark;

    // directed or undirected graph
    bool directed;

    // edge list (ordered by time)
    vector<Edge> edges;

    // graph and reversed graph
    vector<vector<Neighbor> > graph;
    vector<vector<Neighbor> > r_graph;

    // in-degree and out-degree
    vector<int> in_degree;
    vector<int> out_degree;

    // vertex ID <-> vertex order (based on degree)
    vector<int> ID_order;
    vector<int> order_ID;

    // index
    vector<map<int, vector<Label>>> in_label;
    vector<map<int, vector<Label>>> out_label;

    // index construction
    priority_queue<Quad> Q;
    unordered_map<int, vector<Edge>> P;

    // landmark index
    vector<vector<vector<Label>>> index;

    Graph(string graph_file, string directed);
    void sort_by_degree(vector<vector<Neighbor> > &g);

    // display
    void print_graph();
    void print_r_graph();
    void print_labels();
    void print_edge_list();
    void print_vertex_order();
    void print_index();

    // index construction
    void construct();
    void construct_for_a_vertex(vector<vector<Neighbor> > &g, vector<map<int, vector<Label>>> &label, int u, bool reverse);
    void add_label(vector<map<int, vector<Label>>> &label, int u, int v, int d, int t1, int t2);

    // query
    int span_distance(int u, int v, int t1, int t2);
    int span_distance_landmark(int u, int v, int t1, int t2);

    // online
    int temporal_dijkstra(int u, int v, int t1, int t2);
    int min_distance(vector<int>&, vector<bool>&);

    // tests
    void test_correctness();
    void calculate_index_size();
    void calculate_landmark_size();
    void test_landmark_correctness();

    // landmark index
    void set_landmark(string input_file);
    void construct_landmark();
    void construct_for_a_vertex_landmark(int u);
    void add_label_landmark(int u, int v, int d, int t1, int t2);
};