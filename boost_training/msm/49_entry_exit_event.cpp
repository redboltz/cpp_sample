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
    struct InitialEvent {};
    // ----- State machine
    struct Sm1_:msm::front::state_machine_def<Sm1_>
    {
        typedef InitialEvent initial_event;
        // States
        struct State1:msm::front::state<> 
        {
            // Entry action
            template <class Fsm>
            void on_entry(InitialEvent const&, Fsm&) const {
                std::cout << "State1::on_entry(InitialEvent)" << std::endl;
            }
            template <class Fsm>
            void on_entry(Event2 const&, Fsm&) const {
                std::cout << "State1::on_entry(Event2)" << std::endl;
            }
            // Exit action
            template <class Fsm>
            void on_exit(Event1 const&, Fsm&) const {
                std::cout << "State1::on_exit(Event1)" << std::endl;
            }
        };
        struct State2:msm::front::state<> 
        {
            // Entry action
            template <class Fsm>
            void on_entry(Event1 const&, Fsm&) const {
                std::cout << "State2::on_entry(Event1)" << std::endl;
            }
            // Exit action
            template <class Fsm>
            void on_exit(Event2 const&, Fsm&) const {
                std::cout << "State2::on_exit(Event2)" << std::endl;
            }
        };

        // Set initial state
        typedef State1 initial_state;

        // Transition table
        struct transition_table:mpl::vector<
            //          Start   Event   Next    Action      Guard
            msmf::Row < State1, Event1, State2, msmf::none, msmf::none >,
            msmf::Row < State2, Event2, State1, msmf::none, msmf::none >
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
    }
}

int main()
{
    test();
    return 0;
}

// Output:
//
// State1::on_entry()
// > Send Event1
// State1::on_exit()
