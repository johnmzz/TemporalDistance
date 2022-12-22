#include "graph.h"

Graph::Graph(string input_file, string _directed) {
    n = 0;
    m = 0;
    d_min = INT_MAX;
    d_max = 0;
    t_min = INT_MAX;
    t_max = 0;

    set_landmark(input_file);
    
    // directed or undirected
    if (_directed == "U") 
        directed = false;
    else if (_directed == "D")
        directed = true;
    else {
        cout << "Incorrect input!" << endl;
        exit(0);
    }

    // get number of vertices and edges
    ifstream fin("./data/" + input_file + ".txt");
    if (!fin) {
        cout << "Cannot open " << input_file << ".txt" << endl;
        exit(0);
    }
    int u, v, d, t;
    while (fin >> u >> v >> d >> t) {
        n = max(u,n);
        n = max(v,n);
        d_min = min(d, d_min);
        d_max = max(d, d_max);
        t_min = min(t, t_min);
        t_max = max(t, t_max);
        m++;
    }
    fin.close();

    // initialize adj_list, degree
    graph = vector<vector<Neighbor> >(n+1, vector<Neighbor>());
    out_degree = vector<int>(n+1);
    if (directed) {
        r_graph = vector<vector<Neighbor> >(n+1, vector<Neighbor>());
        in_degree = vector<int>(n+1);
    }
    ID_order = vector<int>(n+1);
    order_ID = vector<int>(n+1);

    // read graph
    fin.open("./data/" + input_file + ".txt");
    while (fin >> u >> v >> d >> t) {
        graph[u].push_back(Neighbor(v, d, t));
        out_degree[u]++;

        if (directed) {
            r_graph[v].push_back(Neighbor(u, d, t));
            in_degree[v]++;
        }
        else {
            graph[v].push_back(Neighbor(u, d, t));
            out_degree[v]++;
        }

        edges.push_back(Edge(u, v, d, t));
    }
    if (!directed) in_degree = out_degree;
    
    sort_by_degree(graph);

    // initialize index
    in_label = vector<map<int,vector<Label>>>(n+1, map<int, vector<Label> >());
    out_label = vector<map<int,vector<Label>>>(n+1, map<int, vector<Label> >());
    index = vector<vector<vector<Label>>>(n+1, vector<vector<Label>>(n+1, vector<Label>()));

    cout << "Graph contructed, n = " << n << ", m = " << m << ", d_max = " << d_max << ", t_max = " << t_max << endl;
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

    // vector<int> offset(g.size());
    // for (int i = 0; i < g.size(); i++) {
    //     offset[v_degree_ordered[i].first] = i+1;
    // }

    // vector<vector<Neighbor> > new_graph = vector<vector<Neighbor> > (n+1, vector<Neighbor>());
    // for (int i = 1; i <= n; i++) {
    //     int u = v_degree_ordered[i-1].first;
    //     for (int j = 0; j < g[u].size(); j++) {
    //         int v = offset[g[u][j].v];
    //         int d = g[u][j].d;
    //         int t = g[u][j].t;
    //         new_graph[i].push_back(Neighbor(v,t,d));
    //     }
    // }
    // g = new_graph;
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

void Graph::print_r_graph() {
    for (int i = 0; i < r_graph.size(); i++) {
        cout << i << ":[";
        for (auto j : r_graph[i]) {
            cout << "(" << j.v << "," << j.d << "," << j.t << "),";
        }
        cout << endl;
    }
}

void Graph::print_edge_list() {
    for (auto e : edges) {
        cout << "(" << e.u << "," << e.v << "," << e.d << "," << e.t << ")" << endl;
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

void Graph::print_labels() {
    cout << "out_label = " << endl;
    for (int i = 0; i < out_label.size(); i++){
        cout << i << ": [";
        for (auto it = out_label[i].begin(); it != out_label[i].end(); it++) {
            cout << "{" << it->first + 1 << ": [";
            for (auto label : it->second) {
                cout << "(" << label.d << "," << label.t1 << "," << label.t2 << "), ";
            }
            cout << "]}, ";
        }
        cout << "]," << endl;
    }

    cout << "in_label = " << endl;
    for (int i = 0; i < in_label.size(); i++){
        cout << i << ": [";
        for (auto it = in_label[i].begin(); it != in_label[i].end(); it++) {
            cout << "{" << it->first + 1 << ": [";
            for (auto label : it->second) {
                cout << "(" << label.d << "," << label.t1 << "," << label.t2 << "), ";
            }
            cout << "]}, ";
        }
        cout << "]," << endl;
    }
}

int Graph::span_distance(int u, int v, int t1, int t2) {
    if (u == v) return 0;
    int min_distance = INT_MAX;

    for (auto label : out_label[u][v]) {
        if (t1 <= label.t1 && label.t2 <= t2 && label.d < min_distance) {
            min_distance = label.d;
            break;
        }
    }

    for (auto label : in_label[v][u]) {
        if (t1 <= label.t1 && label.t2 <= t2 && label.d < min_distance) {
            min_distance = label.d;
            break;
        }
    }

    for (map<int,vector<Label> >::iterator it1 = out_label[u].begin(), it2 = in_label[v].begin(); it1 != out_label[u].end() && it2 != in_label[v].end();) {
        if (it1->first < it2->first) it1++;
        else if (it1->first > it2->first) it2++;
        else {
            for (vector<Label>::iterator i = it1->second.begin(), j = it2->second.begin(); i != it1->second.end() && j != it2->second.end();) {
                if (!(t1 <= i -> t1 && i -> t2 <= t2)) i++;
                else if (!(t1 <= j -> t1 && j -> t2 <= t2)) j++;
                else {
                    if (i -> d + j -> d < min_distance) min_distance = i -> d + j -> d;
                    i++;
                    j++;
                }
            }
            it1++;
            it2++;
        }
    }
    return min_distance;
}

void Graph::construct() {
    clock_t start, end;
    start = clock();

    // TODO: construct new Q every time
    for (int i = 1; i <= n; i++) {
        clock_t start_i, end_i;
        start_i = clock();

        int u = order_ID[i];
        cout << "construct for vertex u = " << u << ", order i = " << i;
        Q.push(Quad(u,0,-1,-1));
        construct_for_a_vertex(graph, in_label, u, false);

        if (directed) {
            Q.push(Quad(u,0,-1,-1));
            construct_for_a_vertex(r_graph, out_label, u, true);
        }
        end_i = clock();
        cout << ", time taken = " << (float)(end_i - start_i) / CLOCKS_PER_SEC << endl;
        //cout << ", finished!" << endl;
    }

    end = clock();
    cout << "build index: " << (float)(end - start) / CLOCKS_PER_SEC << " s" << endl;
}

void Graph::construct_for_a_vertex(vector<vector<Neighbor> > &g, vector<map<int,vector<Label>>> &label, int u, bool reverse) {
    //int i = 0;
    while (!Q.empty()) {
        Quad quad = Q.top();
        //cout << i << " pop: (v=" << quad.v << ", d=" << quad.d << ", t1=" << quad.t1 << ", t2=" << quad.t2 << ")" << endl;
        Q.pop();
        int v = quad.v;
        if (u != v) {
            if ((!reverse && span_distance(u, v, quad.t1, quad.t2) <= quad.d) || (reverse && span_distance(v, u, quad.t1, quad.t2) <= quad.d)) {
                continue;
            }
            else {
                add_label(label, v, u, quad.d, quad.t1, quad.t2);
                //cout << i << " add label: L(" << v << ") += (u=" << u << ", d=" << quad.d << ", t1=" << quad.t1 << ", t2=" << quad.t2 << ")" << endl;
            }
        }
        for (Neighbor e : g[v]) { 
            if (ID_order[u] < ID_order[e.v]) {
                int _t1 = (quad.t1 == -1? e.t : min(quad.t1, e.t));
                int _t2 = (quad.t2 == -1? e.t : max(quad.t2, e.t));
                int _d = quad.d + e.d;

                // if use max_d_from_u[v], then need to specify (v, t1, t2), maybe use nested hash
                Q.push(Quad(e.v, _d, _t1, _t2));
                //cout << i << " push: (v=" << e.v << ", d=" << _d << ", t1=" << _t1 << ", t2=" << _t2 << ")" << endl;
            }
        }
        // i++;
        // cout << endl;
    }
}

void Graph::add_label(vector<map<int,vector<Label>>> &label, int u, int v, int d, int t1, int t2) {
    label[u][v].push_back(Label(d,t1,t2));
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

void Graph::test_correctness() {
    cout << "testing corrrectness..." << endl;
    for (int u = 1; u <= n; u++) {
        for (int v = 1; v <= n; v++) {
            for (int ts = t_min; ts <= t_max; ts++) {
                for (int te = t_min; te <= t_max; te++) {
                    int query = span_distance(u,v,ts,te);
                    int online = temporal_dijkstra(u,v,ts,te);
                    if (query != online) {
                        cout << "u = " << u << ", v = " << v << ",ts = " << ts << ", te = " << te
                        << ", Incorrect query = " << query << ", online = " << online << endl;
                    }
                }
            }
        }
    }
}

void Graph::calculate_index_size() {
    int num_label = 0;

    for (int i = 0; i < out_label.size(); i++){
        for (auto it = out_label[i].begin(); it != out_label[i].end(); it++) {
            for (auto label : it->second) {
                num_label++;
            }
        }
    }

    if (directed) {
        for (int i = 0; i < in_label.size(); i++){
            for (auto it = in_label[i].begin(); it != in_label[i].end(); it++) {
                for (auto label : it->second) {
                    num_label++;
                }
            }
        }
    }

    cout << "num_label = " << num_label << endl;
}

void Graph::set_landmark(string file) {
    if (file == "graph") landmark = 6;
    if (file == "synthetic_100") landmark = 10;
    if (file == "CollegeMsg_1") landmark = 100;
}

int Graph::span_distance_landmark(int u, int v, int t1, int t2) {
    if (u == v) return 0;

    for (auto label: index[u][v]) {
        if (t1 <= label.t1 && label.t2 <= t2) {
            return label.d;
        }
    }
    return INT_MAX;
}

void Graph::construct_landmark(){
    clock_t start, end;
    start = clock();

    for (int i = 1; i <= landmark; i++) {
        clock_t start_i, end_i;
        start_i = clock();

        int u = order_ID[i];
        cout << "construct for vertex u = " << u << ", order i = " << i << endl;
        Q.push(Quad(u,0,-1,-1));
        construct_for_a_vertex_landmark(u);

        end_i = clock();
        cout << ", time taken = " << (float)(end_i - start_i) / CLOCKS_PER_SEC << endl;
    }
    end = clock();
    cout << "build index: " << (float)(end - start) / CLOCKS_PER_SEC << " s" << endl;
}

void Graph::construct_for_a_vertex_landmark(int u) {
    //int i = 0;
    while (!Q.empty()) {
        Quad quad = Q.top();
        //cout << i << " pop: (v=" << quad.v << ", d=" << quad.d << ", t1=" << quad.t1 << ", t2=" << quad.t2 << ")" << endl;
        Q.pop();
        int v = quad.v;
        if (u != v) {
            if (span_distance_landmark(u, v, quad.t1, quad.t2) <= quad.d) {
                continue;
            }
            else {
                add_label_landmark(u, v, quad.d, quad.t1, quad.t2);
                //cout << i << " add label: Index[" << u << "][" << v <<  "] += (" << quad.d << "," << quad.t1 << "," << quad.t2 << ")" << endl; 
            }
        }
        for (Neighbor e : graph[v]) {
            int _t1 = (quad.t1 == -1? e.t : min(quad.t1, e.t));
            int _t2 = (quad.t2 == -1? e.t : max(quad.t2, e.t));
            int _d = quad.d + e.d;

            // if use max_d_from_u[v], then need to specify (v, t1, t2), maybe use nested hash
            Q.push(Quad(e.v, _d, _t1, _t2));
            //cout << i << " push: (v=" << e.v << ", d=" << _d << ", t1=" << _t1 << ", t2=" << _t2 << ")" << endl;
        }
        //i++;
        //cout << endl;
    }
}

void Graph::add_label_landmark(int u, int v, int d, int t1, int t2) {
    index[u][v].push_back(Label(d,t1,t2));
}

void Graph::print_index() {
    cout << "Index = " << endl;
    for (int i = 1; i < index.size(); i++) {
        cout << i << ": {";
        for (int j = 1; j < index[i].size(); j++) {
            cout << j << ":[";
            for (auto label: index[i][j]) {
                cout << "(" << label.d << "," << label.t1 << "," << label.t2 << "),";
            }
            cout << "], ";
        }
        cout << "}" << endl;
    }
}

void Graph::test_landmark_correctness() {
    cout << "testing corrrectness..." << endl;
    for (int i = 1; i <= landmark; i++) {
        for (int j = 1; j <= landmark; j++) {
            for (int ts = t_min; ts <= t_max; ts++) {
                for (int te = t_min; te <= t_max; te++) {
                    int u = order_ID[i];
                    int v = order_ID[j];
                    int query = span_distance_landmark(u,v,ts,te);
                    int online = temporal_dijkstra(u,v,ts,te);
                    if (query != online) {
                        cout << "u = " << u << ", v = " << v << ",ts = " << ts << ", te = " << te
                        << ", Incorrect query = " << query << ", online = " << online << endl;
                    }
                }
            }
        }
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

    cout << "num_labels = " << num_label << ", size = num_labels * 3 * 4 = " << num_label * 3 * sizeof(int) << endl;
}