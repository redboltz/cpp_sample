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
    struct OuterSm_:msmf::state_machine_def<OuterSm_>
    {
        struct State1_:msmf::state_machine_def<State1_>
        {
            template <class Event,class Fsm>
            void on_entry(Event const&, Fsm&) const {
                std::cout << "State1::on_entry()" << std::endl;
            }
            template <class Event,class Fsm>
            void on_exit(Event const&, Fsm&) const {
                std::cout << "State1::on_exit()" << std::endl;
            }

            struct SubState1:msmf::state<> {
                template <class Event,class Fsm>
                void on_entry(Event const&, Fsm&) const {
                    std::cout << "SubState1::on_entry()" << std::endl;
                }
                template <class Event,class Fsm>
                void on_exit(Event const&, Fsm&) const {
                    std::cout << "SubState1::on_exit()" << std::endl;
                }
            };
            struct Exit1:msmf::exit_pseudo_state<msmf::none> {};

            // Set initial state
            typedef mpl::vector<SubState1> initial_state;
            // Transition table
            struct transition_table:mpl::vector<
                //          Start      Event   Next   Action      Guard
                msmf::Row < SubState1, Event1, Exit1, msmf::none, msmf::none >
                > {};
        };
        struct State2:msmf::state<>
        {
            template <class Event,class Fsm>
            void on_entry(Event const&, Fsm&) const {
                std::cout << "State2::on_entry()" << std::endl;
            }
            template <class Event,class Fsm>
            void on_exit(Event const&, Fsm&) const {
                std::cout << "State2::on_exit()" << std::endl;
            }
        };

        typedef msm::back::state_machine<State1_> State1;
        
        // Set initial state
        typedef State1 initial_state;
        // Transition table
        struct transition_table:mpl::vector<
            //          Start             Event       Next    Action      Guard
            msmf::Row < State1::exit_pt
                        <State1_::Exit1>, msmf::none, State2, msmf::none, msmf::none >
        > {};
    };

    // Pick a back-end
    typedef msm::back::state_machine<OuterSm_> Osm;

    void test()
    {        
        Osm osm;
        osm.start(); 

        std::cout << "> Send Event1()" << std::endl;
        osm.process_event(Event1());
    }
}

int main()
{
    test();
    return 0;
}

// Output:
//
