#include <boost/asio.hpp>
#include <boost/phoenix.hpp>
#include <boost/thread.hpp>

namespace phx = boost::phoenix;
namespace asio = boost::asio;


class thread_pool
{
public:
    thread_pool(size_t num_of_threads): service_(num_of_threads), work_(service_) {
        for (size_t i = 0; i < num_of_threads; ++i) { 
            pool_.create_thread(phx::bind(&asio::io_service::run, &service_)); 
        }
    }

    ~thread_pool() {
        service_.stop();
        pool_.join_all();
    }

    template<typename F> void add_task(F f) {
        service_.post(f);
    }

protected:
    boost::thread_group pool_;
    asio::io_service service_;
    asio::io_service::work work_;
};

int main() {
    auto t1 = []{ for (int i = 0; i < 1000; ++i) std::cout << "A" << std::endl; };
    auto t2 = []{ for (int i = 0; i < 1000; ++i) std::cout << "B" << std::endl; };
    auto t3 = []{ for (int i = 0; i < 1000; ++i) std::cout << "C" << std::endl; };
    auto t4 = []{ for (int i = 0; i < 1000; ++i) std::cout << "D" << std::endl; };
    auto t5 = []{ for (int i = 0; i < 1000; ++i) std::cout << "E" << std::endl; };
    auto t6 = []{ for (int i = 0; i < 1000; ++i) std::cout << "F" << std::endl; };
    auto t7 = []{ for (int i = 0; i < 1000; ++i) std::cout << "G" << std::endl; };
    thread_pool p(2);
    p.add_task(t1);
    p.add_task(t2);
    p.add_task(t3);
    p.add_task(t4);
    p.add_task(t5);
    p.add_task(t6);
    p.add_task(t7);
}

