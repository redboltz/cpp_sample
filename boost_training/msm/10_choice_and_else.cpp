#include <iostream>
#include <boost/msm/back/state_machine.hpp>

#include <boost/msm/front/state_machine_def.hpp>
#include <boost/msm/front/functor_row.hpp>

#include <boost/msm/front/euml/euml.hpp>

namespace {
    namespace msm = boost::msm;
    namespace msmf = boost::msm::front;
    namespace msme = boost::msm::front::euml;
    namespace mpl = boost::mpl;
    // ----- Events
    struct Event1 { 
        int val;
        Event1(int val_):val(val_) {}
    };

    // ----- State machine
    struct Sm1_:public msm::front::state_machine_def<Sm1_>
    {
        // States
        struct State1:msm::front::state<> 
        {
            // Entry action
            template <class Event,class Fsm>
            void on_entry(Event const&, Fsm&) const {
                std::cout << "State1::on_entry()" << std::endl;
            }
            // Exit action
            template <class Event,class Fsm>
            void on_exit(Event const&, Fsm&) const {
                std::cout << "State1::on_exit()" << std::endl;
            }
        };

        struct State2:msm::front::state<> 
        {
            // Entry action
            template <class Event,class Fsm>
            void on_entry(Event const&, Fsm&) const {
                std::cout << "State2::on_entry()" << std::endl;
            }
            // Exit action
            template <class Event,class Fsm>
            void on_exit(Event const&, Fsm&) const {
                std::cout << "State2::on_exit()" << std::endl;
            }
        };

        struct Choice1:msm::front::state<> {};

        // Actions
        struct Action1 {
            template <class Event, class Fsm, class SourceState, class TargetState>
            void operator()(Event const& e, Fsm& fsm, SourceState&, TargetState&) const 
            {
                fsm.res = e.val + 5;
            }
        };

        // Guards
        struct Guard1 {
            template <class Event, class Fsm, class SourceState, class TargetState>
            bool operator()(Event const&, Fsm& fsm, SourceState&, TargetState&) const 
            {
                return fsm.res > 10;
            }
        };

        // Set initial state
        typedef State1 initial_state;

        // Transition table
        struct transition_table:mpl::vector<
            //          Start    Event       Next     Action      Guard
            msmf::Row < State1,  Event1,     Choice1, Action1,    msmf::none         >,
            msmf::Row < Choice1, msmf::none, State2,  msmf::none, Guard1             >,
            msmf::Row < Choice1, msmf::none, State1,  msmf::none, msme::Not_<Guard1> >
        > {};

        // data member
        int res;
    };

    // Pick a back-end
    typedef msm::back::state_machine<Sm1_> Sm1;

    void test()
    {        
        Sm1 sm1;
        sm1.start(); 
        std::cout << "> Send Event1(5)" << std::endl;
        sm1.process_event(Event1(5));
        std::cout << "> Send Event1(6)" << std::endl;
        sm1.process_event(Event1(6));
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
// > Send Event1(5)
// State1::on_exit()
// State1::on_entry()
// > Send Event1(6)
// State1::on_exit()
// State2::on_entry()
