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
    struct Event3 {};
    struct Event4 {};

    // ----- State machine
    struct Sm1_:public msm::front::state_machine_def<Sm1_>
    {
        // States
        struct Init:msm::front::state<> {};
        struct State1_:public msm::front::state_machine_def<State1_>
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
            struct Entry1:public msm::front::entry_pseudo_state<> {};
            struct State1_1 : msm::front::state<> {
                // Entry action
                template <class Event,class Fsm>
                void on_entry(Event const&, Fsm&) {
                    std::cout << "State1_1::on_entry()" << std::endl;
                }
                // Exit action
                template <class Event,class Fsm>
                void on_exit(Event const&, Fsm&) {
                    std::cout << "State1_1::on_exit()" << std::endl;
                }
            };
            struct State1_2 : msm::front::state<> {
                // Entry action
                template <class Event,class Fsm>
                void on_entry(Event const&, Fsm&) {
                    std::cout << "State1_2::on_entry()" << std::endl;
                }
                // Exit action
                template <class Event,class Fsm>
                void on_exit(Event const&, Fsm&) {
                    std::cout << "State1_2::on_exit()" << std::endl;
                }
            };
            struct State1_3 : msm::front::state<> {
                // Entry action
                template <class Event,class Fsm>
                void on_entry(Event const&, Fsm&) {
                    std::cout << "State1_3::on_entry()" << std::endl;
                }
                // Exit action
                template <class Event,class Fsm>
                void on_exit(Event const&, Fsm&) {
                    std::cout << "State1_3::on_exit()" << std::endl;
                }
            };
            typedef State1_1 initial_state;
            // Transition table
            struct transition_table:mpl::vector<
                //          Start       Event       Next        Action      Guard
                msmf::Row < Entry1,     msmf::none, State1_3,   msmf::none, msmf::none >,
                msmf::Row < State1_1,   Event2,     State1_2,   msmf::none, msmf::none >,
                msmf::Row < State1_2,   Event2,     State1_3,   msmf::none, msmf::none >
            > {};
        };
        typedef msm::back::state_machine<State1_, msm::back::AlwaysHistory> State1;
        struct State2:msm::front::state<> {
            // Entry action
            template <class Event,class Fsm>
            void on_entry(Event const&, Fsm&) {
                std::cout << "State2::on_entry()" << std::endl;
            }
            // Exit action
            template <class Event,class Fsm>
            void on_exit(Event const&, Fsm&) {
                std::cout << "State2::on_exit()" << std::endl;
            }
        };
        struct State3:msm::front::state<> {
            // Entry action
            template <class Event,class Fsm>
            void on_entry(Event const&, Fsm&) {
                std::cout << "State3::on_entry()" << std::endl;
            }
            // Exit action
            template <class Event,class Fsm>
            void on_exit(Event const&, Fsm&) {
                std::cout << "State3::on_exit()" << std::endl;
            }
        };

        // Set initial state
        typedef State2 initial_state;

        // Transition table
        struct transition_table:mpl::vector<
            //          Start   Event       Next                Action      Guard
            msmf::Row < State2, Event1,     State1,             msmf::none, msmf::none >,
            msmf::Row < State3, Event4,     State1,             msmf::none, msmf::none >,
            msmf::Row < State3, Event1,     State1::entry_pt
                                            <State1_::Entry1>,  msmf::none, msmf::none >,
            msmf::Row < State1, Event3,     State3,             msmf::none, msmf::none >
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
        std::cout << "> Send Event2" << std::endl;
        sm1.process_event(Event2());
        std::cout << "> Send Event3" << std::endl;
        sm1.process_event(Event3());
        std::cout << "> Send Event4" << std::endl;
        sm1.process_event(Event4());
        std::cout << "> Send Event3" << std::endl;
        sm1.process_event(Event3());
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
// State2::on_entry()
// > Send Event1
// State2::on_exit()
// State1::on_entry()
// State1_1::on_entry()
// > Send Event2
// State1_1::on_exit()
// State1_2::on_entry()
// > Send Event3
// State1_2::on_exit()
// State1::on_exit()
// State3::on_entry()
// > Send Event4
// State3::on_exit()
// State1::on_entry()
// State1_2::on_entry()
// > Send Event3
// State1_2::on_exit()
// State1::on_exit()
// State3::on_entry()
// > Send Event1
// State3::on_exit()
// State1::on_entry()
// State1_2::on_entry()
