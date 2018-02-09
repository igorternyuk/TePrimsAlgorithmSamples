#include "prim.hpp"
#include <cstdlib>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <sstream>

Prims1::Prims1()
{}

Prims1::Prims1(const std::vector<std::vector<int> > &adjacency_matrix):
    m_adjacency_matrix{adjacency_matrix}, m_used(m_adjacency_matrix.size(), false),
    m_min_edges(m_adjacency_matrix.size(), {INFINITY, -1})
{}

const std::vector<std::pair<int,int> > &Prims1::find_mst()
{
    if(has_mst_changed)
    {
        m_used.clear();
        m_used.resize(m_adjacency_matrix.size());
        std::fill(m_used.begin(), m_used.end(), false);
        m_min_edges.clear();
        m_min_edges.resize(m_adjacency_matrix.size());
        std::fill(m_min_edges.begin(), m_min_edges.end(), edge_min(INFINITY, -1));
        m_mst.clear();
        m_total_cost = 0;

        const size_t vertex_num = m_adjacency_matrix.size();
        m_min_edges[0].weight = 0;

        for(size_t i{0}; i < vertex_num; ++i)
        {
            int curr_vertex {-1};
            for(size_t j{0}; j < vertex_num; ++j)
            {
                if(!m_used[j] && (curr_vertex == -1 ||
                   m_min_edges[j].weight < m_min_edges[curr_vertex].weight))
                    curr_vertex = j;
            }

            if(m_min_edges[curr_vertex].weight == INFINITY)
                return m_mst;
            m_used[curr_vertex] = true;
            if(m_min_edges[curr_vertex].end_on_mst != -1)
                m_mst.push_back({m_min_edges[curr_vertex].end_on_mst, curr_vertex});
            m_total_cost += m_min_edges[curr_vertex].weight;

            //Recalculation of minimum edges

            for(size_t to{0}; to < vertex_num; ++to)
            {
                if(m_adjacency_matrix[curr_vertex][to] < m_min_edges[to].weight)
                {
                    m_min_edges[to].weight = m_adjacency_matrix[curr_vertex][to];
                    m_min_edges[to].end_on_mst = curr_vertex;
                }
            }
        }
        has_mst_changed = false;
    }
    return m_mst;
}

void Prims1::set_edge_weight(int from, int to, int weight)
{
    if(from >= 0 && from < (int)m_adjacency_matrix.size() &&
       to >= 0 && to < (int)m_adjacency_matrix[from].size())
    {
        m_adjacency_matrix[from][to] = weight;
        has_mst_changed = true;
    }
}

void Prims1::read_edges_from_file(const std::string &path_to_file)
{
    std::ifstream fi(path_to_file);
    if(fi.is_open())
    {
        int counter {0};
        while(!fi.eof())
        {
            std::string line;
            std::getline(fi, line);
            std::stringstream ss;
            ss << line;
            if(counter == 0)
            {
                int num_vertex;
                ss >> num_vertex;
                m_adjacency_matrix.clear();
                m_adjacency_matrix.reserve(num_vertex);
                std::vector<int> row(num_vertex, INFINITY);
                for(int i{0}; i < num_vertex; ++i)
                    m_adjacency_matrix.push_back(row);
                for(int i{0}; i < num_vertex; ++i)
                    m_adjacency_matrix[i][i] = 0;
                ++counter;
                continue;
            }
            int from, to, weight;
            ss >> from >> to >> weight;
            m_adjacency_matrix[from][to] = m_adjacency_matrix[to][from] = weight;
            ++counter;
        }

        fi.close();
    }
}

void Prims1::write_mst_to_file(const std::string &path_to_file)
{
    std::ofstream fo(path_to_file);
    if(fo.is_open())
    {
        for(const auto &v: m_mst)
            fo << "(" << v.first << "," << v.second << ") -> ";
        fo << "\nTotal cost: " << m_total_cost;
        fo.close();
    }
}

void Prims1::print_adjacency_matrix() const
{
    for(size_t i{0}; i < m_adjacency_matrix.size(); ++i)
    {
        for(size_t j{0}; j < m_adjacency_matrix[i].size(); ++j)
            std::cout << m_adjacency_matrix[i][j] << " ";
        std::cout << std::endl;
    }
}

void Prims1::print_mst() const
{
    for(const auto &v: m_mst)
        std::cout << "(" << v.first << "," << v.second << ") -> ";
    std::cout << std::endl << "Total cost: " << m_total_cost << std::endl;
}


