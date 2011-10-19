#include "atm_all.hpp"

int main()
{
    Atm::All t;
    t.start();
    t.process_event(Atm::HumanDetect());
    t.process_event(Atm::ChooseWithdraw());
    t.process_event(Atm::CardDetect());
    t.process_event(Atm::FingerDetect());
    t.process_event(Atm::AuthSuccess(Atm::AccountInfo(100)));
    t.process_event(Atm::EnterAmount(50));
    t.process_event(Atm::Ok());
    t.process_event(Atm::Ok());
}
