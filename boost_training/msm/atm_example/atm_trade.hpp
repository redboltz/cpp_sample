#if !defined(ATM_TRADE_HPP)
#define ATM_TRADE_HPP

#include <iostream>
#include <boost/msm/back/state_machine.hpp>

#include <boost/msm/front/state_machine_def.hpp>
#include <boost/msm/front/functor_row.hpp>

#include "atm_withdraw.hpp"

namespace Atm {
    namespace msm = boost::msm;
    namespace msmf = boost::msm::front;
    namespace mpl = boost::mpl;

    // ----- Events
    struct ChooseWithdraw {};

    // ----- State machine
    struct Trade_:msm::front::state_machine_def<Trade_>
    {
        // States
        struct Choosing:msm::front::state<> 
        {
            template <class Event,class Fsm>
            void on_entry(Event const&, Fsm&) const {
                std::cout << "[DBG] Enter Choosing" << std::endl;
                std::cout << "1.Withdraw, 2.N/A, 3.N/A ..." << std::endl;
            }
            template <class Event,class Fsm>
            void on_exit(Event const&, Fsm&) const {
                std::cout << "[DBG] Exit  Choosing" << std::endl;
			}
        };
        struct Withdrawing:Withdraw
		{
            template <class Event,class Fsm>
            void on_entry(Event const&, Fsm&) const {
                std::cout << "[DBG] Enter Withdrawing" << std::endl;
            }
            template <class Event,class Fsm>
            void on_exit(Event const&, Fsm&) const {
                std::cout << "[DBG] Exit  Withdrawing" << std::endl;
			}
		};

        // Set initial state
        typedef Choosing initial_state;

        // Transition table
        struct transition_table:mpl::vector<
            //          Start              Event           Next                      Action      Guard
            msmf::Row < Choosing,          ChooseWithdraw, Withdraw::entry_pt
                                                           <Withdraw_::Entry>,       msmf::none, msmf::none >,
            msmf::Row < Choosing,          CardDetect,     Withdraw::entry_pt
                                                           <Withdraw_::EntryByCard>, msmf::none, msmf::none >,
            msmf::Row < Withdraw::exit_pt
                        <Withdraw_::Exit>, msmf::none,     Choosing,                 msmf::none, msmf::none >
        > {};
    };

    // Pick a back-end
    typedef msm::back::state_machine<Trade_> Trade;
}

#endif // ATM_TRADE_HPP
