#include <iostream>
#include <boost/msm/back/state_machine.hpp>

#include <boost/msm/front/state_machine_def.hpp>
#include <boost/msm/front/functor_row.hpp>


namespace {
    namespace msm = boost::msm;
    namespace msmf = boost::msm::front;
    namespace mpl = boost::mpl;

    // ----- Events
    struct Before {
        Before(int param_):param(param_) {}
        int param; 
    };
    struct After {
        After(Before const& b):param(b.param) {}
        int param;
    };

    // ----- State machine
    struct OuterSm_:msmf::state_machine_def<OuterSm_>
    {
        struct State1_:msmf::state_machine_def<State1_>
        {
            struct SubState1:msmf::state<> {};
            struct Exit1:msmf::exit_pseudo_state<After> {};

            // Set initial state
            typedef mpl::vector<SubState1> initial_state;
            // Transition table
            struct transition_table:mpl::vector<
                //          Start      Event   Next   Action      Guard
                msmf::Row < SubState1, Before, Exit1, msmf::none, msmf::none >
                > {};
        };
        struct State2:msmf::state<>
        {
            template <class Event,class Fsm>
            void on_entry(Event const& e, Fsm&) const {
                std::cout << "State2::on_entry()" << std::endl;
                std::cout << "Event param = " << e.param << std::endl;
            }
        };

        typedef msm::back::state_machine<State1_> State1;
        
        // Set initial state
        typedef State1 initial_state;
        // Transition table
        struct transition_table:mpl::vector<
            //          Start             Event  Next    Action      Guard
            msmf::Row < State1::exit_pt
                        <State1_::Exit1>, After, State2, msmf::none, msmf::none >
        > {};
    };

    // Pick a back-end
    typedef msm::back::state_machine<OuterSm_> Osm;

    void test()
    {        
        Osm osm;
        osm.start(); 

        std::cout << "> Send Before(42)" << std::endl;
        osm.process_event(Before(42));
    }
}

int main()
{
    test();
    return 0;
}

// Output:
//
// > Send Before(42)
// State2::on_entry()
// Event param = 42
