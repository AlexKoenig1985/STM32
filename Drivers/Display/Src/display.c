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
HAL_StatusTypeDef DisplayValues(const char *label, float value, const char *unit, const uint8_t x_cord, const uint8_t y_cord);
/* Private user code ---------------------------------------------------------*/

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

void DisplaySensorData(strFdbkSensor SensorData)
{
    HAL_StatusTypeDef rslt = HAL_OK;

    rslt = DisplayValues("Temperature", SensorData.temperature, "C", DISPLAY_X_TEMPERATURE, DISPLAY_Y_TEMPERATURE);
    if (rslt == HAL_OK)
    {
        rslt = DisplayValues("Pres", SensorData.pressure, "hPa", DISPLAY_X_PRESSURE, DISPLAY_Y_PRESSURE);
    }
    if (rslt == HAL_OK)
    {
        rslt = DisplayValues("Humidity", SensorData.humidity, "%", DISPLAY_X_HUMIDITY, DISPLAY_Y_HUMIDITY);
    }
}

HAL_StatusTypeDef DisplayValues(const char *label, float value, const char *unit, const uint8_t x_cord, const uint8_t y_cord)
{
    int whole = (int)value;
    int decimal = (int)((value - whole) * 100);
    char TextBuffer[18] = "";
    sprintf(TextBuffer, "%s %d.%01d%s", label, whole, decimal, unit);
    HAL_StatusTypeDef rslt = WriteToDisplay(x_cord, y_cord, TextBuffer);
    return rslt;
}
