#include <iostream>
#include <boost/msm/back/state_machine.hpp>

#include <boost/msm/front/state_machine_def.hpp>
#include <boost/msm/front/functor_row.hpp>

namespace {
    namespace msm = boost::msm;
    namespace msmf = boost::msm::front;
    namespace mpl = boost::mpl;

    // Events
    struct Event1 {};

    // ----- State machine
    struct Sm1_:msmf::state_machine_def<Sm1_>
    {
        struct State1_:msmf::state<>{
            template <class Event, class Fsm, class SourceState, class TargetState>
            void operator()(Event const&, Fsm& f, SourceState&, TargetState&) const 
            {
                f.val = 0;
                std::cout << "val = " << f.val << std::endl;
            }
        };
        struct Choice_:msmf::state<>{};

        // Set initial state
        typedef State1_ initial_state;

         // Guards
        struct GuardVal1 {
            template <class Event, class Fsm, class SourceState, class TargetState>
            bool operator()(Event const&, Fsm& f, SourceState&, TargetState&) const 
            {
                if (f.val == 1) return true;
                return false;
            }
        };
         // Actions
        struct ActionVal1Assign {
            template <class Event, class Fsm, class SourceState, class TargetState>
            void operator()(Event const&, Fsm& f, SourceState&, TargetState&) const 
            {
                f.val = 1;
                std::cout << "ActionVal1Assign val = " << f.val << std::endl;
            }
        };
        struct ActionVal1Branch {
            template <class Event, class Fsm, class SourceState, class TargetState>
            void operator()(Event const&, Fsm& f, SourceState&, TargetState&) const 
            {
                std::cout << "ActionVal1Branch val = " << f.val << std::endl;
            }
        };
        struct ActionElseBranch {
            template <class Event, class Fsm, class SourceState, class TargetState>
            void operator()(Event const&, Fsm& f, SourceState&, TargetState&) const 
            {
                std::cout << "ActionElseBranch val = " << f.val << std::endl;
            }
        };

        // Transition table
        struct transition_table:mpl::vector<
            //          Start    Event       Next     Action            Guard
            msmf::Row < State1_, Event1,     Choice_, ActionVal1Assign, msmf::none >,
            msmf::Row < Choice_, msmf::none, State1_, ActionElseBranch, msmf::none >, // else
            msmf::Row < Choice_, msmf::none, State1_, ActionVal1Branch, GuardVal1 >
        > {};
        private:
            int val;
    };

    // back-end
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

// Output:
//
// > Send Event1
// ActionVal1Assign val = 1
// ActionVal1Branch val = 1
