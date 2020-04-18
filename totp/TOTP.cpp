// OpenAuthentication Time-based One-time Password Algorithm (RFC 6238)
// For the complete description of the algorithm see
// http://tools.ietf.org/html/rfc4226#section-5.3
//
// Luca Dentella (http://www.lucadentella.it)

#include "TOTP.h"
#include "Sha1.h"

// Init the library with the private key, its length and the timeStep duration
void TOTPClass::setup(uint8_t *hmacKey, int keyLength, int timeStep) {
    _prevSteps = -1;
    _hmacKey = hmacKey;
    _keyLength = keyLength;
    _timeStep = timeStep;
};

// Generate a code, using the timestamp provided
char *TOTPClass::getCode(long timeStamp, uint8_t length) {

    long steps = timeStamp / _timeStep;
    if (steps == _prevSteps) return _code;
    _prevSteps = steps;
    return getCodeFromSteps(steps, length);
}

// Generate a code, using the number of steps provided
char *TOTPClass::getCodeFromSteps(long steps, uint8_t length) {
    const static long dividers[] = {1, 10, 100, 1000, 10000, 100000, 1000000, 10000000, 100000000};
    // STEP 0, map the number of steps in a 8-bytes array (counter value)
    static uint8_t byteArray[8];
    byteArray[0] = 0x00;
    byteArray[1] = 0x00;
    byteArray[2] = 0x00;
    byteArray[3] = 0x00;
    byteArray[4] = (int) ((steps >> 24) & 0xFF);
    byteArray[5] = (int) ((steps >> 16) & 0xFF);
    byteArray[6] = (int) ((steps >> 8) & 0XFF);
    byteArray[7] = (int) ((steps & 0XFF));

    // STEP 1, get the HMAC-SHA1 hash from counter and key
    Sha1.initHmac(_hmacKey, _keyLength);
    Sha1.write(byteArray, 8);
    uint8_t *hash = Sha1.resultHmac();

    // STEP 2, apply dynamic truncation to obtain a 4-bytes string
    _offset = hash[20 - 1] & 0xF;
    _truncatedHash = 0;
    for (int j = 0; j < 4; ++j) {
        _truncatedHash <<= 8;
        _truncatedHash |= hash[_offset + j];
    }

    // STEP 3, compute the OTP value
    _truncatedHash &= 0x7FFFFFFF;

    _truncatedHash %= dividers[length % (sizeof(dividers) / sizeof(dividers[0]))];

    // convert the value in string, with heading zeroes
    char fmt[8];
    sprintf(fmt, "%%0%dld", length);
    sprintf(_code, fmt, _truncatedHash);
    return _code;
}

TOTPClass TOTP;
