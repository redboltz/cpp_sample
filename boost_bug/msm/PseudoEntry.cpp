// Copyright 2012 Christophe Henry, Takatoshi Kondo
// henry UNDERSCORE christophe AT hotmail DOT com, redboltz AT gmail DOT com
// This is an extended version of the state machine available in the boost::mpl library
// Distributed under the same license as the original.
// Copyright for the original version:
// Copyright 2005 David Abrahams and Aleksey Gurtovoy. Distributed
// under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <iostream>
// back-end
#include <boost/msm/back/state_machine.hpp>
//front-end
#include <boost/msm/front/state_machine_def.hpp>
#include <boost/msm/front/functor_row.hpp>

#define BOOST_TEST_MODULE MyTest
#include <boost/test/unit_test.hpp>

namespace msm = boost::msm;
namespace mpl = boost::mpl;
using namespace boost::msm::front;

namespace
{
    // events
    struct event1 {};
    struct event2 {};

    // front-end: define the FSM structure 
    struct L0Fsm_ : public state_machine_def<L0Fsm_>
    {
        // The list of FSM states
        struct L1State1 : public state<> 
        {
            template <class Event,class FSM>
            void on_entry(Event const&,FSM& ) {++entry_counter;}
            template <class Event,class FSM>
            void on_exit(Event const&,FSM& ) {++exit_counter;}
            int entry_counter;
            int exit_counter;
        };
        struct L1Fsm_ : public state_machine_def<L1Fsm_>
        {
            unsigned int entry_action_counter;

            template <class Event,class FSM>
            void on_entry(Event const&,FSM& ) {++entry_counter;}
            template <class Event,class FSM>
            void on_exit(Event const&,FSM& ) {++exit_counter;}
            int entry_counter;
            int exit_counter;

            // test with a pseudo entry
            struct L2PseudoEntry : public msm::front::entry_pseudo_state<0>
            {
                template <class Event,class FSM>
                void on_entry(Event const&,FSM& ) {++entry_counter;}
                template <class Event,class FSM>
                void on_exit(Event const&,FSM& ) {++exit_counter;}
                int entry_counter;
                int exit_counter;
            };
            struct L2Fsm_ : public state_machine_def<L2Fsm_>
            {
                unsigned int entry_action_counter;
                unsigned int deferred_action_counter;

                template <class Event,class FSM>
                void on_entry(Event const&,FSM& ) {++entry_counter;}
                template <class Event,class FSM>
                void on_exit(Event const&,FSM& ) {++exit_counter;}
                int entry_counter;
                int exit_counter;

                // test with a pseudo entry
                struct L3PseudoEntry : public entry_pseudo_state<0>
                {
                    template <class Event,class FSM>
                    void on_entry(Event const&,FSM& ) {++entry_counter;}
                    template <class Event,class FSM>
                    void on_exit(Event const&,FSM& ) {++exit_counter;}
                    int entry_counter;
                    int exit_counter;
                };
                struct L3State1 : public state<>
                {
                    template <class Event,class FSM>
                    void on_entry(Event const&,FSM& ) {++entry_counter;}
                    template <class Event,class FSM>
                    void on_exit(Event const&,FSM& ) {++exit_counter;}
                    int entry_counter;
                    int exit_counter;
                };
                struct L3State2 : public state<>
                {
                    template <class Event,class FSM>
                    void on_entry(Event const&,FSM& ) {++entry_counter;}
                    template <class Event,class FSM>
                    void on_exit(Event const&,FSM& ) {++exit_counter;}
                    int entry_counter;
                    int exit_counter;
                };

                struct EntryAction {
                    template <class Event, class Fsm, class SourceState, class TargetState>
                    void operator()(Event const&, Fsm& f, SourceState&, TargetState&) {
                        ++f.entry_action_counter;
                    }
                };
                struct DeferredAction {
                    template <class Event, class Fsm, class SourceState, class TargetState>
                    void operator()(Event const&, Fsm& f, SourceState&, TargetState&) {
                        ++f.deferred_action_counter;
                    }
                };

                // the initial state. Must be defined
                typedef L3State1 initial_state;

                // Transition table for SubSubFsm2
                struct transition_table : mpl::vector<
                    //     Start           Event    Next       Action          Guard
                    //   +---------------+--------+----------+----------------+-------+
                    Row  < L3PseudoEntry , none   , L3State1 , EntryAction    , none  >,
                    Row  < L3State1      , none   , L3State2 , none           , none  >,
                    Row  < L3State2      , event2 , none     , DeferredAction , none  >
                    //   +---------------+--------+----------+----------------+-------+
                > {};

                // Replaces the default no-transition response.
                template <class FSM,class Event>
                void no_transition(Event const& e, FSM&,int state)
                {
                    BOOST_FAIL("no_transition called!");
                }
            };
            typedef msm::back::state_machine<L2Fsm_> L2Fsm;
            
