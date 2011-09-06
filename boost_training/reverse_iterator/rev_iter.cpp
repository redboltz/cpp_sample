#include <boost/iterator/reverse_iterator.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/compressed_sparse_row_graph.hpp>
#include <list>

void test_list() {
    std::list<int> l;
    l.push_back(0);
    l.push_back(1);
    {
        std::list<int>::iterator ib = l.begin();
        std::list<int>::iterator ie = l.end();
        std::cout << *ib++ << std::endl;
	    std::cout << *ib++ << std::endl;

    }
    {
        std::list<int>::iterator ib = l.begin();
        std::list<int>::iterator ie = l.end();
        boost::reverse_iterator<std::list<int>::iterator> rib(ie);
        boost::reverse_iterator<std::list<int>::iterator> rie(ib);
	    std::cout << *rib++ << std::endl;
	    std::cout << *rib++ << std::endl;
    }
}

void test_graph() {
    typedef boost::adjacency_list<> graph_type;
	graph_type g(2);
    boost::graph_traits<graph_type>::vertex_iterator ib, ie;
    {
	    boost::tie(ib, ie) = boost::vertices(g);
	    std::cout << *ib++ << std::endl;
	    std::cout << *ib++ << std::endl;
    }
    {
	    boost::tie(ib, ie) = boost::vertices(g);
        boost::reverse_iterator<boost::graph_traits<graph_type>::vertex_iterator> rib(ie);
        boost::reverse_iterator<boost::graph_traits<graph_type>::vertex_iterator> rie(ib);

	    std::cout << *rib++ << std::endl;
	    std::cout << *rib++ << std::endl;
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
	    std::cout << *ib++ << std::endl;
	    std::cout << *ib++ << std::endl;
    }
    {
	    boost::tie(ib, ie) = boost::vertices(g);
        boost::reverse_iterator<boost::graph_traits<graph_type>::vertex_iterator> rib(ie);
        boost::reverse_iterator<boost::graph_traits<graph_type>::vertex_iterator> rie(ib);

	    std::cout << *rib++ << std::endl;
	    std::cout << *rib++ << std::endl;
    }
    {
	    boost::tie(ib, ie) = boost::vertices(g);
	    std::cout << *--ie << std::endl;
	    std::cout << *--ie << std::endl;
    }
}



int main() {
    test_list();
    test_graph();
    test_csr_graph();
}

