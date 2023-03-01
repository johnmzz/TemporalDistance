#include "graph.h"

void Graph::set_graph_detail(string file) {
    if (file == "graph") {
        n = 12;
        t_max = 6;
        landmark = 12;
    }
    if (file == "synthetic_100") landmark = 10;
    if (file == "CollegeMsg_1") landmark = 100;
    if (file == "email_1") landmark = 1;
    if (file == "chess_2") landmark = 300;
    if (file == "slashdot_1") landmark = 1000;
    if (file == "mathoverflow_10days") landmark = 1;
    if (file == "mathoverflow_1month") landmark = 1;
}

Graph::Graph(string input_file, string _directed) {
	max_v = 10000000;
	max_e = 100000000;

    set_graph_detail(input_file);
    m = 0;

    // directed or undirected
    if (_directed == "U")
        directed = false;
    else if (_directed == "D")
        directed = true;
    else {
        cout << "Incorrect input!" << endl;
        exit(0);
    }

    // initialize adj_list, degree
    graph = vector<vector<Neighbor> >(n+1, vector<Neighbor>());
    out_degree = vector<int>(n+1);
    if (directed) {
        in_degree = vector<int>(n+1);
    }
    ID_order = vector<int>(n+1);
    order_ID = vector<int>(n+1);

    graph_head = vector<int>(n+1, 0);
    edges = vector<Edge>(max_e, Edge(0, 0, 0));
    d_out = vector<int>(n+1);
    d_in = vector<int>(n+1);

    // read graph
    cout << "reading graph..." << endl;
    ifstream fin("./data/" + input_file + ".txt");
    if (!fin) {
        cout << "Cannot open " << input_file << ".txt" << endl;
        exit(0);
    }
    int u, v, d, t;
    while (fin >> u >> v >> d >> t) {
        graph[u].push_back(Neighbor(v, d, t));
        out_degree[u]++;

        if (directed) {
            in_degree[v]++;
        }
        else {
            graph[v].push_back(Neighbor(u, d, t));
            out_degree[v]++;
        }

        d_out[u]++;
        if (directed) d_in[v]++;    // TODO: if undirected d_out[v]++;
        insert_edge(u, v, d, t);
    }
    if (!directed) in_degree = out_degree;
    
    sort_by_degree(graph);

    index = vector<vector<vector<Label>>>(n+1, vector<vector<Label>>(n+1, vector<Label>()));
    cout << "Graph contructed, n = " << n << ", m = " << m << ", t_max = " << t_max << endl;
}

void Graph::insert(int u, int v, int d, int t, int eid) {
    edges[eid].v = v;
    edges[eid].d = d;
    edges[eid].t = t;
    edges[eid].next = graph_head[u];
    edges[eid].last = 0;

    if (graph_head[u])
        edges[graph_head[u]].last = eid;
    graph_head[u] = eid;    
}

void Graph::insert_edge(int u, int v, int d, int t) {
    m++;
    insert(u, v, d, t, m);

    if (!directed) {
        // TODO: insertfor v, u
    }
}

void Graph::sort_by_degree(vector<vector<Neighbor> > &g) {
    vector<pair<int, int>> v_degree_ordered(g.size());
    for (uint32_t i = 1; i < g.size(); i++) {
        v_degree_ordered[i] = make_pair(i, (in_degree[i] + 1) * (out_degree[i] + 1));
    }
    std::sort(v_degree_ordered.begin(), v_degree_ordered.end(), [](auto &left, auto &right) {return left.second > right.second;});

    for (int i = 0; i < v_degree_ordered.size(); i++) {
        auto v = v_degree_ordered[i];
        ID_order[v.first] = i+1;
        order_ID[i+1] = v.first;
    }
}

void Graph::print_graph() {
    for (int i = 0; i < graph.size(); i++) {
        cout << i << ":[";
        for (auto j : graph[i]) {
            cout << "(" << j.v << "," << j.d << "," << j.t << "),";
        }
        cout << endl;
    }
}

void Graph::print_graph_head() {
    for (int i = 0; i <= n; i++) {
        cout << i << ": " << graph_head[i] << endl;
    }
}

void Graph::print_edge_list() {
    cout << "Edge list:" << endl;
    for (int i = 0; i <= m; i++) {
        cout << "eid = " << i << ", v = " << edges[i].v << ", t = " << edges[i].t;
        cout << ", d = " << edges[i].d << ", next = " << edges[i].next;
        cout << ", last = " << edges[i].last << endl;
    }
}

void Graph::print_vertex_order() {
    cout << "ID_order = " << endl;
    for (int i = 1; i < ID_order.size(); i++) {
        cout << "original ID = " << i << ", vertex order = " << ID_order[i] << endl;
    }
    cout << "order_ID = " << endl;
    for (int i = 1; i < order_ID.size(); i++) {
        cout << "vertex order = " << i << ", original ID = " << order_ID[i] << endl;
    }
}

