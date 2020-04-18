//
// Created by Alexander Galilov on 10.04.2020.
// E-Mail: alexander.galilov@gmail.com 

#include "FSM.h"
#include "Watch.h"
#include "TOTPSelector.h"

FSMClass::FSMClass() : _state(INITIAL) {
}

void FSMClass::tick() {
    switch (_state) {
        case INITIAL:
            Watch.tick();
            if (Hardware.getButton().isHolded()) {
                Hardware.getButton().resetStates();
                RTCConfigurator.reset();
                _state = SET_DATE_TIME;
            } else if (Hardware.getButton().isClick()) {
                Hardware.getButton().resetStates();
                TOTPSelector.reset();
                _state = SELECT_TOTP;
            }
            break;
        case SELECT_TOTP:
            if (TOTPSelector.tick()) {
                _state = INITIAL;
                Watch.reset();
            }
            break;
        case SET_DATE_TIME:
            if (RTCConfigurator.tick()) {
                _state = INITIAL;
                Watch.reset();
            }
            break;
    }
}

FSMClass FSM;
