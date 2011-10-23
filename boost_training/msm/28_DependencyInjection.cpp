#include <iostream>
#include <boost/msm/back/state_machine.hpp>

#include <boost/msm/front/state_machine_def.hpp>
#include <boost/msm/front/functor_row.hpp>

namespace {
    namespace msm = boost::msm;
    namespace msmf = boost::msm::front;
    namespace mpl = boost::mpl;
    struct StateImp_:msm::front::state_machine_def<StateImp_>
    {
        struct Entry:msm::front::entry_pseudo_state<> {};
        struct StateImp_1 : msm::front::state<> {
            // Entry action
            template <class Event,class Fsm>
            void on_entry(Event const&, Fsm&) {
                std::cout << "StateImp_1::on_entry()" << std::endl;
            }
            // Exit action
            template <class Event,class Fsm>
            void on_exit(Event const&, Fsm&) {
                std::cout << "StateImp_1::on_exit()" << std::endl;
            }
        };
        typedef Entry initial_state;
        // Transition table
        struct transition_table:mpl::vector<
            //          Start  Event       Next          Action      Guard
            msmf::Row < Entry, msmf::none, StateImp_1,   msmf::none, msmf::none >
        > {};
    };
    typedef msm::back::state_machine<StateImp_> StateImp;
}

namespace {
    namespace msm = boost::msm;
    namespace msmf = boost::msm::front;
    namespace mpl = boost::mpl;

    struct Event1 {};

    struct Sm1_:msm::front::state_machine_def<Sm1_>
    {
        struct State1:msm::front::state<> {};
        struct State2:StateImp {};

        // Set initial state
        typedef State1 initial_state;

        // Transition table
        struct transition_table:mpl::vector<
            //          Start   Event       Next                 Action      Guard
            msmf::Row < State1, Event1,     State2::entry_pt
                                            <StateImp_::Entry>,  msmf::none, msmf::none >
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
