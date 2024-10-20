#ifndef SENSOR_H_
#define SENSOR_H_

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

    int8_t InitSensor(void);
    int8_t SetupSensor(void);
    strFdbkSensor ReadSensor(void);

#ifdef __cplusplus
}
#endif /* End of CPP guard */
#endif /* SENSOR_H_ */