Prims2::Prims2()
{}

void Prims2::read_edges_from_file(const std::string &path_to_file)
{
    std::ifstream fi(path_to_file);
    if(fi.is_open())
    {
        int counter {0};
        int num_vertex {0};
        while(!fi.eof())
        {
            std::string line;
            std::getline(fi, line);
            std::stringstream ss;
            ss << line;
            if(counter == 0)
            {
                num_vertex = std::atoi(line.c_str());
                m_adjacency_sets.clear();
                m_adjacency_sets.reserve(num_vertex);
                for(int i {0}; i < num_vertex; ++i)
                {
                    std::set<std::pair<int,int>> neighbours;
                    m_adjacency_sets.push_back(neighbours);
                }
                ++counter;
                continue;
            }
            int from, to, weight;
            ss >> from >> to >> weight;
            if(from >= 0 && from < num_vertex &&
               to >= 0 && to < num_vertex)
            {
                m_adjacency_sets[from].insert({to,weight});
                m_adjacency_sets[to].insert({from,weight});
            }
        }
        fi.close();
    }
}

void Prims2::add_neigbour_to_vertex(int vertex, int neigbour, int weight)
{
    if(vertex >= 0 && vertex < int(m_adjacency_sets.size()) &&
       neigbour >= 0 && neigbour < int(m_adjacency_sets.size()))
    {
        m_adjacency_sets[vertex].insert({neigbour, weight});
        m_has_mst_changed = true;
    }
}

const std::vector<std::pair<int, int> > &Prims2::find_mst()
{
    if(m_has_mst_changed)
    {
        const size_t num_vertex {m_adjacency_sets.size()};
        m_min_edges.clear();
        m_min_edges.resize(num_vertex);
        std::fill(m_min_edges.begin(), m_min_edges.end(), std::make_pair(INFINITY, -1));
        m_mst.clear();
        m_mst.reserve(num_vertex);
        m_used.resize(num_vertex);
        std::fill(m_used.begin(), m_used.end(), false);
        m_total_cost = 0;

        //Algorithm

        std::set<std::pair<int,int> > vertex_queue;
        m_min_edges[0].first = 0;
        vertex_queue.insert({0,0});

        for(size_t i{0}; i < num_vertex; ++i)
        {
            if(vertex_queue.empty())
            {
                std::cout << "Could not found MST" << std::endl;
                return m_mst;
            }
            auto curr_vertex = vertex_queue.begin()->second;
            m_used[curr_vertex] = true;
            vertex_queue.erase(vertex_queue.begin());
            if(m_min_edges[curr_vertex].second != -1)
                m_mst.push_back({m_min_edges[curr_vertex].second, curr_vertex });
            m_total_cost += m_min_edges[curr_vertex].first;

            //Recalculation of minimum edges

            auto neighbours = m_adjacency_sets[curr_vertex];
            for(auto it = neighbours.begin(); it != neighbours.end(); ++it)
            {
                auto to = it->first;
                if(m_used[to]) continue;
                auto weight = it->second;
                if(weight < m_min_edges[to].first)
                {
                    vertex_queue.erase({m_min_edges[to].first, to});
                    m_min_edges[to].first = weight;
                    m_min_edges[to].second = curr_vertex;
                    vertex_queue.insert({m_min_edges[to].first, to});
                }
            }
        }
        m_has_mst_changed = false;
    }
    return m_mst;
}

void Prims2::write_mst_to_file(const std::string &path_to_file)
{
    std::ofstream fo(path_to_file);
    if(fo.is_open())
    {
        for(const auto &v: m_mst)
            fo << "(" << v.first << "," << v.second << ") -> ";
        fo << std::endl << "Total cost: " << m_total_cost << std::endl;
        fo.close();
    }
}

void Prims2::print_adjacency_sets() const
{
    for(size_t i{0}; i < m_adjacency_sets.size(); ++i)
    {
        std::cout << i << " - ";
        for(auto it = m_adjacency_sets[i].begin(); it != m_adjacency_sets[i].end(); ++it)
            std::cout << "(" << it->first << "," << it->second << ") ";
        std::cout << std::endl;
    }
}

void Prims2::print_mst() const
{
    for(const auto &v: m_mst)
        std::cout << "(" << v.first << "," << v.second << ") -> ";
    std::cout << std::endl << "Total cost: " << m_total_cost << std::endl;
}
