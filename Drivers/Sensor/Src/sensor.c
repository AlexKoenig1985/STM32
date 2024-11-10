/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "bme280.h"
#include "sensor.h"
#include <string.h>
#include "i2c.h"

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

static struct bme280_dev dev;
static int8_t rslt;
static strAdcPhy AdcSensorPhyValue = {.UBatt = 0, .UExtPhy = 0, .URefPhy = 0, .TempIntMicroPhy = UINT16_MAX};

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */

strAdcPhy ConvertAdcToPhyValue(Adc_strRawData RawAdcValue);

int8_t user_i2c_write(const uint8_t id, const uint8_t reg_addr, uint8_t *data, const uint16_t len);

int8_t user_i2c_read(const uint8_t id, uint8_t reg_addr, uint8_t *data, const uint16_t len);

static struct bme280_data I2C_ReadSensorRawData(struct bme280_dev *dev);

static int8_t SetupSensors(void);

void user_delay_ms(const uint32_t period);
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

int8_t InitSensors(void)
{

    dev.dev_id = BME280_I2C_ADDR_PRIM;
    dev.intf = BME280_I2C_INTF;
    dev.read = user_i2c_read;
    dev.write = user_i2c_write;
    dev.delay_ms = user_delay_ms;

    rslt = bme280_init(&dev);

    if (rslt == HAL_OK)
    {
        rslt = SetupSensors();
    }

    return rslt;
}

int8_t SetupSensors(void)
{
    dev.settings.osr_h = BME280_OVERSAMPLING_1X;
    dev.settings.osr_p = BME280_OVERSAMPLING_16X;
    dev.settings.osr_t = BME280_OVERSAMPLING_2X;
    dev.settings.filter = BME280_FILTER_COEFF_16;
    rslt = bme280_set_sensor_settings(BME280_OSR_PRESS_SEL | BME280_OSR_TEMP_SEL | BME280_OSR_HUM_SEL | BME280_FILTER_SEL, &dev);
    return rslt;
}

strFdbkSensor Sensor_Read(void)
{
    // struct bme280_data comp_data;
    strFdbkSensor TempHumPresSensor = {.humidity = 0, .pressure = 0, .temperature = 0, .rslt = BME280_OK};

    struct bme280_data SenorRawData = I2C_ReadSensorRawData(&dev);

    TempHumPresSensor.humidity = SenorRawData.humidity / 1000.0;
    TempHumPresSensor.pressure = SenorRawData.pressure / 10024.0;
    TempHumPresSensor.temperature = SenorRawData.temperature / 100.0;
    TempHumPresSensor.rslt = rslt;
    Adc_strRawData RawAdcValue = ADC_ReadRawValue();
    AdcSensorPhyValue = ConvertAdcToPhyValue(RawAdcValue);

    return TempHumPresSensor;
}

uint16_t Sensor_GetValues(Sensor_enuSensorType SensorType)
{
    uint16_t SensorPhyValue = INT16_MIN;
    switch (SensorType)
    {
    case UExt:
        SensorPhyValue = AdcSensorPhyValue.UExtPhy;
        break;
    case URef:
        SensorPhyValue = AdcSensorPhyValue.URefPhy;
        break;
    case UBatt:
        SensorPhyValue = AdcSensorPhyValue.UBatt;
        break;
    case Temp:
        SensorPhyValue = AdcSensorPhyValue.TempIntMicroPhy;
        break;
    default:
        break;
    }
    return SensorPhyValue;
}

strAdcPhy ConvertAdcToPhyValue(Adc_strRawData RawAdcValue)
{
    strAdcPhy PhyAdcValue = {.UBatt = 0, .UExtPhy = 0, .URefPhy = 0, .TempIntMicroPhy = UINT16_MAX};
    PhyAdcValue.UBatt = __HAL_ADC_CALC_DATA_TO_VOLTAGE(__VREFANALOG_VOLTAGE__, RawAdcValue.URawData.UBattRaw, ADC_RESOLUTION12b) * 4;

    PhyAdcValue.UExtPhy = __HAL_ADC_CALC_DATA_TO_VOLTAGE(__VREFANALOG_VOLTAGE__, RawAdcValue.URawData.UExtRaw, ADC_RESOLUTION12b);
    PhyAdcValue.URefPhy = (__HAL_ADC_CALC_DATA_TO_VOLTAGE(__VREFANALOG_VOLTAGE__, RawAdcValue.URawData.UIntRefRaw, ADC_RESOLUTION12b));
    PhyAdcValue.TempIntMicroPhy = __HAL_ADC_CALC_TEMPERATURE(__VREFANALOG_VOLTAGE__, RawAdcValue.TempRawData.TempIntUcRaw, ADC_RESOLUTION12b);

    return PhyAdcValue;
}

struct bme280_data I2C_ReadSensorRawData(struct bme280_dev *dev)
{
    struct bme280_data Raw_Data;
    rslt = bme280_set_sensor_mode(BME280_FORCED_MODE, dev);
    // dev->delay_ms(40);

    rslt = bme280_get_sensor_data(BME280_ALL, &Raw_Data, dev);

    return Raw_Data;
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