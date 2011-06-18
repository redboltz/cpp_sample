#include <iostream>
#include <boost/msm/back/state_machine.hpp>

#include <boost/msm/front/state_machine_def.hpp>
#include <boost/msm/front/functor_row.hpp>

namespace msm = boost::msm;
namespace mpl = boost::mpl;
using namespace msm::front;

namespace {
	using namespace msm::front;

	// ----- Events
    struct Event1 {};

    // ----- State machine
    struct Sm1_:public msm::front::state_machine_def<Sm1_>
    {
        // States
        struct State1:msm::front::state<> 
        {
            // Entry action
            template <class Event,class FSM>
            void on_entry(Event const&, FSM&) {
				std::cout << "State1::on_entry()" << std::endl;
			}
            // Exit action
            template <class Event,class FSM>
            void on_exit(Event const&, FSM&) {
				std::cout << "State1::on_exit()" << std::endl;
			}
        };
		struct End:msm::front::terminate_state<> {};

        // Set initial state
        typedef State1 initial_state;

        // Transition table
        struct transition_table:mpl::vector<
            //    Start		Event	Next	Action	Guard
            Row < State1,	Event1,	End,	none,	none >
        > {};
    };

	// Pick a back-end
    typedef msm::back::state_machine<Sm1_> Sm1;

    void test()
    {        
        Sm1 sm1;
        sm1.start(); 
		sm1.process_event(Event1());
    }
}

int main()
{
    test();
    return 0;
}
