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

    typedef struct strFdbkSensor
    {
        float temperature;
        float humidity;
        float pressure;
        int8_t rslt;
    } strFdbkSensor;

    int8_t InitSensors(void);

    strFdbkSensor Sensor_Read(void);

#ifdef __cplusplus
}
#endif /* End of CPP guard */
#endif /* SENSOR_H_ */