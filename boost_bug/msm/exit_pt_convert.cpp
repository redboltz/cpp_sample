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
    struct Event2 {
        Event2(int val_):val(val_) {}
        int val;
    };
    struct Event3 {
        Event3() {}
        Event3(int val_):val(val_) {}
        template <class T> Event3(T const& e):val(e.val) {}
        int val;
    };
    // ----- State machine
    struct Sm_:msm::front::state_machine_def<Sm_>
    {
        // States
        struct SubSm_:msm::front::state_machine_def<SubSm_>
        {
            // States
            struct Entry:msm::front::state<> {};
            struct Exit :msm::front::exit_pseudo_state<Event3> {};

            struct SubState1:msm::front::state<> {};
            // Set initial state
            typedef Entry initial_state;

            // Transition table
            struct transition_table:mpl::vector<
                //          Start      Event       Next       Action      Guard
                msmf::Row < Entry,     msmf::none, SubState1, msmf::none, msmf::none >,
                msmf::Row < SubState1, Event2,     Exit,      msmf::none, msmf::none >
            > {};
        };
        struct State1:msm::front::state<> {};
        typedef msm::back::state_machine<SubSm_> State2;
        struct State3:msm::front::state<> {};

        // Set initial state
        typedef State1 initial_state;

        // Transition table
        struct transition_table:mpl::vector<
            //          Start           Event   Next    Action      Guard
            msmf::Row < State1,         Event1, State2, msmf::none, msmf::none >,
            msmf::Row < State2::exit_pt
                        <SubSm_::Exit>, Event3, State3, msmf::none, msmf::none >
        > {};
    };
    typedef msm::back::state_machine<Sm_> Sm;
}

int main()
{
    Sm s;
    s.start();
    s.process_event(Event1());
    s.process_event(Event2(1));
}
