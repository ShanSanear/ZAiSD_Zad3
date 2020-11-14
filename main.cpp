// Algorytm kolorowania krawędzi grafu nieskierowanego
// Data   : 26.05.2014
// (C)2014 mgr Jerzy Wałaszek
//----------------------------------------------------

#include <iostream>

using namespace std;

// Definicja elementu listy sąsiedztwa

struct slistEl
{
    slistEl * next; // Następny element listy;
    int v;          // Wierzchołek docelowy
    int i;          // Numer krawędzi
};

int main( )
{
    int n, m, i, u, v, d, *CT, *DT, *VT;
    slistEl **G, **GE, *p, *r, *s;
    bool *C;

    cin >> n >> m;            // Odczytujemy liczbę wierzchołków i krawędzi grafu

    G = new slistEl * [ n ];  // Tworzymy zerowy graf G
    for( i = 0; i < n; i++ ) G [ i ] = NULL;

    GE = new slistEl * [ m ]; // Tworzymy zerowy graf GE
    for( i = 0; i < m; i++ ) GE [ i ] = NULL;

    CT = new int [ m ];       // Tablica kolorów wierzchołków
    DT = new int [ m ];       // Tablica stopni wyjściowych wierzchołków
    VT = new int [ m ];       // Tablica numerów wierzchołków
    C  = new bool [ m ];      // Tablica dostępności kolorów

    // Odczytujemy definicje krawędzi grafu G

    for( i = 0; i < m; i++ )
    {
        cin >> v >> u;     // Czytamy wierzchołki
        p = new slistEl;   // Tworzymy rekord listy
        p->v = u;          // Wypełniamy go danymi
        p->i = i;
        p->next = G [ v ]; // Element dołączamy do listy sąsiedztwa wierzchołka v
        G [ v ] = p;

        p = new slistEl;   // To samo dla krawędzi odwrotnej
        p->v = v;
        p->i = i;
        p->next = G [ u ];
        G [ u ] = p;
    }

    // Tworzymy graf krawędziowy

    for( v = 0; v < n; v++ )       // Przechodzimy przez kolejne wierzchołki grafu
        for( p = G [ v ]; p; p = p->next ) // Przechodzimy przez listę sąsiadów wierzchołka v
            for( r = G [ p->v ]; r; r = r->next ) // Przechodzimy przez listę sąsiadów sąsiada v
                if( r->v != v )
                {
                    s = new slistEl;       // Tworzymy nowy element listy
                    s->v = r->i;           // Wierzchołkiem docelowym będzie krawędź wychodząca
                    s->next = GE [ p->i ]; // Wierzchołkiem startowym będzie krawędź wchodząca
                    GE [ p->i ] = s;       // Dodajemy krawędź do grafu krawędziowego
                }

    // Rozpoczynamy algorytm kolorowania grafu krawędziowego

    for( v = 0; v < m; v++ )      // Przeglądamy kolejne wierzchołki grafu
    {
        VT [ v ] = v;               // Zapamiętujemy numer wierzchołka
        DT [ v ] = 0;               // Zerujemy jego stopień wyjściowy

        for( p = GE [ v ]; p; p = p->next ) // Przeglądamy kolejnych sąsiadów
            DT [ v ] ++;              // Obliczamy stopień wyjściowy wierzchołka v

        // Sortujemy DT i VT

        d = DT [ v ];

        for( i = v; ( i > 0 ) && ( DT [ i - 1 ] < d ); i-- )
        {
            DT [ i ] = DT [ i - 1 ];
            VT [ i ] = VT [ i - 1 ];
        }

        DT [ i ] = d;
        VT [ i ] = v;
    }

    // Teraz stosujemy algorytm zachłanny, lecz wierzchołki wybieramy wg VT

    for( i = 0; i < m; i++ ) CT [ i ] = -1;

    CT [ VT [ 0 ] ] = 0;          // Wierzchołek startowy

    for( v = 1; v < m; v++ )      // Przeglądamy resztę grafu
    {
        for( i = 0; i < m; i++ ) C [ i ] = false;

        for( p = GE [ VT [ v ] ]; p; p = p->next ) // Przeglądamy sąsiadów bieżącego wierzchołka
            if( CT [ p->v ] > -1 ) C [ CT [ p->v ] ] = true; // Oznaczamy kolor jako zajęty

        for( i = 0; C [ i ]; i++ ); // Szukamy wolnego koloru

        CT [ VT [ v ] ] = i;        // Przypisujemy go bieżącemu wierzchołkowi
    }

    // Wyświetlamy wyniki

    cout << endl;
    for( i = 0; i < m; i++ ) C [ i ] = true;
    for( v = 0; v < n; v++ )
        for( p = G [ v ]; p; p = p->next )
            if( C [ p->i ] )
            {
                C [ p->i ] = false;
                cout << "edge " << v << "-" << p->v << " has color " << CT [ p->i ] << endl;
            }
    cout << endl;

    // Usuwamy tablice dynamiczne

    for( v = 0; v < n; v++ )
    {
        p = G [ v ];
        while( p )
        {
            r = p;
            p = p->next;
            delete r;
        }
    }

    for( v = 0; v < m; v++ )
    {
        p = GE [ v ];
        while( p )
        {
            r = p;
            p = p->next;
            delete r;
        }
    }

    delete [ ] G;
    delete [ ] GE;
    delete [ ] CT;
    delete [ ] DT;
    delete [ ] VT;
    delete [ ] C;

    return 0;
}
 