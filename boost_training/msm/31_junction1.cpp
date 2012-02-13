#include <iostream>
#include <boost/msm/back/state_machine.hpp>

#include <boost/msm/front/state_machine_def.hpp>
#include <boost/msm/front/functor_row.hpp>

namespace {
    namespace msm = boost::msm;
    namespace msmf = boost::msm::front;
    namespace mpl = boost::mpl;

    // Events
    struct Event1 {
        Event1(int val):val(val) {}
        int val;
    };

    // ----- State machine
    struct Sm1_:msm::front::state_machine_def<Sm1_>
    {
        struct State1_:msm::front::state_machine_def<State1_>{};

        // Set initial state
        typedef State1_ initial_state;

         // Guards
        struct Guard1 {
            template <class Event, class Fsm, class SourceState, class TargetState>
            bool operator()(Event const& e, Fsm&, SourceState&, TargetState&) const 
            {
                if (e.val == 1) return true;
                return false;
            }
        };
        struct Guard2 {
            template <class Event, class Fsm, class SourceState, class TargetState>
            bool operator()(Event const& e, Fsm&, SourceState&, TargetState&) const 
            {
                if (e.val == 2) return true;
                return false;
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

        // Transition table
        struct transition_table:mpl::vector<
            //          Start    Event   Next     Action   Guard
            msmf::Row < State1_, Event1, State1_, Action1, Guard1 >,
            msmf::Row < State1_, Event1, State1_, Action2, Guard2 >
        > {};
    };

    // back-end
    typedef msm::back::state_machine<Sm1_> Sm1;

    void test()
    {
        Sm1 sm1;
        sm1.start(); 
        std::cout << "> Send Event1(1)" << std::endl;
        sm1.process_event(Event1(1));
        std::cout << "> Send Event1(2)" << std::endl;
        sm1.process_event(Event1(2));
    }
}

int main()
{
    test();
    return 0;
}

// Output:
//
// > Send Event1(1)
// Action1
// > Send Event1(2)
// Action2
