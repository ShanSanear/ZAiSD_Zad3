// Algorytm kolorowania krawędzi grafu nieskierowanego
// Data   : 26.05.2014
// (C)2014 mgr Jerzy Wałaszek
//----------------------------------------------------

#include <iostream>

using namespace std;

// Definicja elementu listy sąsiedztwa

struct node {
    node *next; // Następny element listy;
    int vertex_number;          // Wierzchołek docelowy
    int edge_number;          // Numer krawędzi
};

int main() {
    int number_of_vertexes, number_of_edges, vertex_u, vertex_v;
    int current_vertex_color, *color_table, *order_table, *vertex_numbers_table;
    int i;
    node **graph, **second_graph, *current_node, *reference_node, *start_node;
    bool *available_colors;

    cin >> number_of_vertexes >> number_of_edges;            // Odczytujemy liczbę wierzchołków i krawędzi grafu

    graph = new node *[number_of_vertexes];  // Tworzymy zerowy graf G
    for (i = 0; i < number_of_vertexes; i++) graph[i] = nullptr;

    second_graph = new node *[number_of_edges]; // Tworzymy zerowy graf GE
    for (i = 0; i < number_of_edges; i++) second_graph[i] = nullptr;

    color_table = new int[number_of_edges];       // Tablica kolorów wierzchołków
    order_table = new int[number_of_edges];       // Tablica stopni wyjściowych wierzchołków
    vertex_numbers_table = new int[number_of_edges];       // Tablica numerów wierzchołków
    available_colors = new bool[number_of_edges];      // Tablica dostępności kolorów

    // Odczytujemy definicje krawędzi grafu G

    for (i = 0; i < number_of_edges; i++) {
        cin >> vertex_v >> vertex_u;     // Czytamy wierzchołki
        current_node = new node;   // Tworzymy rekord listy
        current_node->vertex_number = vertex_u;          // Wypełniamy go danymi
        current_node->edge_number = i;
        current_node->next = graph[vertex_v]; // Element dołączamy do listy sąsiedztwa wierzchołka v
        graph[vertex_v] = current_node;

        current_node = new node;   // To samo dla krawędzi odwrotnej
        current_node->vertex_number = vertex_v;
        current_node->edge_number = i;
        current_node->next = graph[vertex_u];
        graph[vertex_u] = current_node;
    }

    // Tworzymy graf krawędziowy

    for (vertex_v = 0; vertex_v < number_of_vertexes; vertex_v++) {    // Przechodzimy przez kolejne wierzchołki grafu
        for (current_node = graph[vertex_v]; current_node; current_node = current_node->next) { // Przechodzimy przez listę sąsiadów wierzchołka v
            for (reference_node = graph[current_node->vertex_number]; reference_node; reference_node = reference_node->next) { // Przechodzimy przez listę sąsiadów sąsiada v
                if (reference_node->vertex_number != vertex_v) {
                    start_node = new node;       // Tworzymy nowy element listy
                    start_node->vertex_number = reference_node->edge_number;           // Wierzchołkiem docelowym będzie krawędź wychodząca
                    start_node->next = second_graph[current_node->edge_number]; // Wierzchołkiem startowym będzie krawędź wchodząca
                    second_graph[current_node->edge_number] = start_node;       // Dodajemy krawędź do grafu krawędziowego
                }
            }
        }
    }

    // Rozpoczynamy algorytm kolorowania grafu krawędziowego

    for (vertex_v = 0; vertex_v < number_of_edges; vertex_v++)      // Przeglądamy kolejne wierzchołki grafu
    {
        vertex_numbers_table[vertex_v] = vertex_v;               // Zapamiętujemy numer wierzchołka
        order_table[vertex_v] = 0;               // Zerujemy jego stopień wyjściowy

        for (current_node = second_graph[vertex_v]; current_node; current_node = current_node->next) // Przeglądamy kolejnych sąsiadów
            order_table[vertex_v]++;              // Obliczamy stopień wyjściowy wierzchołka v

        current_vertex_color = order_table[vertex_v];
        int currently_checked_vertex;

        for (currently_checked_vertex = vertex_v;
             (currently_checked_vertex > 0) && (order_table[currently_checked_vertex - 1] < current_vertex_color);
             currently_checked_vertex--) {
            order_table[currently_checked_vertex] = order_table[currently_checked_vertex - 1];
            vertex_numbers_table[currently_checked_vertex] = vertex_numbers_table[currently_checked_vertex - 1];
        }

        order_table[currently_checked_vertex] = current_vertex_color;
        vertex_numbers_table[currently_checked_vertex] = vertex_v;
    }

    // Teraz stosujemy algorytm zachłanny, lecz wierzchołki wybieramy wg VT

    for (i = 0; i < number_of_edges; i++) {
        color_table[i] = -1;
    }

    color_table[vertex_numbers_table[0]] = 0;

    for (vertex_v = 1; vertex_v < number_of_edges; vertex_v++)      // Przeglądamy resztę grafu
    {
        for (i = 0; i < number_of_edges; i++) {
            available_colors[i] = false;
        }

        for (current_node = second_graph[vertex_numbers_table[vertex_v]]; current_node; current_node = current_node->next) {
            if (color_table[current_node->vertex_number] > -1) {
                available_colors[color_table[current_node->vertex_number]] = true; // Oznaczamy kolor jako zajęty
            }
        }
        int current_color;
        for (current_color = 0; available_colors[current_color]; current_color++);

        color_table[vertex_numbers_table[vertex_v]] = current_color;        // Przypisujemy go bieżącemu wierzchołkowi
    }

    // Wyświetlamy wyniki

    cout << endl;
    for (i = 0; i < number_of_edges; i++) available_colors[i] = true;
    for (vertex_v = 0; vertex_v < number_of_vertexes; vertex_v++)
        for (current_node = graph[vertex_v]; current_node; current_node = current_node->next)
            if (available_colors[current_node->edge_number]) {
                available_colors[current_node->edge_number] = false;
                cout << "edge " << vertex_v << "-" << current_node->vertex_number << " has color "
                     << color_table[current_node->edge_number] << endl;
            }
    cout << endl;

    // Usuwamy tablice dynamiczne

    for (vertex_v = 0; vertex_v < number_of_vertexes; vertex_v++) {
        current_node = graph[vertex_v];
        while (current_node) {
            reference_node = current_node;
            current_node = current_node->next;
            delete reference_node;
        }
    }

    for (vertex_v = 0; vertex_v < number_of_edges; vertex_v++) {
        current_node = second_graph[vertex_v];
        while (current_node) {
            reference_node = current_node;
            current_node = current_node->next;
            delete reference_node;
        }
    }

    delete[] graph;
    delete[] second_graph;
    delete[] color_table;
    delete[] order_table;
    delete[] vertex_numbers_table;
    delete[] available_colors;

    return 0;
}
 