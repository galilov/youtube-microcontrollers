//
// Created by Alexander Galilov on 12.04.2020.
// E-Mail: alexander.galilov@gmail.com 

#ifndef KEY_CONFIGURATOR_H
#define KEY_CONFIGURATOR_H

#include <Arduino.h>
#include "Storage.h"

class KeyConfiguratorClass {
    enum State {
        INITIAL, RECEIVE_KEY_NUM, RECEIVE_TOTP_LENGTH, RECEIVE_STEP, RECEIVE_KEY_BYTES
    };
public:
    KeyConfiguratorClass();

    bool tick();

    void reset();

private:
    State _state;
    int8_t _dataLength;
    int8_t _keyIndex;
    uint8_t  _keyNum;
    Storage::TotpKey _totpKey;
};

extern KeyConfiguratorClass KeyConfigurator;

#endif //KEY_CONFIGURATOR_H
