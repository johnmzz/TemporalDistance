#include "graph.h"

using namespace std;

int main(int argc, char * argv[]) {
    string input_graph = argv[1];
    string directed = argv[2];

    Graph graph = Graph(input_graph, directed);

    graph.construct();

    //graph.test_correctness();

    graph.calculate_index_size();
    graph.print_labels();
}