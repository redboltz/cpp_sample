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
    struct Sm1_:msmf::state_machine_def<Sm1_>
    {
        // States
        struct State1_:msmf::state<>
        {
            template <class Event,class Fsm>
            void on_entry(Event const&, Fsm&) const {
                std::cout << "State1::on_entry()" << std::endl;
            }
        };
        // ----- SubState
        struct State2_:msmf::state_machine_def<State2_>
        {
            template <class Event,class Fsm>
            void on_entry(Event const&, Fsm&) const {
                std::cout << "State2::on_entry()" << std::endl;
            }
            struct SubState1:msmf::state<>
            {
                template <class Event,class Fsm>
                void on_entry(Event const&, Fsm&) const {
                    std::cout << "SubState1::on_entry()" << std::endl;
                }
            };
            struct SubState2:msmf::state<>
            {
                template <class Event,class Fsm>
                void on_entry(Event const&, Fsm&) const {
                    std::cout << "SubState2::on_entry()" << std::endl;
                }
            };
            struct Entry1:msmf::entry_pseudo_state<> {};
            struct Exit1:msmf::exit_pseudo_state<msmf::none> {};
            // Set initial state
            typedef mpl::vector<SubState1> initial_state;
            // Transition table
            struct transition_table:mpl::vector<
                //          Start      Event       Next       Action      Guard
                msmf::Row < Entry1,    msmf::none, SubState2, msmf::none, msmf::none >,
                msmf::Row < SubState1, Event1,     Exit1,     msmf::none, msmf::none >,
                msmf::Row < SubState2, Event1,     Exit1,     msmf::none, msmf::none >
            > {};
        };
        typedef msm::back::state_machine<State2_> State2;

        // Set initial state
        typedef State1_ initial_state;
        // Transition table
        struct transition_table:mpl::vector<
            //          Start    Event       Next               Action      Guard
            msmf::Row < State1_, msmf::none, State2::entry_pt
                                             <State2_::Entry1>, msmf::none, msmf::none >
        > {};
    };

    // Pick a back-end
    typedef msm::back::state_machine<Sm1_> Sm1;

    void test()
    {        
        Sm1 sm1;
        sm1.start(); 
    }
}

int main()
{
    test();
    return 0;
}
