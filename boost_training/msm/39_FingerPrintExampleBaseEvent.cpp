#include <iostream>
#include <boost/msm/back/state_machine.hpp>

#include <boost/msm/front/state_machine_def.hpp>
#include <boost/msm/front/functor_row.hpp>


namespace {
    namespace msm = boost::msm;
    namespace msmf = boost::msm::front;
    namespace mpl = boost::mpl;

    // ----- Events
    struct AnyEvent {};
    struct StartButtonPressed:AnyEvent {};
    struct CheckSucceeded {};
    struct CheckFailed {};
    struct ScanSucceeded {};
    struct ScanFailed {};
    struct Completed:AnyEvent {
        enum Result {
            OK,
            NG
        };
        Completed(CheckFailed const&):r_(NG) {}
        Completed(ScanSucceeded const&):r_(OK) {}
        Completed(ScanFailed const&):r_(NG) {}
        Result result() const { return r_; }
    private:
        Result r_;
    };

    // ----- State machine
    struct Sm1_:msmf::state_machine_def<Sm1_>
    {
        struct Waiting_:msmf::state<>
        {
            template <class Event,class Fsm>
            void on_entry(Event const&, Fsm&) const {
                std::cout << "Press Start Button" << std::endl;
            }
        };
        struct ScanFinger_:msmf::state_machine_def<ScanFinger_>
        {
            struct AliveChecking:msmf::state<> {
                template <class Event,class Fsm>
                void on_entry(Event const&, Fsm&) const {
                    std::cout << "Alive checking..." << std::endl;
                }
            };
            struct Scanning:msmf::state<> {
                template <class Event,class Fsm>
                void on_entry(Event const&, Fsm&) const {
                    std::cout << "Scanning..." << std::endl;
                }
            };
            struct Entry1:msmf::entry_pseudo_state<> {};
            struct Exit1:msmf::exit_pseudo_state<Completed> {};

            // Set initial state
            typedef mpl::vector<AliveChecking> initial_state;
            // Transition table
            struct transition_table:mpl::vector<
                //          Start          Event           Next      Action      Guard
                msmf::Row < AliveChecking, CheckSucceeded, Scanning, msmf::none, msmf::none >,
                msmf::Row < AliveChecking, CheckFailed,    Exit1,    msmf::none, msmf::none >,
                msmf::Row < Entry1,        AnyEvent,       Scanning, msmf::none, msmf::none >,
                msmf::Row < Scanning,      ScanSucceeded,  Exit1,    msmf::none, msmf::none >,
                msmf::Row < Scanning,      ScanFailed,     Exit1,    msmf::none, msmf::none >
                > {};
        };

        struct RightScanning_:ScanFinger_ {};
        struct LeftScanning_:ScanFinger_ {};

        typedef msm::back::state_machine<RightScanning_> RightScanning;
        typedef msm::back::state_machine<LeftScanning_> LeftScanning;

        // Actions
        struct RightMessage {
            template <class Event, class Fsm, class SourceState, class TargetState>
            void operator()(Event const&, Fsm&, SourceState&, TargetState&)
            {
                std::cout << "Right hand please." << std::endl;
            }
        };
        struct LeftMessage {
            template <class Event, class Fsm, class SourceState, class TargetState>
            void operator()(Event const&, Fsm&, SourceState&, TargetState&)
            {
                std::cout << "Left hand please." << std::endl;
            }
        };
        struct FailMessage {
            template <class Event, class Fsm, class SourceState, class TargetState>
            void operator()(Event const&, Fsm&, SourceState&, TargetState&)
            {
                std::cout << "Register Failed." << std::endl;
            }
        };
        struct CompleteMessage {
            template <class Event, class Fsm, class SourceState, class TargetState>
            void operator()(Event const&, Fsm&, SourceState&, TargetState&)
            {
                std::cout << "Register Completed." << std::endl;
            }
        };
        
       // Guards
        struct IsResultOk {
            template <class Event, class Fsm, class SourceState, class TargetState>
            bool operator()(Event const& e, Fsm&, SourceState&, TargetState&)
            {
                return e.result() == Completed::OK;
            }
        };
        // Set initial state
        typedef Waiting_ initial_state;
        // Transition table
        struct transition_table:mpl::vector<
            //          Start                    Event               Next                      Action           Guard
            msmf::Row < Waiting_,                StartButtonPressed, RightScanning::entry_pt
                                                                     <RightScanning_::Entry1>, RightMessage,    msmf::none >,
            msmf::Row < RightScanning::exit_pt
                        <RightScanning_::Exit1>, Completed,          Waiting_,                 FailMessage,     msmf::none >, // else
            msmf::Row < RightScanning::exit_pt
                        <RightScanning_::Exit1>, Completed,          LeftScanning::entry_pt
                                                                     <LeftScanning_::Entry1>,  LeftMessage,     IsResultOk >,
            msmf::Row < LeftScanning::exit_pt
                        <LeftScanning_::Exit1>,  Completed,          Waiting_,                 FailMessage,     msmf::none >, // else
            msmf::Row < LeftScanning::exit_pt
                        <LeftScanning_::Exit1>,  Completed,          Waiting_,                 CompleteMessage, IsResultOk >
        > {};
    };

    // Pick a back-end
    typedef msm::back::state_machine<Sm1_> Sm1;

    void test()
    {        
        Sm1 sm1;
        sm1.start(); 

        std::cout << "=== 1. Success Scenario ===" << std::endl;
        std::cout << "> Send StartButtonPressed()" << std::endl;
        sm1.process_event(StartButtonPressed());
        std::cout << "> Send ScanSucceeded()" << std::endl;
        sm1.process_event(ScanSucceeded());
        std::cout << "> Send ScanSucceeded()" << std::endl;
        sm1.process_event(ScanSucceeded());

        std::cout << "=== 2. Right Fail Scenario ===" << std::endl;
        std::cout << "> Send StartButtonPressed()" << std::endl;
        sm1.process_event(StartButtonPressed());
        std::cout << "> Send ScanFailed()" << std::endl;
        sm1.process_event(ScanFailed());

        std::cout << "=== 3. Left Fail Scenario ===" << std::endl;
        std::cout << "> Send StartButtonPressed()" << std::endl;
        sm1.process_event(StartButtonPressed());
        std::cout << "> Send ScanSucceeded()" << std::endl;
        sm1.process_event(ScanSucceeded());
        std::cout << "> Send ScanFailed()" << std::endl;
        sm1.process_event(ScanFailed());
    }
}

int main()
{
    test();
    return 0;
}

// Output:
//
// Press Start Button
// === 1. Success Scenario ===
// > Send StartButtonPressed()
// Right hand please.
// Scanning...
// > Send ScanSucceeded()
// Left hand please.
// Scanning...
// > Send ScanSucceeded()
// Register Completed.
// Press Start Button
// === 2. Right Fail Scenario ===
// > Send StartButtonPressed()
// Right hand please.
// Scanning...
// > Send ScanFailed()
// Register Failed.
// Press Start Button
// === 3. Left Fail Scenario ===
// > Send StartButtonPressed()
// Right hand please.
// Scanning...
// > Send ScanSucceeded()
// Left hand please.
// Scanning...
// > Send ScanFailed()
// Register Failed.
// Press Start Button
