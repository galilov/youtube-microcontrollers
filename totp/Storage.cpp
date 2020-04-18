//
// Created by Alexander Galilov on 11.04.2020.
// E-Mail: alexander.galilov@gmail.com 

#include "Storage.h"
#include <EEPROM.h>

namespace Storage {

    void getUtc(UTC &utc) {
        EEPROM.get(0, utc);
        if (utc.hours < -24 || utc.hours > 24
            || utc.minutes < 0 || utc.minutes > 59) {
            utc.minutes = 0;
            utc.hours = 0;
            EEPROM.put(0, utc);
        }
    }

    void putUtc(const UTC &utc) {
        EEPROM.put(0, utc);
    }

    void putKey(const TotpKey &key, uint8_t index) {
        EEPROM.put((index % MAX_TOTP_KEY_COUNT) * sizeof(TotpKey) + sizeof(UTC), key);
    }

    bool getKey(TotpKey &key, uint8_t index) {
        EEPROM.get((index % MAX_TOTP_KEY_COUNT)  * sizeof(TotpKey) + sizeof(UTC), key);
        return (key.signature == VALID_KEY_SIGN);
    }

    bool hasKeyAtIndex(uint8_t index) {
        uint32_t signature;
        EEPROM.get((index % MAX_TOTP_KEY_COUNT) * sizeof(TotpKey) + sizeof(UTC), signature);
        return (signature == VALID_KEY_SIGN);
    }

    void clearKey(uint8_t index) {
        TotpKey key;
        EEPROM.put((index % MAX_TOTP_KEY_COUNT) * sizeof(TotpKey) + sizeof(UTC), key);
    }
}
