#include <iostream>
#include <boost/msm/back/state_machine.hpp>

#include <boost/msm/front/state_machine_def.hpp>
#include <boost/msm/front/functor_row.hpp>

#include <boost/mpl/print.hpp>

namespace {
    namespace msm = boost::msm;
    namespace msmf = boost::msm::front;
    namespace mpl = boost::mpl;

    // ----- Events
    struct Event1 {};
    struct Event2 {};
    struct Event3 {};

    // ----- State machine
    struct Sm_:msmf::state_machine_def<Sm_>
    {
        struct State1:msmf::state<> 
        {
#if 0
            template <typename Event, typename Fsm>
            void on_entry(Event const&, Fsm const&) {
                typedef typename boost::mpl::print<Event>::type t;
            }
#endif
        };
        struct State2:msmf::state<> 
        {
#if 0
            template <typename Event, typename Fsm>
            void on_entry(Event const&, Fsm const&) {
                typedef typename boost::mpl::print<Event>::type t;
            }
#endif
        };
        struct State3:msmf::state<>
        {
        };
        struct State4:msmf::state<>
        {
#if 1
            template <typename Event, typename Fsm>
            void on_entry(Event const&, Fsm const&) {
                typedef typename boost::mpl::print<Event>::type t;
            }
#endif
        };
        struct State5:msmf::state<>
        {
        };
        struct State6:msmf::state<>
        {
        };

       
        // Set initial state
        typedef State1 initial_state;
        // Transition table
        struct transition_table:mpl::vector<
            //          Start   Event       Next    Action      Guard
            msmf::Row < State1, Event1,     State4, msmf::none, msmf::none >,
            msmf::Row < State4, msmf::none, State2, msmf::none, msmf::none >,
            msmf::Row < State2, Event2,     State4, msmf::none, msmf::none >,
            msmf::Row < State5, msmf::none, State3, msmf::none, msmf::none >,
            msmf::Row < State3, Event3,     State6, msmf::none, msmf::none >
        > {};
    };

    // Pick a back-end
    typedef msm::back::state_machine<Sm_> Sm;

    void test()
    {        
        Sm sm;
        sm.start(); 

        std::cout << "> Send Event1()" << std::endl;
        sm.process_event(Event1());
        std::cout << "> Send Event2()" << std::endl;
        sm.process_event(Event2());
        std::cout << "> Send Event3()" << std::endl;
        sm.process_event(Event3());
    }
}

int main()
{
    test();
    return 0;
}

// Output:
//
