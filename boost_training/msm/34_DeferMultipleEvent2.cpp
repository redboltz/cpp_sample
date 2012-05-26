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

    // ----- State machine
    struct Sm1_:msmf::state_machine_def<Sm1_>
    {
        // States
        struct State1:msmf::state<> 
        {
            template <class Event,class Fsm>
            void on_entry(Event const&, Fsm&) const {
                std::cout << "State1::on_entry()" << std::endl;
            }
        };
        struct State2:msmf::state<> 
        {
            template <class Event,class Fsm>
            void on_entry(Event const&, Fsm&) const {
                std::cout << "State2::on_entry()" << std::endl;
            }
        };
        struct State3:msmf::state<> 
        {
            template <class Event,class Fsm>
            void on_entry(Event const&, Fsm&) const {
                std::cout << "State3::on_entry()" << std::endl;
            }
        };
        struct State4:msmf::state<> 
        {
            template <class Event,class Fsm>
            void on_entry(Event const&, Fsm&) const {
                std::cout << "State4::on_entry()" << std::endl;
            }
        };
        struct State5:msmf::state<> 
        {
            template <class Event,class Fsm>
            void on_entry(Event const&, Fsm&) const {
                std::cout << "State5::on_entry()" << std::endl;
            }
        };
        struct State6:msmf::state<> 
        {
            template <class Event,class Fsm>
            void on_entry(Event const&, Fsm&) const {
                std::cout << "State6::on_entry()" << std::endl;
            }
        };

        // Set initial state
        typedef State1 initial_state;
        // Enable deferred capability
        typedef int activate_deferred_events;
        // Transition table
        struct transition_table:mpl::vector<
            //          Start   Event       Next        Action       Guard
            msmf::Row < State1, Event1,     msmf::none, msmf::Defer, msmf::none >,
            msmf::Row < State1, Event2,     State2,     msmf::none,  msmf::none >,
            msmf::Row < State2, msmf::none, State3,     msmf::none,  msmf::none >,
            msmf::Row < State3, Event1,     State4,     msmf::none,  msmf::none >,
            msmf::Row < State2, Event1,     State5,     msmf::none,  msmf::none >
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
// > Send Event2
// State2::on_entry()
// State3::on_entry()
// State4::on_entry()
