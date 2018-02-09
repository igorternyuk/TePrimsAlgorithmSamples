#include <iostream>
#include "prim.hpp"

int main()
{
    Prims1 p1;
    p1.read_edges_from_file("input.txt");
    p1.print_adjacency_matrix();
    auto mst = p1.find_mst();
    p1.print_mst();
    p1.write_mst_to_file("output.txt");

    Prims2 p2;
    p2.read_edges_from_file("input.txt");
    p2.print_adjacency_sets();
    p2.find_mst();
    p2.print_mst();
    p2.write_mst_to_file("output2.txt");
    return 0;
}
