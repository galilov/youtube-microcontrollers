//
// Created by Alexander Galilov on 11.04.2020.
// E-Mail: alexander.galilov@gmail.com 

#ifndef STORAGE_H
#define STORAGE_H

#include <Arduino.h>

#define MAX_TOTP_KEY_COUNT 8

namespace Storage {

    struct UTC {
        int8_t hours, minutes;
    };
    static const uint32_t VALID_KEY_SIGN = 19770923LU;

    struct TotpKey {
        uint32_t signature;
        uint8_t keyLength, totpLength, step;
        uint8_t key[20];

        TotpKey() : signature(0), keyLength(0), totpLength(0) {}
    };

    void getUtc(UTC &utc);

    void putUtc(const UTC &utc);

    void putKey(const TotpKey &key, uint8_t index);

    bool getKey(TotpKey &key, uint8_t index);

    bool hasKeyAtIndex(uint8_t index);

    void clearKey(uint8_t index);
}

#endif //STORAGE_H
