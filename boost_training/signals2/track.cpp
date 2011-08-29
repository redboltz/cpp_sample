#include <iostream>
#include <memory>
#include <boost/signals2/signal.hpp>
#include <boost/signals2/trackable.hpp>

struct NewsMessageArea {
    void displayNews() const {}
};

class NewsItem { /* ... */ };

typedef boost::signals2::signal<void (const NewsItem&)> signal_type;
signal_type deliverNews;

boost::signals2::signal<void ()> signal;

int main() {
    // ...
    boost::shared_ptr<NewsMessageArea> newsMessageArea(new NewsMessageArea(/* ... */));
    // ...
    deliverNews.connect(signal_type::slot_type(&NewsMessageArea::displayNews,
        newsMessageArea.get()).track(newsMessageArea));
}
