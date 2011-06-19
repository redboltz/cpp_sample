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
    struct Sm1_:public msm::front::state_machine_def<Sm1_>
    {
        // States
        struct State1:msm::front::state<> 
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
        };
        struct End:msm::front::terminate_state<> {};

        // Set initial state
        typedef State1 initial_state;

        // Transition table
        struct transition_table:mpl::vector<
            //          Start   Event   Next    Action      Guard
            msmf::Row < State1, Event1, End,    msmf::none, msmf::none >
        > {};
        // No handled event handler
        template <class Fsm,class Event> 
        void no_transition(Event const& e, Fsm& ,int state) {
            std::cout << "No handled event " << typeid(e).name() << " on State " << state << std::endl;
        }
    };

    // Pick a back-end
    typedef msm::back::state_machine<Sm1_> Sm1;

    void test()
    {        
        Sm1 sm1;
        sm1.start(); 
        std::cout << "> Send Event2" << std::endl;
        sm1.process_event(Event2());
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
