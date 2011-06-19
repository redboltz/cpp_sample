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

    // ----- State machine (Outer)
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
		// ----- State machine (Inner) as State2 of Outer state machine.
		struct State2_:public msm::front::state_machine_def<State2_>
		{
            // Entry action
            template <class Event,class Fsm>
            void on_entry(Event const&, Fsm&) {
				std::cout << "State2::on_entry()" << std::endl;
			}
            // Exit action
            template <class Event,class Fsm>
            void on_exit(Event const&, Fsm&) {
				std::cout << "State2::on_exit()" << std::endl;
			}

			// (Sub) States
			struct Init:msm::front::state<> {};
			struct State2_1:msm::front::state<>,  public msm::front::explicit_entry<0> 
			{
				// Entry action
				template <class Event,class Fsm>
				void on_entry(Event const&, Fsm&) {
					std::cout << "State2_1::on_entry()" << std::endl;
				}
				// Exit action
				template <class Event,class Fsm>
				void on_exit(Event const&, Fsm&) {
					std::cout << "State2_1::on_exit()" << std::endl;
				}
			};

			// (Sub) Set initial state
			typedef Init initial_state;

			// Actions
			struct InitAction {
				template <class Event, class Fsm, class SourceState, class TargetState>
				void operator()(Event const&, Fsm&, SourceState&, TargetState&)
				{
					std::cout << "InitAction()" << std::endl;
				}
			};

			// (Sub) Transition table
			struct transition_table:mpl::vector<
				//          Start	Event		Next		Action		Guard
				msmf::Row < Init,	msmf::none,	State2_1,	InitAction,	msmf::none >
			> {};
		};
		// (Sub) Pick a back-end
		typedef msm::back::state_machine<State2_> State2;

		// Set initial state
		typedef State1 initial_state;
		// Transition table
		struct transition_table:mpl::vector<
			//          Start	Event	Next								Action		Guard
			msmf::Row < State1,	Event1,	State2,								msmf::none,	msmf::none >,
			msmf::Row < State1,	Event2,	State2::direct<State2_::State2_1>,	msmf::none,	msmf::none >
		> {};
	};

	// Pick a back-end
    typedef msm::back::state_machine<Sm1_> Sm1;

    void test1()
    {        
		std::cout << "=== test1 start" << std::endl;
		Sm1 sm1;
		sm1.start(); 
		std::cout << "> Send Event1" << std::endl;
		sm1.process_event(Event1());
		std::cout << "=== test1 end" << std::endl;
	}
    void test2()
	{
		std::cout << "=== test2 start" << std::endl;
		Sm1 sm1;
		sm1.start(); 
		std::cout << "> Send Event2" << std::endl;
		sm1.process_event(Event2());
		std::cout << "=== test2 end" << std::endl;
	}
}

int main()
{
    test1();
    test2();
    return 0;
}

// Output:
//
// === test1 start
// State1::on_entry()
// > Send Event1
// State1::on_exit()
// State2::on_entry()
// InitAction()
// State2_1::on_entry()
// === test1 end
// === test2 start
// State1::on_entry()
// > Send Event2
// State1::on_exit()
// State2::on_entry()
// State2_1::on_entry()
// === test2 end
