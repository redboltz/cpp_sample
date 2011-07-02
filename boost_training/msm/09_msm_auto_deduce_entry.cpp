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

    // ----- State machine
    struct Sm1_:msm::front::state_machine_def<Sm1_>
    {
        // States
        struct State1:msm::front::state<> {};
        struct State2_:msm::front::state_machine_def<State2_>
        {
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
            struct Entry1:msm::front::entry_pseudo_state<> {}; // Omit!
            struct State2_A : msm::front::state<> {
                // Entry action
                template <class Event,class Fsm>
                void on_entry(Event const&, Fsm&) {
                    std::cout << "State2_A::on_entry()" << std::endl;
                }
                // Exit action
                template <class Event,class Fsm>
                void on_exit(Event const&, Fsm&) {
                    std::cout << "State2_A::on_exit()" << std::endl;
                }
            };
            struct State2_B_1 : msm::front::state<> {
                // Entry action
                template <class Event,class Fsm>
                void on_entry(Event const&, Fsm&) {
                    std::cout << "State2_B_1::on_entry()" << std::endl;
                }
                // Exit action
                template <class Event,class Fsm>
                void on_exit(Event const&, Fsm&) {
                    std::cout << "State2_B_1::on_exit()" << std::endl;
                }
            };
            struct State2_B_2 : msm::front::state<> {
                // Entry action
                template <class Event,class Fsm>
                void on_entry(Event const&, Fsm&) {
                    std::cout << "State2_B_2::on_entry()" << std::endl;
                }
                // Exit action
                template <class Event,class Fsm>
                void on_exit(Event const&, Fsm&) {
                    std::cout << "State2_B_2::on_exit()" << std::endl;
                }
            };
            typedef mpl::vector<State2_A, State2_B_2> initial_state;
            // Transition table
            struct transition_table:mpl::vector<
                //          Start       Event       Next        Action      Guard
                msmf::Row < Entry1,     msmf::none, State2_B_1, msmf::none, msmf::none >,
                msmf::Row < State2_B_2, Event1,     State2_B_1, msmf::none, msmf::none >
            > {};
        };
        typedef msm::back::state_machine<State2_> State2;

        // Set initial state
        typedef State1 initial_state;

        // Transition table
        struct transition_table:mpl::vector<
            //          Start   Event       Next                Action      Guard
            msmf::Row < State1, Event1,     State2::entry_pt
                                            <State2_::Entry1>,  msmf::none, msmf::none >
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
    }
}

int main()
{
    test();
    return 0;
}

// Output:
//
