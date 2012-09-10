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
    struct Sub_:msm::front::state_machine_def<Sub_>
    {
        struct SubState1:msmf::state<> {
            // Entry action
            template <class Event>
            void on_entry(Event const&, Sub_&) const {}
        };
        typedef SubState1 initial_state;
        // Transition table
        struct transition_table:mpl::vector<
            //          Start   Event   Next    Action   Guard
        > {};
    };
    typedef msm::back::state_machine<Sub_> Sub;

    struct Sm1_:msm::front::state_machine_def<Sm1_>
    {
        // States
        struct State1:Sub
        {
            template <class Event, class Fsm>
            void on_entry(Event const&, Fsm&) const {
                std::cout << "State1::on_entry(Event1)" << std::endl;
            }
        };

        // Set initial state
        typedef State1 initial_state;

        // Transition table
        struct transition_table:mpl::vector<
            //          Start   Event   Next    Action   Guard
//            msmf::Row < State2, Event2, State1, Action1, Guard1 >
        > {};
    };

    // Pick a back-end
    typedef msm::back::state_machine<Sm1_> Sm1;

    void test()
    {        
        Sm1 sm1;
        sm1.start(); 
//        std::cout << "> Send Event1" << std::endl;
//        sm1.process_event(Event1());
    }
}

int main()
{
    test();
    return 0;
}

// Output:
//
