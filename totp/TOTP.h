// OpenAuthentication Time-based One-time Password Algorithm (RFC 6238)
// Arduino Library
//
// Luca Dentella (http://www.lucadentella.it)

#ifndef _TOTP_H
#define _TOTP_H

#include "Arduino.h"

class TOTPClass {

public:

    void setup(uint8_t *hmacKey, int keyLength, int timeStep = 30);

    char *getCode(long timeStamp, uint8_t length);

    char *getCodeFromSteps(long steps, uint8_t length);

private:

    uint8_t *_hmacKey;
    int _keyLength;
    int _timeStep;
    //uint8_t _byteArray[8];
    //uint8_t *_hash;
    int _offset;
    long _truncatedHash;
    char _code[10];
    long _prevSteps;
};

extern TOTPClass TOTP;

#endif
