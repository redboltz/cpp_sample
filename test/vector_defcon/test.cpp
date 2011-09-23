#include <vector>
#include <boost/ref.hpp>

class Area;
class Content;

class AreaContentPair {
public:
	AreaContentPair(Area& area, Content& content):area_(area), content_(content) {}
	AreaContentPair(AreaContentPair const& acp):area_(acp.area_), content_(acp.content_) {}
	Area const& area() const { return area_; }
	Content const& content() const { return content_; }
	Area& area() { return area_; }
	Content& content() { return content_; }
	friend bool operator< (AreaContentPair const& lhs, AreaContentPair const& rhs) {
		if (lhs.area_.get_pointer() < rhs.area_.get_pointer()) return true;
		if (lhs.area_.get_pointer() > rhs.area_.get_pointer()) return false;
		if (lhs.content_.get_pointer() < rhs.content_.get_pointer()) return true;
		return false;
	}
private:
	boost::reference_wrapper<Area> area_;
	boost::reference_wrapper<Content> content_;
};


struct Area {};
struct Content {};

int main() {
	std::vector<AreaContentPair> m;
}