            struct EntryAction {
                template <class Event, class Fsm, class SourceState, class TargetState>
                void operator()(Event const&, Fsm& f, SourceState&, TargetState&) {
                    ++f.entry_action_counter;
                }
            };

            // the initial state. Must be defined
            typedef L2Fsm_ initial_state;

            // Transition table for SubFsm2
            struct transition_table : mpl::vector<
                //     Start           Event   Next                      Action        Guard
                //   +---------------+-------+-------------------------+-------------+-------+
                Row  < L2PseudoEntry , none  , L2Fsm::entry_pt 
                                               <L2Fsm_::L3PseudoEntry> , EntryAction , none  >
                //   +---------------+-------+-------------------------+-------------+-------+
            > {};

            // Replaces the default no-transition response.
            template <class FSM,class Event>
            void no_transition(Event const& e, FSM&,int state)
            {
                BOOST_FAIL("no_transition called!");
            }
        };
        typedef msm::back::state_machine<L1Fsm_> L1Fsm;

        // the initial state of the player SM. Must be defined
        typedef L1State1 initial_state;

        // Enable deferred capability
        typedef int activate_deferred_events;

        // transition actions
        // guard conditions

        // Transition table for Fsm
        struct transition_table : mpl::vector<
            //     Start      Event    Next                      Action   Guard
            //   +----------+--------+-------------------------+--------+-------+
            Row  < L1State1 , event1 , L1Fsm::entry_pt 
                                       <L1Fsm_::L2PseudoEntry> , none   , none  >,
            Row  < L1State1 , event2 , none                    , Defer  , none  >
            //   +----------+--------+-------------------------+--------+-------+
        > {};

        // Replaces the default no-transition response.
        template <class FSM,class Event>
        void no_transition(Event const& e, FSM&,int state)
        {
            BOOST_FAIL("no_transition called!");
        }

        // init counters
        template <class Event,class FSM>
        void on_entry(Event const&,FSM& fsm) 
        {
            fsm.template get_state<L0Fsm_::L1State1&>().entry_counter=0;
            fsm.template get_state<L0Fsm_::L1State1&>().exit_counter=0;
            fsm.template get_state<L0Fsm_::L1Fsm&>().entry_counter=0;
            fsm.template get_state<L0Fsm_::L1Fsm&>().exit_counter=0;
            fsm.template get_state<L0Fsm_::L1Fsm&>().entry_action_counter=0;
            fsm.template get_state<L0Fsm_::L1Fsm&>().template get_state<L0Fsm_::L1Fsm::L2Fsm&>().entry_counter=0;
            fsm.template get_state<L0Fsm_::L1Fsm&>().template get_state<L0Fsm_::L1Fsm::L2Fsm&>().exit_counter=0;
            fsm.template get_state<L0Fsm_::L1Fsm&>().template get_state<L0Fsm_::L1Fsm::L2Fsm&>().entry_action_counter=0;
            fsm.template get_state<L0Fsm_::L1Fsm&>().template get_state<L0Fsm_::L1Fsm::L2Fsm&>().deferred_action_counter=0;
            fsm.template get_state<L0Fsm_::L1Fsm&>().template get_state<L0Fsm_::L1Fsm::L2PseudoEntry&>().entry_counter=0;
            fsm.template get_state<L0Fsm_::L1Fsm&>().template get_state<L0Fsm_::L1Fsm::L2PseudoEntry&>().exit_counter=0;
            fsm.template get_state<L0Fsm_::L1Fsm&>().template get_state<L0Fsm_::L1Fsm::L2Fsm&>().template get_state<L0Fsm_::L1Fsm::L2Fsm::L3PseudoEntry&>().entry_counter=0;
            fsm.template get_state<L0Fsm_::L1Fsm&>().template get_state<L0Fsm_::L1Fsm::L2Fsm&>().template get_state<L0Fsm_::L1Fsm::L2Fsm::L3PseudoEntry&>().exit_counter=0;
            fsm.template get_state<L0Fsm_::L1Fsm&>().template get_state<L0Fsm_::L1Fsm::L2Fsm&>().template get_state<L0Fsm_::L1Fsm::L2Fsm::L3State1&>().entry_counter=0;
            fsm.template get_state<L0Fsm_::L1Fsm&>().template get_state<L0Fsm_::L1Fsm::L2Fsm&>().template get_state<L0Fsm_::L1Fsm::L2Fsm::L3State1&>().exit_counter=0;
            fsm.template get_state<L0Fsm_::L1Fsm&>().template get_state<L0Fsm_::L1Fsm::L2Fsm&>().template get_state<L0Fsm_::L1Fsm::L2Fsm::L3State2&>().entry_counter=0;
            fsm.template get_state<L0Fsm_::L1Fsm&>().template get_state<L0Fsm_::L1Fsm::L2Fsm&>().template get_state<L0Fsm_::L1Fsm::L2Fsm::L3State2&>().exit_counter=0;
            
        }
    };
    typedef msm::back::state_machine<L0Fsm_> L0Fsm;


