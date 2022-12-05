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

using namespace std;

struct Edge {
    int v;
    int t;
    int d;
    //Edge *last, *next;
    Edge(int _v, int _t, int _d): v(_v), t(_t), d(_d) {};
};

// Index label entries
struct Label {
    int v;
    int d;
    int t1;
    int t2;
    Label(int _v, int _d, int _t1, int _t2): v(_v), t1(_t1), t2(_t2), d(_d) {};
    
    bool operator< (const Label &x) const {
        return v < x.v;
    }
};

// Quadruplet in priority queue Q
struct Quad {
    int v;
    int d;
    int t1;
    int t2;
    Quad(int _v, int _d, int _t1, int _t2): v(_v), d(_d), t1(_t1), t2(_t2){};
    
    bool operator< (const Quad &x) const {
        return (d > x.d) || (d == x.d && t2 - t1 > x.t2 - x.t1) || (d == x.d && t2 - t1 == x.t2 - x.t1 && v > x.v);
    }
};

class Graph {
  public:
    // number of vertices and number of edges
    int n, m, t_min, t_max, d_min, d_max;

    // directed or undirected graph
    bool directed;

    // graph and reversed graph
    vector<vector<Edge> > graph;
    vector<vector<Edge> > r_graph;

    // in-degree and out-degree
    vector<int> in_degree;
    vector<int> out_degree;

    // map original ID -> new ID based on appearance
    map<int, int> vmap_org_new;
    map<int, int> vmap_new_org;

    // index
    vector<vector<Label> > in_label;
    vector<vector<Label> > out_label;

    // index construction
    priority_queue<Quad> Q;

    Graph(string graph_file, string directed);
    void sort_by_degree(vector<vector<Edge> > &g);

    // display
    void print_graph();
    void print_r_graph();
    void print_labels();

    // index construction
    void construct();
    void construct_for_a_vertex(vector<vector<Edge> > &g, vector<vector<Label> > &label, int u, bool reverse);
    void add_label(vector<vector<Label> > &label, int u, int v, int d, int t1, int t2);

    // query
    int span_distance(int u, int v, int t1, int t2);

    // online
    int temporal_dijkstra(int u, int v, int t1, int t2);
    int min_distance(vector<int>&, vector<bool>&);

    // tests
    void test_correctness();
    void calculate_index_size();
};