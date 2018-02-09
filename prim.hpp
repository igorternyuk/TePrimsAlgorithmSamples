#ifndef PRIM_HPP
#define PRIM_HPP

#include <vector>
#include <set>
#include <string>

class Prims1
{
public:
    explicit Prims1();
    explicit Prims1(const std::vector<std::vector<int>> &adjacency_matrix);
    const std::vector<std::pair<int,int> >& find_mst();
    void set_edge_weight(int from, int to, int weight);
    inline auto get_total_cost() const noexcept { return m_total_cost; }
    void read_edges_from_file(const std::string &path_to_file);
    void write_mst_to_file(const std::string &path_to_file);
    void print_adjacency_matrix() const;
    void print_mst() const;
private:
    enum { INFINITY = 9999 };
    struct edge_min
    {
        int weight;
        int end_on_mst; //The end of the edge which belongs to the minimun spanning tree
        edge_min(int w = INFINITY, int e = -1):
            weight{w}, end_on_mst{e}
        {}
    };
    std::vector<std::vector<int> > m_adjacency_matrix;
    std::vector<bool> m_used;
    std::vector<edge_min> m_min_edges;
    std::vector<std::pair<int,int> > m_mst;
    int m_total_cost {0};
    mutable bool has_mst_changed {true};

};

class Prims2
{
public:
    explicit Prims2();
    void read_edges_from_file(const std::string &path_to_file);
    void add_neigbour_to_vertex(int vertex, int neigbour, int weight);
    const std::vector<std::pair<int,int> >& find_mst();
    void write_mst_to_file(const std::string &path_to_file);
    void print_adjacency_sets() const;
    void print_mst() const;
private:
    enum { INFINITY = 9999 };
    std::vector<std::set<std::pair<int, int> > > m_adjacency_sets; // pair - neighbour vertex and its weight
    std::vector<std::pair<int,int> > m_min_edges; // pair - weight of minimum permissible edge from i to mst,
                                                  // index of the second end of above mentioned edge
    std::vector<std::pair<int,int> > m_mst;
    std::vector<bool> m_used;
    int m_total_cost {0};
    mutable bool m_has_mst_changed {true};
};

#endif // PRIM_HPP
