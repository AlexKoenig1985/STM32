/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "bme280.h"
#include "sensor.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "i2c.h"

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

static struct bme280_dev dev;
static struct bme280_data comp_data;
static int8_t rslt;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */

int8_t user_i2c_write(const uint8_t id, const uint8_t reg_addr, uint8_t *data, const uint16_t len);

int8_t user_i2c_read(const uint8_t id, uint8_t reg_addr, uint8_t *data, const uint16_t len);

void user_delay_ms(const uint32_t period);
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

int8_t InitSensor(void)
{
    MX_I2C1_Init();

    dev.dev_id = BME280_I2C_ADDR_PRIM;
    dev.intf = BME280_I2C_INTF;
    dev.read = user_i2c_read;
    dev.write = user_i2c_write;
    dev.delay_ms = user_delay_ms;

    rslt = bme280_init(&dev);

    return rslt;
}

int8_t SetupSensor(void)
{
    dev.settings.osr_h = BME280_OVERSAMPLING_1X;
    dev.settings.osr_p = BME280_OVERSAMPLING_16X;
    dev.settings.osr_t = BME280_OVERSAMPLING_2X;
    dev.settings.filter = BME280_FILTER_COEFF_16;
    rslt = bme280_set_sensor_settings(BME280_OSR_PRESS_SEL | BME280_OSR_TEMP_SEL | BME280_OSR_HUM_SEL | BME280_FILTER_SEL, &dev);
    return rslt;
}

strFdbkSensor ReadSensor(void)
{
    strFdbkSensor TempHumPresSensor = {.humidity = 0, .pressure = 0, .temperature = 0, .rslt = BME280_OK};

    rslt = bme280_set_sensor_mode(BME280_FORCED_MODE, &dev);
    dev.delay_ms(40);

    rslt = bme280_get_sensor_data(BME280_ALL, &comp_data, &dev);

    TempHumPresSensor.humidity = comp_data.humidity / 1000.0;
    TempHumPresSensor.pressure = comp_data.pressure / 10024.0;
    TempHumPresSensor.temperature = comp_data.temperature / 100.0;
    TempHumPresSensor.rslt = rslt;

    return TempHumPresSensor;
}

int8_t user_i2c_write(const uint8_t id, const uint8_t reg_addr, uint8_t *data, const uint16_t len)
{
    int8_t buf[len + 1];
    // buf = malloc(len + 1);
    buf[0] = reg_addr;
    memcpy(buf + 1, data, len);

    if (HAL_I2C_Master_Transmit(&hi2c1, (id << 1), (uint8_t *)buf, len + 1, HAL_MAX_DELAY) != HAL_OK)
        return -1;

    // free(buf);
    return 0;
}

int8_t user_i2c_read(const uint8_t id, uint8_t reg_addr, uint8_t *data, const uint16_t len)
{
    if (HAL_I2C_Master_Transmit(&hi2c1, (id << 1), &reg_addr, 1, 10) != HAL_OK)
        return -1;
    if (HAL_I2C_Master_Receive(&hi2c1, (id << 1) | 0x01, data, len, 10) != HAL_OK)
        return -1;

    return 0;
}

void user_delay_ms(const uint32_t period)
{
    HAL_Delay(period);
}