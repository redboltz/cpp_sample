#if !defined(ATM_TRADE_HPP)
#define ATM_TRADE_HPP

#include <iostream>
#include <boost/msm/back/state_machine.hpp>

#include <boost/msm/front/state_machine_def.hpp>
#include <boost/msm/front/functor_row.hpp>

#include "atm_auth.hpp"

namespace Atm {
    namespace msm = boost::msm;
    namespace msmf = boost::msm::front;
    namespace mpl = boost::mpl;

    // ----- Events
    struct Withdraw {};
    struct Cancel {};
    struct Ok {};
    struct EnterAmount {
        EnterAmount(int amount_):amount(amount_) {}
        int amount;
    };

    // ----- State machine
    struct Trade_:msm::front::state_machine_def<Trade_>
    {
        // States
        struct Choosing:msm::front::state<> 
        {
            // Entry action
            template <class Event,class Fsm>
            void on_entry(Event const&, Fsm&) const {
                std::cout << "1.Withdraw, 2.N/A, 3.N/A ..." << std::endl;
            }
        };
        struct WithdrawAuth:Auth {};
        struct EnteringAmount:msm::front::state<> 
        {
            // Entry action
            template <class Event,class Fsm>
            void on_entry(Event const&, Fsm&) const {
                std::cout << "Input amount of money" << std::endl;
            }
            EnteringAmount():amount(int()) {}
            int amount;
            int limit;
        };
        struct InsufficientFunds:msm::front::state<> 
        {
            // Entry action
            template <class Event,class Fsm>
            void on_entry(Event const&, Fsm&) const {
                std::cout << "Insufficient Funds" << std::endl;
            }
        };
        // Guards
        struct CheckAmount {
            template <class Event, class Fsm, class SourceState, class TargetState>
            bool operator()(Event const&, Fsm&, SourceState& s, TargetState&) const 
            {
                return s.amount <= s.limit;
            }
        };
        // Actions
        struct SetAmount {
            template <class Event, class Fsm, class SourceState, class TargetState>
            void operator()(Event const& e, Fsm&, SourceState&, TargetState& t) const 
            {
                t.limit = e.amount;
            }
        };

        // Set initial state
        typedef Choosing initial_state;

        // Transition table
        struct transition_table:mpl::vector<
            //          Start                 Event         Next                   Action       Guard
            msmf::Row < Choosing,             Withdraw,     WithdrawAuth::entry_pt
                                                            <Auth_::Entry>,        msmf::none,  msmf::none >,
            msmf::Row < WithdrawAuth::exit_pt
                        <Auth_::ExitFail>,    msmf::none,   Choosing,              msmf::none,  msmf::none >,
            msmf::Row < WithdrawAuth::exit_pt
                        <Auth_::ExitSuccess>, AccountInfo,  EnteringAmount,        SetAmount,   msmf::none >,
            msmf::Row < Choosing,             Withdraw,     WithdrawAuth::entry_pt
                                                            <Auth_::Entry>,        msmf::none,  msmf::none >,
                                                                                   // else                                                             
            msmf::Row < EnteringAmount,       Ok,           EnteringAmount,        msmf::none,  msmf::none >, 
            msmf::Row < EnteringAmount,       Ok,           EnteringAmount,        CheckAmount, msmf::none >  
        > {};
    };

    // Pick a back-end
    typedef msm::back::state_machine<Trade_> Trade;
}

#endif // ATM_TRADE_HPP
