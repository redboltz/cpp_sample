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
    struct Event1 {
        Event1(int param_):param(param_) {}
        int param;
    };
    struct Event2 {
        Event2(bool result_):result(result_) {}
        bool result;
    };
    struct Event3 {
        Event3(Event2 const& e):result(e.result) {}
        bool result;
    };

    // ----- State machine
    struct Sm1_:msm::front::state_machine_def<Sm1_>
    {
        // States
        struct State1_:msm::front::state_machine_def<State1_>
        {
        public:
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
            struct Exit1:msm::front::exit_pseudo_state<Event3> {};

            // Set initial state
            typedef State1_1 initial_state;

            // Actions
            struct Action1 {
                template <class Event, class Fsm, class SourceState, class TargetState>
                void operator()(Event const& e, Fsm& f, SourceState& s, TargetState& t) const {
                    std::cout << "Action1" << std::endl;
                    f.process_event(Event2(e.param % 2 == 0)); // Can I do that?
                }
            };

            // Transition table
            struct transition_table:mpl::vector<
                //          Start     Event   Next        Action      Guard
                msmf::Row < State1_1, Event1, msmf::none, Action1,    msmf::none >,
                msmf::Row < State1_1, Event2, Exit1,      msmf::none, msmf::none >
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

        struct State3:msm::front::state<> 
        {
            // Entry action
            template <class Event,class Fsm>
            void on_entry(Event const&, Fsm&) const {
                std::cout << "State3::on_entry()" << std::endl;
            }
            // Exit action
            template <class Event,class Fsm>
            void on_exit(Event const&, Fsm&) const {
                std::cout << "State3::on_exit()" << std::endl;
            }
        };

         // Guards
        struct Guard1 {
            template <class Event, class Fsm, class SourceState, class TargetState>
            bool operator()(Event const& e, Fsm&, SourceState&, TargetState&) const 
            {
                std::cout << "Action result is " << e.result << std::endl;
                return e.result;
            }
        };

        // Set initial state
        typedef State1 initial_state;

        // Transition table
        struct transition_table:mpl::vector<
            //          Start             Event        Next    Action      Guard
            msmf::Row < State1::exit_pt 
                       <State1_::Exit1>,  Event3,      State2, msmf::none, Guard1 >,
            msmf::Row < State1::exit_pt 
                       <State1_::Exit1>,  Event3,      State3, msmf::none, msme::Not_<Guard1 > >
        > {};
        // No handled event handler
        template <class Fsm,class Event> 
        void no_transition(Event const& e, Fsm& ,int state) {
            std::cout << "No handled event in State1 " << typeid(e).name() << " on State " << state << std::endl;
        }
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
        {
            std::cout << "=== test case 1 ===" << std::endl;
            Sm1 sm1;
            sm1.start(); 
            std::cout << "> Send Event1(1) odd " << std::endl;
            sm1.process_event(Event1(1));
        }
        {
            std::cout << "=== test case 2 ===" << std::endl;
            Sm1 sm1;
            sm1.start(); 
            std::cout << "> Send Event1(2) even " << std::endl;
            sm1.process_event(Event1(2));
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
// === test case 1 ===
// State1::on_entry()
// State1_1::on_entry()
// > Send Event1(1) odd
// Action1
// State1_1::on_exit()
// Action result is 0
// State1::on_exit()
// State3::on_entry()
// === test case 2 ===
// State1::on_entry()
// State1_1::on_entry()
// > Send Event1(2) even
// Action1
// State1_1::on_exit()
// Action result is 1
// Action result is 1
// State1::on_exit()
// State2::on_entry()
