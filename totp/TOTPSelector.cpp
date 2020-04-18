//
// Created by Alexander Galilov on 12.04.2020.
// E-Mail: alexander.galilov@gmail.com 
#include "main.h"
#include "TOTPSelector.h"
#include "Hardware.h"
#include "TOTP.h"
#include "KeyConfigurator.h"
#include <Keyboard.h>

TOTPSelectorClass::TOTPSelectorClass() : _state(INITIAL), _keyCount(0), _prevKeyIndex(-1) {

}

void TOTPSelectorClass::reset() {
    int8_t j = 0;
    for (int8_t i = 0; i < MAX_TOTP_KEY_COUNT; i++) {
        if (Storage::hasKeyAtIndex(i)) {
            _keyIndexes[j++] = i;
        }
    }
    _keyCount = j;
    _prevKeyIndex = -1;
    _currentTotpCode = nullptr;
    _state = INITIAL;
    
}

bool TOTPSelectorClass::tick() {
    switch (_state) {
        case INITIAL:
            if (_keyCount == 0) {
                KeyConfigurator.reset();
                _state = RECEIVE_CONFIG;
                return false;
            }
            uint8_t keyIndex;
            if (_keyCount > 1) {
                keyIndex = _keyIndexes[Hardware.getEncoderPosition(0, _keyCount - 1)];
            } else {
                keyIndex = _keyIndexes[0];
            }
            //Serial.println(Hardware.getEncoderPosition(0, _keyCount - 1));
            if (_prevKeyIndex != keyIndex) {
                _prevKeyIndex = keyIndex;
                Storage::getKey(_key, keyIndex);
                TOTP.setup(_key.key, _key.keyLength, _key.step);
                _currentTotpCode = TOTP.getCode(Hardware.getRtc().now().unixtime(), _key.totpLength);
                sprintf(_buf, "%u.%s", keyIndex, _currentTotpCode);
                Hardware.getLed().print(_buf);
            }
            if (Hardware.getButton().isHolded()) {
                Hardware.getButton().resetStates();
                KeyConfigurator.reset();
                _state = RECEIVE_CONFIG;
            } else if (Hardware.getButton().isClick()) {
                Hardware.getButton().resetStates();
                _state = SEND_TOTP_TO_HOST;
            }
            break;
        case SEND_TOTP_TO_HOST:
            Keyboard.println(_currentTotpCode);
            return true;
        case RECEIVE_CONFIG:
            if (KeyConfigurator.tick()) {
                _state = INITIAL;
                reset();
            }
            break;
    }
    return false;
}

TOTPSelectorClass TOTPSelector;
