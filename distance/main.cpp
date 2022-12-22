#include "graph.h"

using namespace std;

int main(int argc, char * argv[]) {
    string input_graph = argv[1];
    string directed = argv[2];
    string method = argv[3];

    Graph graph = Graph(input_graph, directed);

    if (method == "2-hop")
        graph.construct();
    else if (method == "landmark")
        graph.construct_landmark();
    else
        cout << "Incorrect method!" << endl;

    graph.calculate_landmark_size();

    //graph.print_index();
    graph.test_landmark_correctness();
}