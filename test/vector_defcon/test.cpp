#include <vector>
#include <boost/ref.hpp>

struct ClsA {};
struct ClsB {};

class ClsAClsBPair {
public:
	ClsAClsBPair(ClsA& clsA, ClsB& clsB):clsA_(clsA), clsB_(clsB) {}
	ClsAClsBPair(ClsAClsBPair const& abp):clsA_(abp.clsA_), clsB_(abp.clsB_) {}
	ClsA const& clsA() const { return clsA_; }
	ClsB const& clsB() const { return clsB_; }
	ClsA& clsA() { return clsA_; }
	ClsB& clsB() { return clsB_; }
	friend bool operator< (ClsAClsBPair const& lhs, ClsAClsBPair const& rhs) {
		if (lhs.clsA_.get_pointer() < rhs.clsA_.get_pointer()) return true;
		if (lhs.clsA_.get_pointer() > rhs.clsA_.get_pointer()) return false;
		if (lhs.clsB_.get_pointer() < rhs.clsB_.get_pointer()) return true;
		return false;
	}
private:
	boost::reference_wrapper<ClsA> clsA_;
	boost::reference_wrapper<ClsB> clsB_;
};



typedef std::vector<ClsAClsBPair> AbpCol;

class Hoge {
	AbpCol col_;
};

int main() {
	std::vector<ClsAClsBPair> m;
	Hoge h;
}
