//
// Created by Alexander Galilov on 12.04.2020.
// E-Mail: alexander.galilov@gmail.com 

/**
 * Data block format:
 * byte 0 - length of the following block
 * byte 1 - key number (0..)
 * byte 2 - totp length (4,5,6,7...)
 * byte 3 - step (or period): 30, 60, etc seconds
 * byte 4 .. length-1 key bytes
 */

#include "KeyConfigurator.h"
#include "Hardware.h"

KeyConfiguratorClass::KeyConfiguratorClass() : _state(INITIAL) {

}

bool KeyConfiguratorClass::tick() {
    if (!Serial) {
        return false;
    }
    while (Serial.available() > 0) {
        int b = Serial.read();
        //char str[4];
        //itoa(b, str, 16);
        if (b == -1) {
            // error
            _state = INITIAL;
            return true;
        }
        switch (_state) {
            case INITIAL:
                _dataLength = b;
                if (b > 3 && b < 24) {
                    _state = RECEIVE_KEY_NUM;
                    _totpKey.keyLength = _dataLength - 3;
                } else {
                    // error
                    Serial.write(static_cast<uint8_t >(0));
                    return true;
                }
                break;
            case RECEIVE_KEY_NUM:
                if (_dataLength-- > 0) {
                    _keyNum = b;
                    _state = RECEIVE_TOTP_LENGTH;
                } else {
                    // error
                    Serial.write(static_cast<uint8_t >(0));
                    _state = INITIAL;
                    return true;
                }
                break;
            case RECEIVE_TOTP_LENGTH:
                if (_dataLength-- > 0) {
                    _totpKey.totpLength = b;
                    _state = RECEIVE_STEP;
                } else {
                    // error
                    Serial.write(static_cast<uint8_t >(0));
                    _state = INITIAL;
                    return true;
                }
                break;
            case RECEIVE_STEP:
                if (_dataLength-- > 0) {
                    _totpKey.step = b;
                    _keyIndex = 0;
                    _state = RECEIVE_KEY_BYTES;
                } else {
                    // error
                    Serial.write(static_cast<uint8_t >(0));
                    _state = INITIAL;
                    return true;
                }
                break;
            case RECEIVE_KEY_BYTES:
                if (_dataLength-- > 0) {
                    _totpKey.key[_keyIndex++] = b;
                    if (_dataLength == 0) {
                        _totpKey.signature = Storage::VALID_KEY_SIGN;
                        Storage::putKey(_totpKey, _keyNum);
                        Serial.write(1); // OK
                        _state = INITIAL;
                        return true;
                    }
                }
                break;

        }
    } // while
    return false;
}

void KeyConfiguratorClass::reset() {
    _state = INITIAL;
    while(Serial.available() > 0) Serial.read(); // skip all buffered bytes
    Hardware.getLed().print("LOAD 0-E");
}

KeyConfiguratorClass KeyConfigurator;
