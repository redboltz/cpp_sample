#include <iostream>
#include <boost/msm/back/state_machine.hpp>

#include <boost/msm/front/state_machine_def.hpp>
#include <boost/msm/front/functor_row.hpp>
#include <boost/msm/front/euml/euml.hpp>

namespace {
    namespace msm = boost::msm;
    namespace msmf = boost::msm::front;
    namespace msme = boost::msm::front::euml;
    namespace mpl = boost::mpl;

    // Events
    struct Event1 {};

    // ----- State machine
    struct Sm1_:msm::front::state_machine_def<Sm1_>
    {

        struct State1_:msm::front::state_machine_def<State1_>
        {
            struct State1_1:msm::front::state<> {
                 // Guards
                struct Guard1_1_1 {
                    template <class Event, class Fsm, class SourceState, class TargetState>
                    bool operator()(Event const&, Fsm&, SourceState&, TargetState&) const 
                    {
                        std::cout << "Guard1_1_1" << std::endl;
                        return false;
                    }
                };
                struct Guard1_1_2 {
                    template <class Event, class Fsm, class SourceState, class TargetState>
                    bool operator()(Event const&, Fsm&, SourceState&, TargetState&) const 
                    {
                        std::cout << "Guard1_1_2" << std::endl;
                        return false;
                    }
                };
                typedef int evaluation_forward;
                // Internal Transition table
                struct internal_transition_table : mpl::vector<
                    //               Event   Action      Guard
                    msmf::Internal < Event1, msmf::none, Guard1_1_1 >,
                    msmf::Internal < Event1, msmf::none, Guard1_1_2 >
                > {};        
            };
            struct State1_2:msm::front::state<> {};
             // Guards
            struct Guard1_1 {
                template <class Event, class Fsm, class SourceState, class TargetState>
                bool operator()(Event const&, Fsm&, SourceState&, TargetState&) const 
                {
                    std::cout << "Guard1_1" << std::endl;
                    return false;
                }
            };
            struct Guard1_2 {
                template <class Event, class Fsm, class SourceState, class TargetState>
                bool operator()(Event const&, Fsm&, SourceState&, TargetState&) const 
                {
                    std::cout << "Guard1_2" << std::endl;
                    return false;
                }
            };
            // Set initial state
            typedef State1_1 initial_state;
            typedef int evaluation_forward;

            // Transition table
            struct transition_table:mpl::vector<
                //          Start     Event   Next        Action      Guard
                msmf::Row < State1_1, Event1, State1_1,   msmf::none, Guard1_1 >,
                msmf::Row < State1_1, Event1, State1_2,   msmf::none, Guard1_2 >
            > {};
        };
        // back-end
        typedef msm::back::state_machine<State1_> State1;
        struct State2:msm::front::state<> {};

         // Guards
        struct Guard1 {
            template <class Event, class Fsm, class SourceState, class TargetState>
            bool operator()(Event const&, Fsm&, SourceState&, TargetState&) const 
            {
                std::cout << "Guard1" << std::endl;
                return false;
            }
        };
        struct Guard2 {
            template <class Event, class Fsm, class SourceState, class TargetState>
            bool operator()(Event const&, Fsm&, SourceState&, TargetState&) const 
            {
                std::cout << "Guard2" << std::endl;
                return false;
            }
        };

        // Set initial state
        typedef State1 initial_state;
        typedef int evaluation_forward;

        // Transition table
        struct transition_table:mpl::vector<
            //          Start   Event   Next        Action      Guard
            msmf::Row < State1, Event1, State1,     msmf::none, Guard1 >,
            msmf::Row < State1, Event1, State2,     msmf::none, Guard2 >
        > {};
    };

    // back-end
    typedef msm::back::state_machine<Sm1_> Sm1;

    // No handled event handler
    template <class Fsm,class Event> 
    void no_transition(Event const& e, Fsm& ,int state) {
        std::cout << "No handled event in Sm1 " << typeid(e).name() << " on State " << state << std::endl;
    }

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
// Guard1_1_2
// Guard1_1_1
// Guard1_2
// Guard1_1
// Guard1_1_2
// Guard1_1_1
// Guard2
// Guard1