int Graph::min_distance(vector<int>& dist, vector<bool>& visited) {
    int min_dist = INT_MAX;
    int idx = INT_MAX;
    for (int i = 0; i < dist.size(); i++) {
        if (!visited[i] && dist[i] < min_dist) {
            min_dist = dist[i];
            idx = i;
        }
    }
    return idx;
}

int Graph::temporal_dijkstra(int s, int t, int t1, int t2) {
    vector<int> dist = vector<int>(graph.size(), INT_MAX);
    vector<bool> visited = vector<bool>(graph.size(), false);

    dist[s] = 0;

    while (!visited[t]) {
        int v = min_distance(dist, visited);
        //cout << "this round v = " << v << endl;
        
        if (v == INT_MAX) break;

        visited[v] = true;
        for (auto e : graph[v]) {
            if (!visited[e.v] && dist[v]+e.d < dist[e.v] && e.t >= t1 && e.t <= t2) {
                dist[e.v] = dist[v] + e.d;
            }
        }

    }
    return dist[t];
}

int Graph::span_distance(int u, int v, int t) {
    if (u == v) return 0;

    for (auto label: index[u][v]) {
        if (t <= label.t) {
            return label.d;
        }
    }
    return INT_MAX;
}

void Graph::construct() {
    clock_t start, end;
    start = clock();

    for (int i = 1; i <= landmark; i++) {
        clock_t start_i, end_i;
        start_i = clock();

        int u = order_ID[i];
        cout << "construct for vertex u = " << u << ", order i = " << i << endl;
        Q.push(Triplet(u, 0, -1));
        construct_for_a_vertex(u);

        end_i = clock();
        cout << ", time taken = " << (float)(end_i - start_i) / CLOCKS_PER_SEC << endl;
    }
    end = clock();
    cout << "build index: " << (float)(end - start) / CLOCKS_PER_SEC << " s" << endl;
}

void Graph::construct_for_a_vertex(int u) {
    int i = 0;
    while (!Q.empty()) {
        Triplet trip = Q.top();
        cout << i << " pop: (v=" << trip.v << ", d=" << trip.d << ", t=" << trip.t << ")" << endl;
        Q.pop();
        int v = trip.v;
        if (u != v) {
            if (span_distance(u, v, trip.t) <= trip.d) {
                continue;
            }
            else {
                add_label(u, v, trip.d, trip.t);
                cout << i << " add label: Index[" << u << "][" << v <<  "] += (" << trip.d << "," << trip.t << ")" << endl; 
            }
        }
        // for (Neighbor e : graph[v]) {
        //     int _t = (trip.t == -1 ? e.t : min(trip.t, e.t));
        //     int _d = trip.d + e.d;

        //     Q.push(Triplet(e.v, _d, _t));
        //     //cout << i << " push: (v=" << e.v << ", d=" << _d << ", t=" << _t << ")" << endl;
        // }
        for (int j = graph_head[v]; j; j = edges[j].next) {
            int _t = (trip.t == -1 ? edges[j].t : min(trip.t, edges[j].t));
            int _d = trip.d + edges[j].d;

            Q.push(Triplet(edges[j].v, _d, _t));
            cout << i << " push: (v=" << edges[j].v << ", d=" << _d << ", t=" << _t << ")" << endl;
        }
        i++;
        cout << endl;
    }
}

void Graph::add_label(int u, int v, int d, int t) {
    index[u][v].push_back(Label(d, t));
}

void Graph::print_index() {
    cout << "Index = " << endl;
    for (int i = 1; i < index.size(); i++) {
        cout << i << ": {";
        for (int j = 1; j < index[i].size(); j++) {
            cout << j << ":[";
            for (auto label: index[i][j]) {
                cout << "(" << label.d << "," << label.t << "),";
            }
            cout << "], ";
        }
        cout << "}" << endl;
    }
}

void Graph::calculate_landmark_size() {
    int num_label = 0;

    for (int i = 0; i < index.size(); i++) {
        for (int j = 0; j < index[i].size(); j++) {
            for (auto label : index[i][j]) {
                num_label++;
            }
        }
    }

    cout << "num_labels = " << num_label << ", size = num_labels * 2 * sizeof(int) = " << num_label * 2 * sizeof(int) << endl;
}

// void Graph::test_correctness() {
//     cout << "testing corrrectness..." << endl;
//     for (int i = 1; i <= landmark; i++) {
//         for (int j = 1; j <= landmark; j++) {
//             for (int ts = t_min; ts <= t_max; ts++) {
//                 int u = order_ID[i];
//                 int v = order_ID[j];
//                 int te = t_max;
//                 int query = span_distance(u,v,ts);
//                 int online = temporal_dijkstra(u,v,ts,te);
//                 if (query != online) {
//                     cout << "u = " << u << ", v = " << v << ",ts = " << ts << ", te = " << te
//                     << ", Incorrect query = " << query << ", online = " << online << endl;
//                 }
//             }
//         }
//     }
// }