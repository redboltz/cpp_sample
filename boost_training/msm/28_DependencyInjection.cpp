#include <iostream>
#include <boost/msm/back/state_machine.hpp>

#include <boost/msm/front/state_machine_def.hpp>
#include <boost/msm/front/functor_row.hpp>

#define USE_TEMPLATE

// Implementation
namespace {
    namespace msm = boost::msm;
    namespace msmf = boost::msm::front;
    namespace mpl = boost::mpl;
    struct StateImp_:msm::front::state_machine_def<StateImp_>
    {
        struct Entry:msm::front::entry_pseudo_state<> {};
        struct StateImp_1 : msm::front::state<> {};
        typedef Entry initial_state;
        struct transition_table:mpl::vector<
            //          Start  Event       Next          Action      Guard
            msmf::Row < Entry, msmf::none, StateImp_1,   msmf::none, msmf::none >
        > {};
    };
    typedef msm::back::state_machine<StateImp_> StateImp;
}

// Interface side
namespace {
    namespace msm = boost::msm;
    namespace msmf = boost::msm::front;
    namespace mpl = boost::mpl;

    struct Event1 {};
#if defined(USE_TEMPLATE)
    template <class Imp>
    struct Sm1_:msm::front::state_machine_def<Sm1_<Imp> >
#else
    struct Sm1_:msm::front::state_machine_def<Sm1_>
#endif
    {
        struct State1:msm::front::state<> {};
        // Connect interface to implementation
#if defined(USE_TEMPLATE)
        struct State2:Imp {};
#else
        struct State2:StateImp {};
#endif
        typedef State1 initial_state;
        // Transition table
        struct transition_table:mpl::vector<
            //          Start   Event       Next                 Action      Guard
#if defined(USE_TEMPLATE)
            msmf::Row < State1, Event1,     State2::entry_pt
                                            <StateImp_::Entry>,  msmf::none, msmf::none >
#else
            msmf::Row < State1, Event1,     State2::entry_pt
                                            <StateImp_::Entry>,  msmf::none, msmf::none >
#endif
        > {};
    };

    // Pick a back-end
#if defined(USE_TEMPLATE)
    template <class Imp>
    struct Sm1:msm::back::state_machine<Sm1_<Imp> > {};
#else
    typedef msm::back::state_machine<Sm1_> Sm1;
#endif

    void test()
    {
#if defined(USE_TEMPLATE)
        Sm1<StateImp> sm1;
#else
        Sm1 sm1;
#endif
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
