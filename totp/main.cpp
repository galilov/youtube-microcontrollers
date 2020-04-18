//
// Created by Alexander Galilov on 09.04.2020.
// E-Mail: alexander.galilov@gmail.com 
#include <Arduino.h>
#include "main.h"
#include "FSM.h"
#include "Hardware.h"

void setup() {
    Hardware.setup();
}

void loop() {
    Hardware.tick();
    FSM.tick();
}
