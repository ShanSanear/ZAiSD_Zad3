#include <ctime>
#include <omp.h>
#include <vector>
#include <fstream>
#include <filesystem>
#include <iterator>
#include <iostream>
#include <chrono>
#include <sstream>
#include <queue>
#include <stack>

const char CSV_SEPARATOR = ';';

class Graph {

public:
    /// Constructor
    Graph();

    /// Shows graph's matrix
    void show_graph_matrix();

    void load_graph_matrix_from_stdin();

    bool color_graph();

    void deallocate_memory();

private:
    /// Maximum size of the matrix being considered
    static const unsigned int MAX_MATRIX_SIZE = 30;
    /// Matrix to be searched
    int **matrix;
    int *vertex_colors;
    /// How many edges has already been loaded
    int vertex_count = 0;
    int highest_node_present = 0;
    int number_of_colors = 0;

    void add_edge(int vertex_u, int vertex_v, bool bidirectional);


    void initialize_matrix(int num_of_vectors);

    int check_neighbours(int vertex_to_check);
};


void Graph::add_edge(int vertex_u, int vertex_v, bool bidirectional) {
    matrix[vertex_u][vertex_v] = 1;
    if (bidirectional) {
        matrix[vertex_v][vertex_u] = 1;
    }
    printf("Adding edge: %d -> %d\n", vertex_u, vertex_v);
}

void Graph::show_graph_matrix() {
    int i, j;
    for (i = 0; i < vertex_count; i++) {
        for (j = 0; j < vertex_count; j++) {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }
}

int Graph::check_neighbours(int vertex_to_check) {
    int found_max = 0;
    for (int other_vertex = 0; other_vertex < vertex_count; other_vertex++) {
        if (other_vertex == vertex_to_check) {
            continue;
        }
        if (matrix[vertex_to_check][other_vertex] == 1) {
            found_max = found_max < vertex_colors[other_vertex] ?
                    vertex_colors[other_vertex] : found_max;
        }
    }
    return found_max;
}

bool Graph::color_graph() {
    for (int i = 0; i < vertex_count; i++) {
        vertex_colors[i] = 0;
    }
    std::vector<int> vertex_stack;
    vertex_stack.push_back(0);
    bool finished = false;
    int i = 0;
    int current_vertex;
    int current_color;
    int max_possible_color = 0;
    int checked_max_possible;
    while (!finished) {
        i++;
        if (i > 100) {
            finished = true;
        }
        current_vertex = vertex_stack.front();
        current_color = vertex_colors[current_vertex];
        for (int other_vertex : vertex_stack) {
            checked_max_possible = check_neighbours(other_vertex);
            max_possible_color = max_possible_color < checked_max_possible ? checked_max_possible : max_possible_color;
        }
        if (max_possible_color < current_color+1) {
            vertex_colors[current_vertex] = current_color+1;
        } else {
            vertex_stack.erase(vertex_stack.begin());
            vertex_colors[current_vertex] = 0;
        }
        if (current_vertex < vertex_count) {
            vertex_stack.insert(vertex_stack.begin(),current_vertex+1);
        }
    }
    for (int x : vertex_stack) {
        printf("Value on stack: %d\n", x);
    }
    return true;
}

void Graph::load_graph_matrix_from_stdin() {
    std::string line;
    std::vector<std::vector<int>> input_data;
    int num_of_vectors;
    std::getline(std::cin, line);
    num_of_vectors = std::stoi(line);
    initialize_matrix(num_of_vectors);
//    std::getline(std::cin, line);
//    number_of_colors = std::stoi(line);
    for (int i = 0; i < num_of_vectors; i++) {
        std::vector<int> row;
        std::getline(std::cin, line);
        std::stringstream basic_stringstream(line);
        std::string provided_number;
        int j = 0;
        while (std::getline(basic_stringstream, provided_number, ' ')) {
            int number = std::stoi(provided_number);
            matrix[i][j] = number;
            if (number == 1) {
                add_edge(i, j, false);
            }
            j++;
        }
    }
}

void Graph::initialize_matrix(int num_of_vectors) {
    matrix = new int *[num_of_vectors];
    vertex_colors = new int[num_of_vectors];
    for (int i = 0; i < num_of_vectors; ++i) {
        matrix[i] = new int[num_of_vectors];
    }
    vertex_count = num_of_vectors;
}

void Graph::deallocate_memory() {
    for (int i = 0; i < vertex_count; ++i) {
        delete[] matrix[i];
    }
    delete[] matrix;
}


Graph::Graph() = default;


int main() {
//    int number_of_cases;
//    std::string line;
//    std::getline(std::cin, line);
//    number_of_cases = std::stoi(line);
//    for (int case_num = 0; case_num < number_of_cases; case_num++) {
//        Graph graph = Graph();
//        graph.load_graph_matrix_from_stdin();
//        graph.show_graph_matrix();
//        graph.color_graph();
//        graph.deallocate_memory();
//    }
    return 0;
}