#include "energieauswertung.h"
#include <Wire.h>
#include <INA3221.h>


#define PRINT_DEC_POINTS 3  

#define I2C_SDA A4
#define I2C_SCL A5

// definition des INA3221
INA3221 ina(INA3221_ADDR40_GND);

void current_measure_init() {
    Wire.begin(I2C_SDA, I2C_SCL);
    Wire.setClock(100000);
    delay(10);

    ina.begin(&Wire);
    ina.reset();
    ina.setShuntRes(100, 100, 100);
}

float getCurrent(int channel){
    float current;
    switch(channel){
        case 1:
            current = ina.getCurrent(INA3221_CH1);
            break;
        case 2:
            current = ina.getCurrent(INA3221_CH2);
            break;
        case 3:
            current = ina.getCurrent(INA3221_CH3);
            break;
    }
    return current;
}

float getVoltage(int channel){
    float voltage;
    switch(channel){
        case 1:
            voltage = ina.getVoltage(INA3221_CH1);
            break;
        case 2:
            voltage = ina.getVoltage(INA3221_CH2);
            break;
        case 3:
            voltage = ina.getVoltage(INA3221_CH3);
            break;
    }
    return voltage;
}