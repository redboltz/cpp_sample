#include <iostream>
#include <boost/current_function.hpp>

#include <boost/msm/back/state_machine.hpp>

#include <boost/msm/front/state_machine_def.hpp>
#include <boost/msm/front/functor_row.hpp>

namespace msm = boost::msm;
namespace mpl = boost::mpl;
using namespace msm::front;

namespace
{
    // events
    struct e1 {};

    // state machine
    struct sm1_ : public msm::front::state_machine_def<sm1_>
    {
        // The list of FSM states
        struct S1 : msm::front::state<> 
        {
            // every (optional) entry/exit methods get the event passed.
            template <class Event,class FSM>
            void on_entry(Event const&,FSM& ) {
				std::cout << "S1::on_entry()" << std::endl;
			}
            template <class Event,class FSM>
            void on_exit(Event const&,FSM& ) {
				std::cout << "S1::on_exit()" << std::endl;
			}
        };
		struct End : msm::front::terminate_state<>
		{
		};

        // the initial state of the player SM. Must be defined
        typedef S1 initial_state;

        // Transition table for player
        struct transition_table : mpl::vector<
            //    Start     Event         Next      Action               Guard
            Row < S1,       e1,           End,      none,                none                  >
            //  +---------+-------------+---------+---------------------+----------------------+
        > {};
    };
    // Pick a back-end
    typedef msm::back::state_machine<sm1_> sm1;

    void test()
    {        
        sm1 sm;
        // needed to start the highest-level SM. This will call on_entry and mark the start of the SM
        sm.start(); 
		sm.process_event(e1());
    }
}

int main()
{
    test();
    return 0;
}
