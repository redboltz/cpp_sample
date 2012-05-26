#include <iostream>
#include <boost/typeof/typeof.hpp>
#include <list>
#include <boost/optional.hpp>
#include <boost/shared_ptr.hpp>
#include <memory>
enum myo {
    HOGE = 10U,
	FUGA = 3
};

int main() {
	myo m = FUGA;
	myo m2 = myo(3);
	std::list<int> l;
	l.push_back(1);
	l.push_back(5);
	l.push_back(3);
	boost::optional<int> o1 = 55;
	boost::optional<int> o2;
	boost::shared_ptr<int> si(new int(55));
	std::tr1::shared_ptr<int> si2(new int(55));
	if (o1) {
	}

}
