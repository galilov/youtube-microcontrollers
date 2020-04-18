//
// Created by Alexander Galilov on 10.04.2020.
// E-Mail: alexander.galilov@gmail.com 

#ifndef RTCCONFIGURATOR_H
#define RTCCONFIGURATOR_H

#include "main.h"
#include "Hardware.h"
#include "Storage.h"

class RTCConfiguratorClass {
    static const unsigned long BLINK_PERIOD = 200;
    enum State {
        INITIAL, UTC_HOUR_OFFSET, UTC_MINUTE_OFFSET, YEAR, MONTH, DAY, HOUR, MINUTE, SECOND
    };
public:
    RTCConfiguratorClass();

    void reset();

    bool tick();

private:
    State _state;
    Storage::UTC _utc, _prevUtc;
    uint16_t _year, _prevYear;
    uint8_t _month, _day, _prevMonth, _prevDay;
    uint8_t _hour, _minute, _second, _prevHour, _prevMinute, _prevSecond;
    unsigned long _prevTs;
    const char *_utcFormat;
    const char *_utcFormatHoursOff;
    const char *_utcFormatMinutesOff;
    const char *_dateFormat;
    const char *_dateFormatYearsOff;
    const char *_dateFormatMonthsOff;
    const char *_dateFormatDaysOff;
    const char *_timeFormat;
    const char *_timeFormatHoursOff;
    const char *_timeFormatMinutesOff;
    const char *_timeFormatSecondsOff;
    char _buf[20];

    void _updateTimeDisplay();

    void _updateUtcDisplay();

    void _updateDateDisplay();

    void _adjustUtcHourOffset();

    void _adjustUtcMinuteOffset();

    void _adjustYear();

    void _adjustMonth();

    void _adjustDay();

    void _adjustHour();

    void _adjustMinute();

    bool _adjustSecond();

    void _initValues();
};

extern RTCConfiguratorClass RTCConfigurator;
#endif //RTCCONFIGURATOR_H
