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
    struct Event2 {};

    // ----- State machine
    struct Sm1_:msm::front::state_machine_def<Sm1_>
    {
        struct State1_:msm::front::state_machine_def<State1_>
        {
             // Guards
            struct Guard1_1 {
                template <class Event, class Fsm, class SourceState, class TargetState>
                bool operator()(Event const&, Fsm&, SourceState&, TargetState&) const 
                {
                    std::cout << "Guard1_1" << std::endl;
                    return true;
                }
            };
            // Actions
            struct Action1_1 {
                template <class Event, class Fsm, class SourceState, class TargetState>
                void operator()(Event const&, Fsm&, SourceState&, TargetState&) const 
                {
                    std::cout << "Action1_1" << std::endl;
                }
            };
            struct Guard1_2 {
                template <class Event, class Fsm, class SourceState, class TargetState>
                bool operator()(Event const&, Fsm&, SourceState&, TargetState&) const 
                {
                    std::cout << "Guard1_2" << std::endl;
                    return true;
                }
            };
            // Actions
            struct Action1_2 {
                template <class Event, class Fsm, class SourceState, class TargetState>
                void operator()(Event const&, Fsm&, SourceState&, TargetState&) const 
                {
                    std::cout << "Action1_2" << std::endl;
                }
            };
            struct State1_1;
            // Set initial state
            typedef State1_1 initial_state;
            // States
            struct State1_1:msm::front::state<> {
                // Internal Transition table
                struct internal_transition_table : mpl::vector<
                    //               Event   Action      Guard
                    msmf::Internal < Event1, Action1_1  ,Guard1_1 >,
                    msmf::Internal < Event1, Action1_1  ,Guard1_1 >
                > {};        
            };
            struct State1_2:msm::front::state<> {};

            // Transition table
            struct transition_table:mpl::vector<
                //          Start     Event   Next        Action      Guard
                msmf::Row < State1_1, Event1, msmf::none, Action1_2,  Guard1_2 > 
            > {};
            // No handled event handler
            template <class Fsm,class Event> 
            void no_transition(Event const& e, Fsm& ,int state) {
                std::cout << "No handled event in State1_1 " << typeid(e).name() << " on State " << state << std::endl;
            }
        };
        // back-end
        typedef msm::back::state_machine<State1_> State1;

        // Set initial state
        typedef State1 initial_state;

        // No handled event handler
        template <class Fsm,class Event> 
        void no_transition(Event const& e, Fsm& ,int state) {
            std::cout << "No handled event in Sm1 " << typeid(e).name() << " on State " << state << std::endl;
        }
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