    BOOST_AUTO_TEST_CASE( my_test )
    {     
        L0Fsm p;

        p.start(); 
        BOOST_CHECK_MESSAGE(p.get_state<L0Fsm_::L1State1&>().entry_counter == 1,"L1State1 entry not called correctly");
        BOOST_CHECK_MESSAGE(p.current_state()[0] == 0,"L1State1 should be active");

        p.process_event(event2()); // Deferred
        BOOST_CHECK_MESSAGE(p.current_state()[0] == 0,"L1State1 should be active");
        BOOST_CHECK_MESSAGE(p.get_state<L0Fsm_::L1State1&>().entry_counter == 1,"L1State1 entry not called correctly");
        BOOST_CHECK_MESSAGE(p.get_state<L0Fsm_::L1State1&>().exit_counter == 0,"L1State1 exit not called correctly");

        p.process_event(event1()); 
        BOOST_CHECK_MESSAGE(p.current_state()[0] == 1,"L1Fsm should be active");
        BOOST_CHECK_MESSAGE(p.get_state<L0Fsm_::L1State1&>().exit_counter == 1,"L1State1 exit not called correctly");
        BOOST_CHECK_MESSAGE(p.get_state<L0Fsm_::L1Fsm&>().entry_counter == 1,"L1Fsm entry not called correctly");
        BOOST_CHECK_MESSAGE(p.get_state<L0Fsm_::L1Fsm&>().entry_action_counter == 1,"L1Fsm entry action not called correctly");
        BOOST_CHECK_MESSAGE(p.get_state<L0Fsm_::L1Fsm&>().get_state<L0Fsm_::L1Fsm::L2PseudoEntry&>().entry_counter == 1,"L2EntryPoint entry not called correctly");
        BOOST_CHECK_MESSAGE(p.get_state<L0Fsm_::L1Fsm&>().get_state<L0Fsm_::L1Fsm::L2PseudoEntry&>().exit_counter == 1,"L2EntryPoint exit not called correctly");
        BOOST_CHECK_MESSAGE(p.get_state<L0Fsm_::L1Fsm&>().get_state<L0Fsm_::L1Fsm::L2Fsm&>().entry_counter == 1,"L2Fsm entry not called correctly");
        BOOST_CHECK_MESSAGE(p.get_state<L0Fsm_::L1Fsm&>().get_state<L0Fsm_::L1Fsm::L2Fsm&>().entry_action_counter == 1,"L2Fsm entry action not called correctly");
        BOOST_CHECK_MESSAGE(p.get_state<L0Fsm_::L1Fsm&>().get_state<L0Fsm_::L1Fsm::L2Fsm&>().exit_counter == 0,"L2Fsm exit not called correctly");
        BOOST_CHECK_MESSAGE(p.get_state<L0Fsm_::L1Fsm&>().get_state<L0Fsm_::L1Fsm::L2Fsm&>().get_state<L0Fsm_::L1Fsm::L2Fsm::L3PseudoEntry&>().entry_counter == 1,"L3PseudoEntry entry not called correctly");
        BOOST_CHECK_MESSAGE(p.get_state<L0Fsm_::L1Fsm&>().get_state<L0Fsm_::L1Fsm::L2Fsm&>().get_state<L0Fsm_::L1Fsm::L2Fsm::L3PseudoEntry&>().exit_counter == 1,"L3PseudoEntry exit not called correctly");
        BOOST_CHECK_MESSAGE(p.get_state<L0Fsm_::L1Fsm&>().get_state<L0Fsm_::L1Fsm::L2Fsm&>().get_state<L0Fsm_::L1Fsm::L2Fsm::L3State1&>().entry_counter == 1,"L3State1 entry not called correctly");
        BOOST_CHECK_MESSAGE(p.get_state<L0Fsm_::L1Fsm&>().get_state<L0Fsm_::L1Fsm::L2Fsm&>().get_state<L0Fsm_::L1Fsm::L2Fsm::L3State1&>().exit_counter == 1,"L3State1 exit not called correctly");
        BOOST_CHECK_MESSAGE(p.get_state<L0Fsm_::L1Fsm&>().get_state<L0Fsm_::L1Fsm::L2Fsm&>().get_state<L0Fsm_::L1Fsm::L2Fsm::L3State2&>().entry_counter == 1,"L3State2 entry not called correctly");
        BOOST_CHECK_MESSAGE(p.get_state<L0Fsm_::L1Fsm&>().get_state<L0Fsm_::L1Fsm::L2Fsm&>().get_state<L0Fsm_::L1Fsm::L2Fsm::L3State2&>().exit_counter == 0,"L3State2 exit not called correctly");
        BOOST_CHECK_MESSAGE(p.get_state<L0Fsm_::L1Fsm&>().get_state<L0Fsm_::L1Fsm::L2Fsm&>().deferred_action_counter == 1,"L2Fsm deferred action not called correctly");
    }
}

