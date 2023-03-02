#include "graph.h"

using namespace std;

int main(int argc, char * argv[]) {
    string input_graph = argv[1];
    string directed = argv[2];
    string graph_stream = argv[3];

    Graph graph = Graph(input_graph, directed);

    graph.construct();

    graph.insert_stream(graph_stream);

    // graph.print_index();

    graph.calculate_landmark_size();

    // graph.test_correctness();
}