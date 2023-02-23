#include "graph.h"

using namespace std;

int main(int argc, char * argv[]) {
    string input_graph = argv[1];
    string directed = argv[2];

    Graph graph = Graph(input_graph, directed);

    exit(0);

    // graph.print_graph();

    graph.construct();

    // graph.print_index();

    graph.calculate_landmark_size();

    // graph.test_correctness();
}