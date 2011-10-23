#if !defined(ATM_TRADE_HPP)
#define ATM_TRADE_HPP

#include <iostream>
#include <boost/msm/back/state_machine.hpp>

#include <boost/msm/front/state_machine_def.hpp>
#include <boost/msm/front/functor_row.hpp>

#include "atm_withdraw.hpp"
#include "atm_card_detect.hpp"

namespace Atm {
    namespace msm = boost::msm;
    namespace msmf = boost::msm::front;
    namespace mpl = boost::mpl;

    // ----- Events
    struct ChooseWithdraw {};

    // ----- State machine
    template <class AuthMethod>
    struct Trade_:msm::front::state_machine_def<Trade_<AuthMethod> >
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
        struct Withdrawing:Withdraw<AuthMethod>
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

        typedef typename Withdrawing::template entry_pt<typename Withdraw_<AuthMethod>::Entry>       WithDrawEntry;
        typedef typename Withdrawing::template entry_pt<typename Withdraw_<AuthMethod>::EntryByCard> WithDrawEntryByCard;
        typedef typename Withdrawing::template exit_pt <typename Withdraw_<AuthMethod>::Exit>        WithDrawExit;
        // Transition table
        struct transition_table:mpl::vector<
            //          Start         Event           Next                 Action      Guard
            msmf::Row < Choosing,     ChooseWithdraw, WithDrawEntry,       msmf::none, msmf::none >,
            msmf::Row < Choosing,     CardDetect,     WithDrawEntryByCard, msmf::none, msmf::none >,
            msmf::Row < WithDrawExit, msmf::none,     Choosing,            msmf::none, msmf::none >
        > {};
    };

    // Pick a back-end
    template <class AuthMethod>
    struct Trade:msm::back::state_machine<Trade_<AuthMethod> > {};
}

#endif // ATM_TRADE_HPP
