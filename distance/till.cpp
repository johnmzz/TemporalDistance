#include "till.h"

TILL::TILL(Graph *graph) {
    max_v = graph -> max_v;
    max_e = graph -> max_e;
    n = graph -> n;
    m = graph -> m;
    directed = graph -> directed;

    graph_head = graph -> graph_head;
    graph_r_head = graph -> graph_r_head;
    graph_edge = graph -> graph_edge;
    graph_r_edge = graph -> graph_r_edge;
    
    d_in = grpah -> d_in;
    d_out = graph -> d_out;

    vertex_map = &(graph -> vertex_map);

    theta = 2000000000;

    in_label = new vector<Triplet> [max_v + 1];
    if (directed) {
        out_label = new vector<Triplet> [max_v + 1];
    }
    else {
        out_label = in_label;
    }

	idx = new int [max_v + 1];
	iota(idx + 1, idx + max_v + 1, 1);
	sort(idx + 1, idx + n + 1, [&](int i, int j){return (long long)(d_in[i] + 1) * (d_out[i] + 1) > (long long)(d_in[j] + 1) * (d_out[j] + 1);});

	order = new int [max_v + 1];
	iota(order + 1, order + max_v + 1, 1);
	for (int i = 1; i <= n; i++)
		order[idx[i]] = i;
}

TILL::~TILL() {
    if (in_label) delete[] in_label;
    if (directed && out_label) delete[] out_label;
    if (idx) delete[] idx;
    if (order) delete[] order;
}

void TILL::construct() {
    for (int i = 1; i <= n; i++) {
        Q.push(Tri(i, -1, -1));
        construct_for_a_vertex(graph_head, in_label, i, true, false);

        if (directed) {
            Q.push(Tri(i, -1, -1));
            construct_for_a_vertex(graph_r_head, out_label, i, false, false);
        }
    }
}

void TILL::add_triplet(vector<Triplet> *label, int u, int v, int t1, int t2, bool update) {
    if (!update || !label[u].size()) {
        label[u].push_back(Triplet(v, t1, t2));
    }
    else {
        Triplet temp(v, t1, t2);
        vector<Triplet>::iterator it = upper_bound(label[u].begin(), label[u].end(), temp);
        label[u].insert(it, temp);
    }
}

void TILL::construct_for_a_vertex(GraphEdge **head, vector<Triplet> *label, int u, bool in, bool update) {
    while (!Q.empty()) {
        Tri tri = Q.top();
        Q.pop();
        int v = tri.v;
        if (u != v) {
            if ((in && span_reach(u, v, tri.t1, tri.t2)) || (!in && span_reach(v, u, tri.t1, tri.t2))) {
                continue;
            }
            else {
                add_triplet(label, v, u, tri.t1, tri.t2, update);
            }
        }
        for (GraphEdge *it = head[idx[v]]; it; it = it -> next) {
            if (order[it -> v] > u) {
				int t1 = (tri.t1 == -1? it -> ts: min(tri.t1, it -> ts));
				int t2 = (tri.t2 == -1? it -> ts: max(tri.t2, it -> ts));
				if (t2 - t1 + 1 <= theta)
					Q.push(Tri(order[it -> v], t1, t2));
            }
        }
    }
}

bool TILL::span_reach(int u, int v, int t1, int t2, bool original_id) {
    if (u == v) return true;
    if (original_id) {
        u = order[(*vertex_map)[u]];
        v = order[(*vertex_map)[v]];
    }

    // check if v in Lout(u)
	vector<Triplet>::iterator it = lower_bound(out_label[u].begin(), out_label[u].end(), Triplet(v, t1, t2));
	while (it != out_label[u].end() && it -> v == v){
		if (t1 <= it -> t1 && it -> t2 <= t2) return true;
		it++;
	}

    // check if u in Lin(v)
	it = lower_bound(in_label[v].begin(), in_label[v].end(), Triplet(u, t1, t2));
	while (it != in_label[v].end() && it -> v == u){
		if (t1 <= it -> t1 && it -> t2 <= t2) return true;
		it++;
	}

	for (vector<Triplet>::iterator it1 = out_label[u].begin(), it2 = in_label[v].begin(); it1 != out_label[u].end() && it2 != in_label[v].end(); ) {
		if (it1 -> v < it2 -> v) it1++;
		else if (it1 -> v > it2 -> v) it2++;
		else if (t1 <= it1 -> t1 && it1 -> t2 <= t2 && t1 <= it2 -> t1 && it2 -> t2 <= t2) return true;
		else if (!(t1 <= it1 -> t1 && it1 -> t2 <= t2)) it1++;
		else it2++;
	}
    return false;
}