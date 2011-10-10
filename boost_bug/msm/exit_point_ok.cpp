#include <iostream>
#include <boost/msm/back/state_machine.hpp>

#include <boost/msm/front/state_machine_def.hpp>
#include <boost/msm/front/functor_row.hpp>

namespace {
    namespace msm = boost::msm;
    namespace msmf = boost::msm::front;
    namespace mpl = boost::mpl;
    // ----- Events
    struct Event1 { int info; };
#if 0
    struct Event2 {
        Event2() {}
        Event2(Event1 const&) {
            std::cout << "Event2 constructed from Event1" << std::endl;
        }
    };
#endif
    struct Event2 {
        Event2() {}
        template <class Event>
        Event2(Event const& e):info(e.info) {}
        int info;
    };
    struct Event3 {};

    // ----- State machine
    struct Sm1_:public msm::front::state_machine_def<Sm1_>
    {
        // States
        struct State1_:public msm::front::state_machine_def<State1_>
        {
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

            // States
            struct State1_1:msm::front::state<> 
            {
                // Entry action
                template <class Event,class Fsm>
                void on_entry(Event const&, Fsm&) const {
                    std::cout << "State1_1::on_entry()" << std::endl;
                }
                // Exit action
                template <class Event,class Fsm>
                void on_exit(Event const&, Fsm&) const {
                    std::cout << "State1_1::on_exit()" << std::endl;
                }
            };
            struct Exit1:public msm::front::exit_pseudo_state<Event2> {};
            struct Exit2:public msm::front::exit_pseudo_state<msmf::none> {};

            // Set initial state
            typedef State1_1 initial_state;

            // Actions
            struct Action1 {
                template <class Event, class Fsm, class SourceState, class TargetState>
                void operator()(Event const&, Fsm&, SourceState&, TargetState&) const 
                {
                    std::cout << "Action1" << std::endl;
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
                //          Start     Event   Next   Action      Guard
                msmf::Row < State1_1, Event1, Exit1, Action1,    msmf::none >,
                msmf::Row < State1_1, Event3, Exit2, Action3,    msmf::none >
            > {};
        };
        // back-end
        typedef msm::back::state_machine<State1_> State1;

        struct State2:msm::front::state<> 
        {
            // Entry action
            template <class Event,class Fsm>
            void on_entry(Event const&, Fsm&) const {
                std::cout << "State2::on_entry()" << std::endl;
            }
            // Exit action
            template <class Event,class Fsm>
            void on_exit(Event const&, Fsm&) const {
                std::cout << "State2::on_exit()" << std::endl;
            }
        };

        // Actions
        struct Action2 {
            template <class Event, class Fsm, class SourceState, class TargetState>
            void operator()(Event const&, Fsm&, SourceState&, TargetState&) const 
            {
                std::cout << "Action2" << std::endl;
            }
        };
        struct Action4 {
            template <class Event, class Fsm, class SourceState, class TargetState>
            void operator()(Event const&, Fsm&, SourceState&, TargetState&) const 
            {
                std::cout << "Action4" << std::endl;
            }
        };

        // Set initial state
        typedef State1 initial_state;

        // Transition table
        struct transition_table:mpl::vector<
            //          Start             Event       Next    Action      Guard
            msmf::Row < State1::exit_pt 
                       <State1_::Exit1>,  Event2,     State2, Action2,    msmf::none >,
            msmf::Row < State1::exit_pt 
                       <State1_::Exit2>,  msmf::none, State2, Action4,    msmf::none >
        > {};
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
        {
            std::cout << "=== test case 1 ===" << std::endl;
            Sm1 sm1;
            sm1.start(); 
            std::cout << "> Send Event1" << std::endl;
            sm1.process_event(Event1());
        }
        {
            std::cout << "=== test case 2 ===" << std::endl;
            Sm1 sm1;
            sm1.start(); 
            std::cout << "> Send Event2" << std::endl;
            sm1.process_event(Event2());
        }
        {
            std::cout << "=== test case 3 ===" << std::endl;
            Sm1 sm1;
            sm1.start(); 
            std::cout << "> Send Event3" << std::endl;
            sm1.process_event(Event3());
        }
    }
}

int main()
{
    test();
    return 0;
}
