#include <iostream>
#include <boost/msm/back/state_machine.hpp>

#include <boost/msm/front/state_machine_def.hpp>
#include <boost/msm/front/functor_row.hpp>


namespace {
    namespace msm = boost::msm;
    namespace msmf = boost::msm::front;
    namespace mpl = boost::mpl;

    // ----- Events
    struct Event1 {};

    // ----- State machine
    struct Sm1_:public msm::front::state_machine_def<Sm1_>
    {
        // States
        struct Init:msm::front::state<> {};
        struct State1:msm::front::state<> 
        {
            // Entry action
            template <class Event,class Fsm>
            void on_entry(Event const&, Fsm&) {
                std::cout << "State1::on_entry()" << std::endl;
            }
            // Exit action
            template <class Event,class Fsm>
            void on_exit(Event const&, Fsm&) {
                std::cout << "State1::on_exit()" << std::endl;
            }
        };

        // Set initial state
        typedef Init initial_state;

        // Actions
        struct InitAction {
            template <class Event, class Fsm, class SourceState, class TargetState>
            void operator()(Event const&, Fsm&, SourceState&, TargetState&)
            {
                std::cout << "InitAction()" << std::endl;
            }
        };

        // Transition table
        struct transition_table:mpl::vector<
            //          Start   Event       Next    Action      Guard
            msmf::Row < Init,   msmf::none, State1, InitAction, msmf::none >,
            msmf::Row < State1, Event1,     State1, msmf::none, msmf::none >
        > {};
    };

    // Pick a back-end
    typedef msm::back::state_machine<Sm1_> Sm1;

    void test()
    {        
        Sm1 sm1;
        sm1.start(); 
        std::cout << "> Send Event1" << std::endl;
        sm1.process_event(Event1());
    }
}

int main()
{
    test();
    return 0;
}

// Output:
//
// InitAction()
// State1::on_entry()
// > Send Event1
// State1::on_exit()
// State1::on_entry()
