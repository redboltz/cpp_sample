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

    // ----- State machine
    struct Sm1_:msm::front::state_machine_def<Sm1_>
    {
        // States
        struct State1_:msm::front::state_machine_def<State1_>
        {
            struct Entry1:msm::front::entry_pseudo_state<> {};
            struct Entry2:msm::front::entry_pseudo_state<0> {};
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
            typedef State1_1 initial_state;
            // Transition table
            struct transition_table:mpl::vector<
                //          Start       Event           Next        Action      Guard
                msmf::Row < Entry1,     msmf::none,     State1_1,   msmf::none, msmf::none >,
                msmf::Row < Entry2,     msmf::none,     State1_2,   msmf::none, msmf::none >
            > {};
        };
        typedef msm::back::state_machine<State1_> State1;

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

        // Set initial state
        typedef State1 initial_state;

        // Transition table
        struct transition_table:mpl::vector<
            //          Start   Event       Next                Action      Guard
            msmf::Row < State1, Event3,     State2,             msmf::none, msmf::none >,
            msmf::Row < State2, Event1,     State1::entry_pt
                                            <State1_::Entry1>,  msmf::none, msmf::none >,
            msmf::Row < State2, Event2,     State1::entry_pt
                                            <State1_::Entry2>,  msmf::none, msmf::none >
        > {};
    };

    // Pick a back-end
    typedef msm::back::state_machine<Sm1_> Sm1;

    void test()
    {
        {        
            std::cout << "=== test case 1 ===" << std::endl;
            Sm1 sm1;
            sm1.start(); 
            std::cout << "> Send Event3" << std::endl;
            sm1.process_event(Event3());
            std::cout << "> Send Event1" << std::endl;
            sm1.process_event(Event1());
        }
        {        
            std::cout << "=== test case 2 ===" << std::endl;
            Sm1 sm1;
            sm1.start(); 
            std::cout << "> Send Event3" << std::endl;
            sm1.process_event(Event3());
            std::cout << "> Send Event2" << std::endl;
            sm1.process_event(Event2());
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
