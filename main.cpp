// Algorytm kolorowania krawędzi grafu nieskierowanego
// Data   : 26.05.2014
// (C)2014 mgr Jerzy Wałaszek
//----------------------------------------------------

#include <iostream>

using namespace std;

// Definicja elementu listy sąsiedztwa

struct node {
    node *next; // Następny element listy;
    int v;          // Wierzchołek docelowy
    int i;          // Numer krawędzi
};

int main() {
    int number_of_vertexes, number_of_edges, vertex_u, vertex_v;
    int d, *color_table, *order_table, *vertex_numbers_table;
    int i;
    node **graph, **second_graph, *p, *r, *s;
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
        p = new node;   // Tworzymy rekord listy
        p->v = vertex_u;          // Wypełniamy go danymi
        p->i = i;
        p->next = graph[vertex_v]; // Element dołączamy do listy sąsiedztwa wierzchołka v
        graph[vertex_v] = p;

        p = new node;   // To samo dla krawędzi odwrotnej
        p->v = vertex_v;
        p->i = i;
        p->next = graph[vertex_u];
        graph[vertex_u] = p;
    }

    // Tworzymy graf krawędziowy

    for (vertex_v = 0; vertex_v < number_of_vertexes; vertex_v++)       // Przechodzimy przez kolejne wierzchołki grafu
        for (p = graph[vertex_v]; p; p = p->next) // Przechodzimy przez listę sąsiadów wierzchołka v
            for (r = graph[p->v]; r; r = r->next) // Przechodzimy przez listę sąsiadów sąsiada v
                if (r->v != vertex_v) {
                    s = new node;       // Tworzymy nowy element listy
                    s->v = r->i;           // Wierzchołkiem docelowym będzie krawędź wychodząca
                    s->next = second_graph[p->i]; // Wierzchołkiem startowym będzie krawędź wchodząca
                    second_graph[p->i] = s;       // Dodajemy krawędź do grafu krawędziowego
                }

    // Rozpoczynamy algorytm kolorowania grafu krawędziowego

    for (vertex_v = 0; vertex_v < number_of_edges; vertex_v++)      // Przeglądamy kolejne wierzchołki grafu
    {
        vertex_numbers_table[vertex_v] = vertex_v;               // Zapamiętujemy numer wierzchołka
        order_table[vertex_v] = 0;               // Zerujemy jego stopień wyjściowy

        for (p = second_graph[vertex_v]; p; p = p->next) // Przeglądamy kolejnych sąsiadów
            order_table[vertex_v]++;              // Obliczamy stopień wyjściowy wierzchołka v

        // Sortujemy DT i VT

        d = order_table[vertex_v];

        for (i = vertex_v; (i > 0) && (order_table[i - 1] < d); i--) {
            order_table[i] = order_table[i - 1];
            vertex_numbers_table[i] = vertex_numbers_table[i - 1];
        }

        order_table[i] = d;
        vertex_numbers_table[i] = vertex_v;
    }

    // Teraz stosujemy algorytm zachłanny, lecz wierzchołki wybieramy wg VT

    for (i = 0; i < number_of_edges; i++) color_table[i] = -1;

    color_table[vertex_numbers_table[0]] = 0;          // Wierzchołek startowy

    for (vertex_v = 1; vertex_v < number_of_edges; vertex_v++)      // Przeglądamy resztę grafu
    {
        for (i = 0; i < number_of_edges; i++) available_colors[i] = false;

        for (p = second_graph[vertex_numbers_table[vertex_v]]; p; p = p->next) // Przeglądamy sąsiadów bieżącego wierzchołka
            if (color_table[p->v] > -1) available_colors[color_table[p->v]] = true; // Oznaczamy kolor jako zajęty

        for (i = 0; available_colors[i]; i++); // Szukamy wolnego koloru

        color_table[vertex_numbers_table[vertex_v]] = i;        // Przypisujemy go bieżącemu wierzchołkowi
    }

    // Wyświetlamy wyniki

    cout << endl;
    for (i = 0; i < number_of_edges; i++) available_colors[i] = true;
    for (vertex_v = 0; vertex_v < number_of_vertexes; vertex_v++)
        for (p = graph[vertex_v]; p; p = p->next)
            if (available_colors[p->i]) {
                available_colors[p->i] = false;
                cout << "edge " << vertex_v << "-" << p->v << " has color " << color_table[p->i] << endl;
            }
    cout << endl;

    // Usuwamy tablice dynamiczne

    for (vertex_v = 0; vertex_v < number_of_vertexes; vertex_v++) {
        p = graph[vertex_v];
        while (p) {
            r = p;
            p = p->next;
            delete r;
        }
    }

    for (vertex_v = 0; vertex_v < number_of_edges; vertex_v++) {
        p = second_graph[vertex_v];
        while (p) {
            r = p;
            p = p->next;
            delete r;
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
 