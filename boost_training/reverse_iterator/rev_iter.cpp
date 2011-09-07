#include <boost/iterator/reverse_iterator.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/compressed_sparse_row_graph.hpp>
#include <iostream>
#include <cassert>

void test_graph() {
    typedef boost::adjacency_list<> graph_type;
    graph_type g(2);
    boost::graph_traits<graph_type>::vertex_iterator ib, ie;
    {
        boost::tie(ib, ie) = boost::vertices(g);
        std::cout << *ib++ << std::endl; // OK
        std::cout << *ib++ << std::endl; // OK
        assert(ib == ie);
    }
    {
        boost::tie(ib, ie) = boost::vertices(g);
        boost::reverse_iterator<boost::graph_traits<graph_type>::vertex_iterator> rib(ie);
        boost::reverse_iterator<boost::graph_traits<graph_type>::vertex_iterator> rie(ib);

        std::cout << *rib++ << std::endl; // OK
        std::cout << *rib++ << std::endl; // OK
        assert(rib == rie);
    }
}

void test_csr_graph() {
    typedef boost::compressed_sparse_row_graph<> graph_type;
    std::pair<int, int> e(0, 1);
    std::pair<int, int> es[] = { e };
    graph_type g(boost::edges_are_unsorted, &es[0], &es[1], 2);
    boost::graph_traits<graph_type>::vertex_iterator ib, ie;
    {
        boost::tie(ib, ie) = boost::vertices(g);
        std::cout << *ib++ << std::endl;  // OK
        std::cout << *ib++ << std::endl;  // OK
        assert(ib == ie);
    }
    {
        boost::tie(ib, ie) = boost::vertices(g);
        boost::reverse_iterator<boost::graph_traits<graph_type>::vertex_iterator> rib(ie);
        boost::reverse_iterator<boost::graph_traits<graph_type>::vertex_iterator> rie(ib);

        std::cout << *rib++ << std::endl; // NG Why?
        std::cout << *rib++ << std::endl; // NG Why?
        assert(rib == rie);
    }
    {
        boost::tie(ib, ie) = boost::vertices(g);
        std::cout << *--ie << std::endl;  // OK
        std::cout << *--ie << std::endl;  // OK
        assert(ib == ie);
    }
}

int main() {
    test_graph();
    test_csr_graph();
}

