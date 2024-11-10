#ifndef SENSOR_H_
#define SENSOR_H_

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include "adc.h"
/*! CPP guard */
#ifdef __cplusplus
extern "C"
{
#endif

    typedef enum
    {
        UExt = 0,
        URef = 1,
        UBatt = 2,
        Temp = 3,
    } Sensor_enuSensorType;
    typedef struct strFdbkSensor
    {
        float temperature;
        float humidity;
        float pressure;
        int8_t rslt;
    } strFdbkSensor;

    typedef struct strAdcPhy
    {
        uint16_t UExtPhy;
        uint16_t URefPhy;
        uint16_t UBatt;
        uint16_t TempIntMicroPhy;
    } strAdcPhy;

    int8_t InitSensors(void);

    strFdbkSensor Sensor_Read(void);

    uint16_t Sensor_GetValues(Sensor_enuSensorType SensorType);
#ifdef __cplusplus
}
#endif /* End of CPP guard */
#endif /* SENSOR_H_ */