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
    struct Event2 {};

    // ----- State machine
    struct Sm1_:msm::front::state_machine_def<Sm1_>
    {
        // States
        struct State1:msm::front::state<> 
        {
            State1():bVal(false) {}
            // Entry action
            template <class Event,class Fsm>
            void on_entry(Event const&, Fsm&) const {
                std::cout << "State1::on_entry()" << std::endl;
            }
            // Exit action
            template <class Event,class Fsm>
            void on_exit(Event const&, Fsm&) const {
                std::cout << "State1::on_exit()" << std::endl;
            }
            bool bVal;
        };
        struct State2:msm::front::state<> 
        {
            // Entry action
            template <class Event,class Fsm>
            void on_entry(Event const&, Fsm& fsm) const {
                std::cout << "State2::on_entry()" << std::endl;
            }
            // Exit action
            template <class Event,class Fsm>
            void on_exit(Event const&, Fsm&) const {
                std::cout << "State2::on_exit()" << std::endl;
            }
        };
         // Guards
        struct Guard1 {
            template <class Event, class Fsm, class SourceState, class TargetState>
            bool operator()(Event const&, Fsm&, SourceState& s, TargetState&) const 
            {
                return s.bVal;
            }
        };

        // Actions
        struct Action1 {
            template <class Event, class Fsm, class SourceState, class TargetState>
            void operator()(Event const&, Fsm& fsm, SourceState& s, TargetState&) const 
            {
                s.bVal = true;
            }
        };
        struct Action2 {
            template <class Event, class Fsm, class SourceState, class TargetState>
            void operator()(Event const&, Fsm& fsm, SourceState& s, TargetState&) const 
            {
                s.bVal = true;
            }
        };

        // Set initial state
        typedef State1 initial_state;
        // Enable deferred capability
        typedef int activate_deferred_events;
        // Transition table
        struct transition_table:mpl::vector<
            //          Start   Event       Next        Action       Guard
            msmf::Row < State1, msmf::none, State2,     msmf::none,  Guard1     >,
            msmf::Row < State1, Event1,     State1,     Action1,     msmf::none >,
            msmf::Row < State1, Event2,     msmf::none, Action2,     msmf::none >
        > {};
    };

    // Pick a back-end
    typedef msm::back::state_machine<Sm1_> Sm1;

    void test()
    {        
        Sm1 sm1;
        sm1.start(); 
        std::cout << "> Send Event2" << std::endl;
        sm1.process_event(Event2());
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
// State1::on_entry()
// > Send Event1
// > Send Event2
// State1::on_exit()
// State2::on_entry()
// State2::on_exit()
// State3::on_entry()
