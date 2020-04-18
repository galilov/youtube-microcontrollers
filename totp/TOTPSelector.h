//
// Created by Alexander Galilov on 12.04.2020.
// E-Mail: alexander.galilov@gmail.com 

#ifndef TOTPSELECTOR_H
#define TOTPSELECTOR_H

#include <Arduino.h>
#include "Storage.h"

class TOTPSelectorClass {
    enum State {
        INITIAL, SEND_TOTP_TO_HOST, RECEIVE_CONFIG
    };
public:
    TOTPSelectorClass();

    void reset();

    bool tick();

private:
    Storage::TotpKey _key;
    int8_t _keyIndexes[MAX_TOTP_KEY_COUNT];
    int8_t _keyCount, _prevKeyIndex;
    State _state;
    char _buf[20];
    char* _currentTotpCode;
};

extern TOTPSelectorClass TOTPSelector;

#endif //TOTPSELECTOR_H
