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

    // ----- StateSub
    struct StateSub_:msmf::state_machine_def<StateSub_>
    {
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
        struct SubState3:msmf::state<>
        {
            template <class Event,class Fsm>
            void on_entry(Event const&, Fsm&) const {
                std::cout << "SubState3::on_entry()" << std::endl;
            }
        };
        struct dummy:msmf::state<> {};
        struct Entry1:msmf::entry_pseudo_state<2> {};
        struct Exit1:msmf::exit_pseudo_state<msmf::none> {};
        // Set initial state
        typedef mpl::vector<SubState1, SubState2, dummy> initial_state;
        // Transition table
        struct transition_table:mpl::vector<
            //          Start      Event       Next       Action      Guard
            msmf::Row < Entry1,    msmf::none, SubState3, msmf::none, msmf::none >,
            msmf::Row < SubState1, Event1,     Exit1,     msmf::none, msmf::none >
        > {};
    };
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
        struct State2_:StateSub_ 
        {
            template <class Event,class Fsm>
            void on_entry(Event const&, Fsm&) const {
                std::cout << "State2::on_entry()" << std::endl;
            }
        };
        typedef msm::back::state_machine<State2_> State2;

        // Set initial state
        typedef State1_ initial_state;
        // Transition table
        struct transition_table:mpl::vector<
            //          Start             Event       Next                Action      Guard
            msmf::Row < State2::exit_pt 
                        <State2_::Exit1>, msmf::none, State1_,            msmf::none, msmf::none >,
            msmf::Row < State1_,          Event1,     State2::entry_pt
                                                     <State2_::Entry1>,   msmf::none, msmf::none >
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
// State1::on_entry()
// > Send Event1
// > Send Event2
// State2::on_entry()
// State3::on_entry()
// State4::on_entry()
