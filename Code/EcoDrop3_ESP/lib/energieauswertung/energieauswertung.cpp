#include "energieauswertung.h"
#include <Wire.h>
#include <INA3221.h>


#define PRINT_DEC_POINTS 3  


// definition des INA3221
INA3221 ina_0(INA3221_ADDR40_GND);

void current_measure_init() {
    ina_0.begin(&Wire);
    ina_0.reset();
    ina_0.setShuntRes(100, 100, 100);
}

void setup() {
    current_measure_init();

    while (!Serial) {
        delay(1);
    }
    // Set shunt resistors to 10 mOhm for all channels
}

void loop() {
    Serial.printf(
        "A1%3.0fma %1.1fV A2%3.0fma %1.1fV\r\n",
        ina_0.getCurrent(INA3221_CH1) * 1000, ina_0.getVoltage(INA3221_CH1),
        ina_0.getCurrent(INA3221_CH2) * 1000, ina_0.getVoltage(INA3221_CH2));

    delay(1000);
}
