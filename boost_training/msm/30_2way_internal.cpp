#include <iostream>
#include <boost/msm/back/state_machine.hpp>

#include <boost/msm/front/state_machine_def.hpp>
#include <boost/msm/front/functor_row.hpp>

namespace {
    namespace msm = boost::msm;
    namespace msmf = boost::msm::front;
    namespace mpl = boost::mpl;

    // Events
    struct Event1 {};

    // ----- State machine
    struct Sm1_:msm::front::state_machine_def<Sm1_>
    {
        struct State1_:msm::front::state_machine_def<State1_>
        {
             // Guards
            struct InternalGuard1 {
                template <class Event, class Fsm, class SourceState, class TargetState>
                bool operator()(Event const&, Fsm&, SourceState&, TargetState&) const 
                {
                    std::cout << "Internal Transition Table's Guard1" << std::endl;
                    return false;
                }
            };
            struct InternalGuard2 {
                template <class Event, class Fsm, class SourceState, class TargetState>
                bool operator()(Event const&, Fsm&, SourceState&, TargetState&) const 
                {
                    std::cout << "Internal Transition Table's Guard2" << std::endl;
                    return false;
                }
            };

            // Internal Transition table
            struct internal_transition_table:mpl::vector<
                //               Event   Action      Guard
                msmf::Internal < Event1, msmf::none, InternalGuard1 >,
                msmf::Internal < Event1, msmf::none, InternalGuard2 >
            > {};        
        };

        // Set initial state
        typedef State1_ initial_state;

         // Guards
        struct Guard1 {
            template <class Event, class Fsm, class SourceState, class TargetState>
            bool operator()(Event const&, Fsm&, SourceState&, TargetState&) const 
            {
                std::cout << "Transition Table's Guard1" << std::endl;
                return false;
            }
        };
        struct Guard2 {
            template <class Event, class Fsm, class SourceState, class TargetState>
            bool operator()(Event const&, Fsm&, SourceState&, TargetState&) const 
            {
                std::cout << "Transition Table's Guard2" << std::endl;
                return false;
            }
        };

        // Transition table
        struct transition_table:mpl::vector<
            //          Start    Event   Next        Action      Guard
            msmf::Row < State1_, Event1, msmf::none, msmf::none, Guard1 >,
            msmf::Row < State1_, Event1, msmf::none, msmf::none, Guard2 >
        > {};
    };

    // back-end
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
// > Send Event1
// Internal Transition Table's Guard2
// Internal Transition Table's Guard1
// Transition Table's Guard2
// Transition Table's Guard1
