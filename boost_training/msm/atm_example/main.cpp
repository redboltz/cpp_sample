#include "atm_all.hpp"

int main()
{
    Atm::All t;
    t.start();
    t.process_event(Atm::HumanDetect());
    t.process_event(Atm::Withdraw());
    t.process_event(Atm::FingerDetect());
    t.process_event(Atm::AuthSuccess(Atm::AccountInfo(100)));
    t.process_event(Atm::Ok());
}
