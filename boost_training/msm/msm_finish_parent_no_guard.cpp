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
    struct e2 {};

    // sub state machine
    struct subsm_ : public msm::front::state_machine_def<subsm_>
    {
        template <class Event,class FSM>
        void on_entry(Event const&,FSM& ) {
			std::cout << "subsm::on_entry()" << std::endl;
		}
        template <class Event,class FSM>
        void on_exit(Event const&,FSM& ) {
			std::cout << "subsm::on_exit()" << std::endl;
		}
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
        struct PseudoExit1 : public msm::front::exit_pseudo_state<none> 
        {
            template <class Event,class FSM>
            void on_entry(Event const&,FSM& ) {
				std::cout << "entering: SubFsm2::PseudoExit1" << std::endl;
			}
            template <class Event,class FSM>
            void on_exit(Event const&,FSM& ) {
				std::cout << "leaving: SubFsm2::PseudoExit1" << std::endl;
			}
        };

        // the initial state of the player SM. Must be defined
        typedef S1 initial_state;

        // Transition table for player
        struct transition_table : mpl::vector<
            //    Start     Event Next              Action               Guard
            Row < S1,       e1,   PseudoExit1,      none,                none                  >
            //  +---------+---- -+-----------------+---------------------+----------------------+
        > {};
    };
	typedef msm::back::state_machine<subsm_> subsm;
	
    // state machine
    struct sm_ : public msm::front::state_machine_def<sm_>
    {
		struct End : msm::front::terminate_state<>
		{
		};

        // the initial state of the player SM. Must be defined
        typedef subsm initial_state;

        // Transition table for player
        struct transition_table : mpl::vector<
            //    Start                Event       Next      Action               Guard
			Row <subsm::exit_pt
			    <subsm_::PseudoExit1>, none,       End,      none,                none                  >
            //  +---------------------+-------------+---------+---------------------+----------------------+
        > {};
	};
	
    // Pick a back-end
    typedef msm::back::state_machine<sm_> sm1;

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
