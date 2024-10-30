/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "display.h"
#include "ssd1306.h"
#include "ssd1306_fonts.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define DISPLAY_X_START 0
#define DISPLAY_Y_START 0
#define DISPLAY_X_TEMPERATURE 0
#define DISPLAY_Y_TEMPERATURE 0
#define DISPLAY_X_PRESSURE 0
#define DISPLAY_Y_PRESSURE 10
#define DISPLAY_X_HUMIDITY 0
#define DISPLAY_Y_HUMIDITY 20
/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
int8_t InitDisplay(void)
{
    HAL_StatusTypeDef rslt;
    ssd1306_Init();
    ssd1306_Fill(Black);
    ssd1306_UpdateScreen();
    char myText[40] = "Startup";
    rslt = WriteToDisplay(DISPLAY_X_START, DISPLAY_Y_START, myText);
    return rslt;
}

int8_t WriteToDisplay(const uint8_t x_cord, const uint8_t y_cord, char *displaytext)
{
    HAL_StatusTypeDef rslt;
    char ReturnVal;
    ssd1306_SetCursor(x_cord, y_cord);

    ReturnVal = ssd1306_WriteString(displaytext, Font_7x10, White);

    if (ReturnVal == '\000')
    {
        ssd1306_UpdateScreen();
        rslt = HAL_OK;
    }
    else
    {
        rslt = HAL_ERROR;
    }
    return rslt;
}

void WriteSensorData(strFdbkSensor SensorData)
{
    int whole = (int)SensorData.temperature;
    int decimal = (int)((SensorData.temperature - whole) * 100);
    char myText[18] = "";
    HAL_StatusTypeDef rslt = HAL_OK;
    sprintf(myText, "Temperature %d.%2dC", whole, decimal);
    rslt = WriteToDisplay(DISPLAY_X_TEMPERATURE, DISPLAY_Y_TEMPERATURE, myText);
    if (rslt == HAL_OK)
    {
        whole = (int)SensorData.pressure;

        sprintf(myText, "Pressure %d hPa", whole);
        rslt = WriteToDisplay(DISPLAY_X_PRESSURE, DISPLAY_Y_PRESSURE, myText);
    }
    if (rslt == HAL_OK)
    {
        whole = (int)SensorData.humidity;
        decimal = (int)((SensorData.humidity - whole) * 100);
        sprintf(myText, "Humidity %d.%2d%%", whole, decimal);
        rslt = WriteToDisplay(DISPLAY_X_HUMIDITY, DISPLAY_Y_HUMIDITY, myText);
    }
}
