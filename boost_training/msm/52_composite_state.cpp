#include <iostream>
#include <boost/msm/back/state_machine.hpp>

#include <boost/msm/front/state_machine_def.hpp>
#include <boost/msm/front/functor_row.hpp>
#include <boost/static_assert.hpp>

namespace {
    namespace msm = boost::msm;
    namespace msmf = boost::msm::front;
    namespace mpl = boost::mpl;

    // ----- Events
    struct Event1 {};
    struct Event2 {};
    struct Event3 {};

    // ----- State machine
    struct OuterSm_:msmf::state_machine_def<OuterSm_>
    {
        struct State1_:msmf::state_machine_def<State1_>
        {
            template <class Event,class Fsm>
            void on_entry(Event const&, Fsm&) const {
                BOOST_STATIC_ASSERT((boost::is_convertible<Fsm, OuterSm_>::value));
                std::cout << "State1::on_entry()" << std::endl;
            }
            template <class Event,class Fsm>
            void on_exit(Event const&, Fsm&) const {
                BOOST_STATIC_ASSERT((boost::is_convertible<Fsm, OuterSm_>::value));
                std::cout << "State1::on_exit()" << std::endl;
            }

            struct SubState1:msmf::state<> {
                template <class Event,class Fsm>
                void on_entry(Event const&, Fsm&) const {
                    BOOST_STATIC_ASSERT((boost::is_convertible<Fsm, State1_>::value));
                    std::cout << "SubState1::on_entry()" << std::endl;
                }
                template <class Event,class Fsm>
                void on_exit(Event const&, Fsm&) const {
                    BOOST_STATIC_ASSERT((boost::is_convertible<Fsm, State1_>::value));
                    std::cout << "SubState1::on_exit()" << std::endl;
                }
            };
            struct SubState2:msmf::state<> {
                template <class Event,class Fsm>
                void on_entry(Event const&, Fsm&) const {
                    BOOST_STATIC_ASSERT((boost::is_convertible<Fsm, State1_>::value));
                    std::cout << "SubState2::on_entry()" << std::endl;
                }
                template <class Event,class Fsm>
                void on_exit(Event const&, Fsm&) const {
                    BOOST_STATIC_ASSERT((boost::is_convertible<Fsm, State1_>::value));
                    std::cout << "SubState2::on_exit()" << std::endl;
                }
            };

            // Set initial state
            typedef mpl::vector<SubState1> initial_state;
            // Transition table
            struct transition_table:mpl::vector<
                //          Start      Event   Next       Action      Guard
                msmf::Row < SubState1, Event2, SubState2, msmf::none, msmf::none >,
                msmf::Row < SubState2, Event3, SubState1, msmf::none, msmf::none >
                > {};
        };
        struct State2:msmf::state<>
        {
            template <class Event,class Fsm>
            void on_entry(Event const&, Fsm&) const {
                BOOST_STATIC_ASSERT((boost::is_convertible<Fsm, OuterSm_>::value));
                std::cout << "State2::on_entry()" << std::endl;
            }
            template <class Event,class Fsm>
            void on_exit(Event const&, Fsm&) const {
                BOOST_STATIC_ASSERT((boost::is_convertible<Fsm, OuterSm_>::value));
                std::cout << "State2::on_exit()" << std::endl;
            }
        };

        typedef msm::back::state_machine<State1_> State1;
        
        // Set initial state
        typedef State1 initial_state;
        // Transition table
        struct transition_table:mpl::vector<
            //          Start   Event   Next    Action      Guard
            msmf::Row < State1, Event1, State2, msmf::none, msmf::none >
        > {};
    };

    // Pick a back-end
    typedef msm::back::state_machine<OuterSm_> Osm;

    void test()
    {        
        Osm osm;
        osm.start(); 

        std::cout << "> Send Event2()" << std::endl;
        osm.process_event(Event2());
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
// State1::on_entry()
// SubState1::on_entry()
// > Send Event2()
// SubState1::on_exit()
// SubState2::on_entry()
// > Send Event1()
// SubState2::on_exit()
// State1::on_exit()
// State2::on_entry()
