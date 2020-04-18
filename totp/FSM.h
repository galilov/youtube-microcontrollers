//
// Created by Alexander Galilov on 10.04.2020.
// E-Mail: alexander.galilov@gmail.com 

#ifndef FSM_H
#define FSM_H

#include "Hardware.h"
#include "RTCConfigurator.h"
#include "TOTP.h"
#include "Watch.h"

class FSMClass {
public:
    enum State {
        INITIAL, SET_DATE_TIME, SELECT_TOTP
    };
public:
    FSMClass();

    void tick();

private:
    State _state;
};

extern FSMClass FSM;
#endif //FSM_H
