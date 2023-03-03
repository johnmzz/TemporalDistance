#include "graph.h"

void Graph::set_graph_detail(string file) {
    if (file == "graph") {
        n = 12;
        t_max = 5;
        landmark = 12;
    }
    if (file == "graph_p1") {
        n = 12;
        t_max = 5;
        landmark = 12;
    }
    if (file == "CollegeMsg_1") {
        n = 1899;
        t_max = 194;
        window = 100;
        landmark = 100;
    }
    if (file == "CollegeMsg_10") {
        n = 1899;
        t_max = 194;
        window = 100;
        landmark = 100;
    }
    if (file == "CollegeMsg_10_p1") {
        n = 1899;
        t_max = 194;
        window = 100;
        landmark = 100;
    }
    if (file == "CollegeMsg_100") {
        n = 1899;
        t_max = 194;
        window = 100;
        landmark = 100;
    }
    if (file == "mathoverflow_10") {
        n = 24818;
        t_max = 101;
        window = 100;
        landmark = 1;
    }
    if (file == "mathoverflow_10_p1") {
        n = 24818;
        t_max = 2351;
        window = 100;
        landmark = 1;
    }
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
        d_out[u]++;
        if (directed) d_in[v]++;    // TODO: if undirected d_out[v]++;
        insert_edge(u, v, d, t);
    }
    
    sort_by_degree();

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

void Graph::sort_by_degree() {
    vector<pair<int, int>> v_degree_ordered(n+1);
    for (uint32_t i = 1; i <= n; i++) {
        v_degree_ordered[i] = make_pair(i, (d_in[i] + 1) * (d_out[i] + 1));
    }
    std::sort(v_degree_ordered.begin(), v_degree_ordered.end(), [](auto &left, auto &right) {return left.second > right.second;});

    for (int i = 0; i < v_degree_ordered.size(); i++) {
        auto v = v_degree_ordered[i];
        ID_order[v.first] = i+1;
        order_ID[i+1] = v.first;
    }
}

void Graph::insert_stream(string input_file) {
    cout << "reading new edges..." << endl;
    ifstream fin("./data/" + input_file + ".txt");
    if (!fin) {
        cout << "Cannot open " << input_file << ".txt" << endl;
        exit(0);
    }

    int w, v, d, t;
    while (fin >> w >> v >> d >> t) {
        d_out[w]++;
        if (directed) d_in[v]++;    // TODO: if undirected d_out[v]++;
        insert_edge(w, v, d, t);

        for (int i = 1; i <= landmark; i++) {
            int u = order_ID[i];
            if (u == w) {
                T.push(Quad(u, v, d, t));
            }
            else {
                for (auto label : index[u][w]) {
                    T.push(Quad(u, v, label.d + d, label.t));
                }
            }
        }
    }

    clock_t start, end;
    start = clock();
    while (!T.empty()) {
        Quad quad = T.top();
        T.pop();

        Q.push(Triplet(quad.v, quad.d, quad.t));
        construct_for_a_vertex_stream(quad.u);
    }
    end = clock();
    cout << "insert stream: " << (float)(end - start) / CLOCKS_PER_SEC << " s" << endl;
}

void Graph::construct_for_a_vertex_stream(int u) {
    //int i = 0;
    while (!Q.empty()) {
        Triplet trip = Q.top();
        //cout << i << " pop: (v=" << trip.v << ", d=" << trip.d << ", t=" << trip.t << ")" << endl;
        Q.pop();
        int v = trip.v;
        if (u != v) {
            if (span_distance(u, v, trip.t) <= trip.d) {
                continue;
            }
            else {
                //cout << "insert (u = " << u << ", v = " << v << ", d = " << trip.d << ", ts = " << trip.t << ")" << endl;
                check_dominate(u, v, trip.d, trip.t);
                add_label_update(u, v, trip.d, trip.t);
                //cout << i << " add label: Index[" << u << "][" << v <<  "] += (" << trip.d << "," << trip.t << ")" << endl; 
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
            //cout << i << " push: (v=" << edges[j].v << ", d=" << _d << ", t=" << _t << ")" << endl;
        }
        //i++;
        //cout << endl;
    }
}

void Graph::add_label_update(int u, int v, int d, int t) {
	Label label = Label(d, t);
	vector<Label>::iterator it = upper_bound(index[u][v].begin(), index[u][v].end(), label);
	index[u][v].insert(it, label);
}

void Graph::check_dominate(int u, int v, int d, int t) {

    for (int i = 0; i < index[u][v].size(); i++) {
        Label label = index[u][v][i];
        if (t >= label.t && d <= label.d) {
            index[u][v].erase(index[u][v].begin() + i);
            i--;
        }
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
    vector<int> dist = vector<int>(n+1, INT_MAX);
    vector<bool> visited = vector<bool>(n+1, false);

    dist[s] = 0;

    while (!visited[t]) {
        int v = min_distance(dist, visited);
        //cout << "this round v = " << v << endl;
        
        if (v == INT_MAX) break;

        visited[v] = true;
        // for (auto e : graph[v]) {
        //     if (!visited[e.v] && dist[v]+e.d < dist[e.v] && e.t >= t1 && e.t <= t2) {
        //         dist[e.v] = dist[v] + e.d;
        //     }
        // }
        for (int j = graph_head[v]; j; j = edges[j].next) { 
            Edge e = edges[j];
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
    //int i = 0;
    while (!Q.empty()) {
        Triplet trip = Q.top();
        //cout << i << " pop: (v=" << trip.v << ", d=" << trip.d << ", t=" << trip.t << ")" << endl;
        Q.pop();
        int v = trip.v;
        if (u != v) {
            if (span_distance(u, v, trip.t) <= trip.d) {
                continue;
            }
            else {
                add_label(u, v, trip.d, trip.t);
                //cout << i << " add label: Index[" << u << "][" << v <<  "] += (" << trip.d << "," << trip.t << ")" << endl; 
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
            //cout << i << " push: (v=" << edges[j].v << ", d=" << _d << ", t=" << _t << ")" << endl;
        }
        //i++;
        //cout << endl;
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
                cout << "(d=" << label.d << ",ts=" << label.t << "),";
            }
            cout << "], ";
        }
        cout << "}" << endl;
    }
}

void Graph::print_T() {
    cout << "T:" << endl;
    while (!T.empty()) {
        Quad quad = T.top();
        T.pop();
        cout << "(u = " << quad.u << ", v = " << quad.v << ", d = " << quad.d << ", ts = " << quad.t << "), " << endl;
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

void Graph::test_correctness() {
    cout << "testing corrrectness..." << endl;
    for (int i = 1; i <= landmark; i++) {
        for (int j = 1; j <= landmark; j++) {
            for (int ts = 1; ts <= t_max; ts++) {
                cout << "testing i = " << i << ", j = " << j << ", ts = " << ts << endl;
                int u = order_ID[i];
                int v = order_ID[j];
                int te = t_max;
                int query = span_distance(u,v,ts);
                int online = temporal_dijkstra(u,v,ts,te);
                if (query != online) {
                    cout << "u = " << u << ", v = " << v << ",ts = " << ts << ", te = " << te
                    << ", Incorrect query = " << query << ", online = " << online << endl;
                }
            }
        }
    }
}