#include <boost/iterator/reverse_iterator.hpp>
#include <boost/graph/adjacency_list.hpp>
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
	boost::adjacency_list<> g(2);
    boost::graph_traits<boost::adjacency_list<> >::vertex_iterator ib, ie;
    {
	    boost::tie(ib, ie) = boost::vertices(g);
	    std::cout << *ib++ << std::endl;
	    std::cout << *ib++ << std::endl;
    }
    {
	    boost::tie(ib, ie) = boost::vertices(g);
        boost::reverse_iterator<boost::graph_traits<boost::adjacency_list<> >::vertex_iterator> rib(ie);
        boost::reverse_iterator<boost::graph_traits<boost::adjacency_list<> >::vertex_iterator> rie(ib);

	    std::cout << *rib++ << std::endl;
	    std::cout << *rib++ << std::endl;
    }
}

int main() {
    test_list();
    test_graph();
}

