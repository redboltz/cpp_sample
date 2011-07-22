#include <iostream>
#include <boost/msm/back/state_machine.hpp>

#include <boost/msm/front/state_machine_def.hpp>
#include <boost/msm/front/functor_row.hpp>

#include <boost/msm/front/euml/euml.hpp>

namespace {
    namespace msm = boost::msm;
    namespace msmf = boost::msm::front;
    namespace mpl = boost::mpl;
    namespace msme = boost::msm::front::euml;

    // ----- Events
    struct Event1 { 
        int param; 
        Event1(int param_):param(param_) {}
    };

    // ----- State machine
    struct Sm1_:msm::front::state_machine_def<Sm1_>
    {
        // States
        struct State1:msm::front::state<> {};
        struct State2:msm::front::state<> {};

        // Set initial state
        typedef State1 initial_state;

        // Guards
        struct Guard1 {
            template <class Event, class Fsm, class SourceState, class TargetState>
            bool operator()(Event const& e, Fsm&, SourceState&, TargetState&) const 
            {
                return e.param == 1;
            }
        };
        struct Guard2 {
            template <class Event, class Fsm, class SourceState, class TargetState>
            bool operator()(Event const& e, Fsm&, SourceState&, TargetState&) const 
            {
                return e.param == 2;
            }
        };
        // Actions
        struct Action1 {
            template <class Event, class Fsm, class SourceState, class TargetState>
            void operator()(Event const&, Fsm&, SourceState&, TargetState&) const 
            {
                std::cout << "Action1" << std::endl;
            }
        };
        struct Action2 {
            template <class Event, class Fsm, class SourceState, class TargetState>
            void operator()(Event const&, Fsm&, SourceState&, TargetState&) const 
            {
                std::cout << "Action2" << std::endl;
            }
        };
        struct Action3 {
            template <class Event, class Fsm, class SourceState, class TargetState>
            void operator()(Event const&, Fsm&, SourceState&, TargetState&) const 
            {
                std::cout << "Action3" << std::endl;
            }
        };

        // Transition table
        struct transition_table:mpl::vector<
            //          Start   Event   Next    Action   Guard
            msmf::Row < State1, Event1, State2, Action3, msmf::none /* else */ >,
            msmf::Row < State1, Event1, State2, Action2, Guard2 >,
            msmf::Row < State1, Event1, State2, Action1, Guard1 >
        > {};
    };

    // Pick a back-end
    typedef msm::back::state_machine<Sm1_> Sm1;

    void test()
    {
        {
            std::cout << "=== test1 ===" << std::endl;
            Sm1 sm1;
            sm1.start(); 
            std::cout << "> Send Event1(1)" << std::endl;
            sm1.process_event(Event1(1));
        }
        {
            std::cout << "=== test2 ===" << std::endl;
            Sm1 sm1;
            sm1.start(); 
            std::cout << "> Send Event1(2)" << std::endl;
            sm1.process_event(Event1(2));
        }
        {
            std::cout << "=== test3 ===" << std::endl;
            Sm1 sm1;
            sm1.start(); 
            std::cout << "> Send Event1(3)" << std::endl;
            sm1.process_event(Event1(3));
        }
    }
}

int main()
{
    test();
    return 0;
}

// Output:
// 
// === test1 ===
// > Send Event1(1)
// Action1
// === test2 ===
// > Send Event1(2)
// Action2
// === test3 ===
// > Send Event1(3)
// Action3
