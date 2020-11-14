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
#include <algorithm>

const char CSV_SEPARATOR = ';';

const bool DEBUG = false;

class Graph {

public:
    /// Constructor
    Graph();

    /// Shows graph's matrix
    void show_graph_matrix();

    void load_graph_matrix_from_stdin();

    void color_graph();

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

    void check_neighbours_colors(int vertex_to_check, const std::vector<int> &vertex_stack);
};


void Graph::add_edge(int vertex_u, int vertex_v, bool bidirectional) {
    matrix[vertex_u][vertex_v] = 1;
    if (bidirectional) {
        matrix[vertex_v][vertex_u] = 1;
    }
    if (DEBUG) {
        printf("Adding edge: %d -> %d\n", vertex_u, vertex_v);
    }
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

void Graph::check_neighbours_colors(int vertex_to_check, const std::vector<int> &vertex_stack) {
    int minimum_color = vertex_colors[vertex_to_check] + 1;
    bool can_assing_color = true;
    for (int current_color = minimum_color; current_color <= number_of_colors; current_color++) {
        can_assing_color = true;
        for (int other_vertex : vertex_stack) {
            if (other_vertex == vertex_to_check) {
                continue;
            }
            if (matrix[vertex_to_check][other_vertex] == 1) {
                if (vertex_colors[other_vertex] == current_color) {
                    can_assing_color = false;
                }
            }
        }
        if (can_assing_color) {
            vertex_colors[vertex_to_check] = current_color;
            break;
        }
    }
    if (!can_assing_color || minimum_color > number_of_colors) {
        vertex_colors[vertex_to_check] = 0;
    }
}

void Graph::color_graph() {
    for (int i = 0; i < vertex_count; i++) {
        vertex_colors[i] = 0;
    }
    std::vector<int> vertex_stack;
    vertex_stack.push_back(0);
    int current_vertex;
    while (vertex_stack.size() <= vertex_count && !vertex_stack.empty()) {
        current_vertex = vertex_stack.back();
        check_neighbours_colors(current_vertex, vertex_stack);
        if (vertex_colors[current_vertex] == 0) {
            vertex_stack.pop_back();
        } else {
            vertex_stack.push_back(current_vertex + 1);
        }
    }
    for (int x : vertex_stack) {
        if (DEBUG) {
            printf("Value on stack: %d\n", x);
        }
    }
    for (int i = 0; i < vertex_count; i++) {
        if (DEBUG) {
            printf("Color for %d is %d\n", i, vertex_colors[i]);
        } else if (i != vertex_count - 1) {
            printf("%d ", vertex_colors[i]);
        } else {
            printf("%d", vertex_colors[i]);
        }
    }
}

void Graph::load_graph_matrix_from_stdin() {
    std::string line;
    std::vector<std::vector<int>> input_data;
    int num_of_vectors;
    std::getline(std::cin, line);
    int space_index = line.find(' ');
    num_of_vectors = std::stoi(line.substr(0, space_index));
    number_of_colors = std::stoi(line.substr(space_index + 1, line.size()));
    initialize_matrix(num_of_vectors);
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
    int number_of_cases;
    std::string line;
    std::getline(std::cin, line);
    number_of_cases = std::stoi(line);
    for (int case_num = 0; case_num < number_of_cases; case_num++) {
        Graph graph = Graph();
        graph.load_graph_matrix_from_stdin();
        if (DEBUG) {
            graph.show_graph_matrix();
        }
        graph.color_graph();
        graph.deallocate_memory();
        if (case_num != number_of_cases-1) {
            printf("\n");
        }
    }
    return 0;
}