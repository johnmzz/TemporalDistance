#include "graph.h"

// triplet in TILL-Index
struct Triplet {
    int v;
    int t1, t2;
    Triplet(int _v, int _t1, int _t2): v(_v), t1(_t1), t2(_t2) {};
    bool operator< (const Triplet &x) const {
        return v < x.v;
    }
};

// triplet in priority queue Q
struct Tri {
    int v;
    int t1, t2;
    Tri(int _v, int _t1, int _t2): v(_v), t1(_t1), t2(_t2) {};
    bool operator< (const Tri &x) const {
        return t2 - t1 > x.t2 - x.t1 || (t2 - t1 == x.t2 - x.t1 && v > x.v);
    }
};

struct State {
    int u, v;
    int t1, t2;
    State(int _u, int _v, int _t1, int _t2): u(_u), v(_v), t1(_t1), t2(_t2) {};
    bool operator< (const State &x) const {
        return min(u, v) < min(x.u, x.v) || (min(u, v) == min(x.u, x.v) && u < x.u);
    }
};

struct Edge {
    int u, v;
    int ts;
    Edge(int _u, int _v, int _ts): u(_u), v(_v), ts(_ts) {};
};

class TILL {
  private:
  // the number of vertices and the number of edges
    int max_v, max_e; // set them in "graph.cpp"
    int n, m;

    bool directed; // directed graph of undirected graph

    // graph and reversed graph
    GraphEdge **graph_head, **graph_r_head;
    GraphEdge *graph_edge, *graph_r_edge;

    // in-degree and out-degree
    int *d_in, *d_out;

    int *idx; // order --> ID
    int *order; // ID --> order

    map<int, int> *vertex_map;

    int theta; // set it in "till.cpp"

    // ITLL-Index
    vector<Triplet> *in_label, *out_label;

    priority_queue<Tri> Q;
    vector<State> P;

    void add_triplet(vector<Triplet> *lable, int u, int v, int t1, int t2, bool update);
    void construct_for_a_vertex(GraphEdge **head, vector<Triplet> *lable, int u, bool in, bool update);
    
 public:
    vector<Edge> S;
    TILL(Graph *graph);
    ~TILL();

    void construct();
    bool span_reach(int u, int v, int t1, int t2, bool original_id = false);

    void insert(int ts);
    void erase(int ts);
};