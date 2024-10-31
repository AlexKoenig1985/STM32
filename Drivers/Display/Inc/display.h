#ifndef DISPLAY_H_
#define DISPLAY_H_

/* Includes ------------------------------------------------------------------*/
#include "sensor.h"

/*! CPP guard */
#ifdef __cplusplus
extern "C"
{
#endif

    int8_t InitDisplay(void);
    int8_t WriteToDisplay(const uint8_t x_cord, const uint8_t y_cord, char *displaytesxt);
    void DisplaySensorData(strFdbkSensor SensorData);

#ifdef __cplusplus
}
#endif /* End of CPP guard */
#endif /* DISPLAY_H_ */