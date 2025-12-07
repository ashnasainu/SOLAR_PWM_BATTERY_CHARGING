/* LCD --> STM8s
 * LCD_RS --> PA1
 * LCD_EN --> PA2
 * LCD_DB4 --> PD1
 * LCD_DB5 --> PD2
 * LCD_DB6 --> PD3
 * LCD_DB7 --> PD4
 */

#define LCD_RS     GPIOC, GPIO_PIN_7
#define LCD_EN     GPIOC, GPIO_PIN_6
#define LCD_DB4    GPIOC, GPIO_PIN_5
#define LCD_DB5    GPIOC, GPIO_PIN_4
#define LCD_DB6    GPIOC, GPIO_PIN_3
#define LCD_DB7    GPIOB, GPIO_PIN_4

#include "STM8S.h"
#include "stm8s103_LCD_16x2.h"
#include "stm8s103_adc.h"
#include <stdio.h> 

#define LCD_BACKLIGHT_PORT GPIOB
#define LCD_BACKLIGHT_PIN  GPIO_PIN_5


// ----------------------------------------------------------
//  ADC Averaging Function (10 samples)
// ----------------------------------------------------------
uint16_t ADC_Read_Average(uint8_t channel)
{
    uint32_t sum = 0;
    uint8_t i;

    for(i = 0; i < 10; i++)    // take 10 readings
    {
        sum += ADC_Read(channel);
        delay_ms(2);           // small delay for stability
    }

    return (uint16_t)(sum / 10);   // return average
}


// ----------------------------------------------------------
// Print 4-digit integer on LCD
// ----------------------------------------------------------
void LCD_Print_Var(int var)
{
    char d4, d3, d2, d1;
    d4 = var % 10 + '0';
    d3 = (var / 10) % 10 + '0';
    d2 = (var / 100) % 10 + '0';
    d1 = (var / 1000) + '0';

    Lcd_Print_Char(d1);
    Lcd_Print_Char(d2);
    Lcd_Print_Char(d3);
    Lcd_Print_Char(d4);
}


// ----------------------------------------------------------
// MAIN PROGRAM
// ----------------------------------------------------------
int main(void)
{
    unsigned int ADC_value = 0;
    int ADC_voltage = 0;
    float NEW_ADC_VOLTAGE = 0;

    int integer_part;
    int decimal_part;
    char buffer[16];

    // LCD Backlight
    GPIO_Init(LCD_BACKLIGHT_PORT, LCD_BACKLIGHT_PIN, GPIO_MODE_OUT_PP_HIGH_FAST);
    GPIO_WriteHigh(LCD_BACKLIGHT_PORT, LCD_BACKLIGHT_PIN);

    // ADC Init
    CLK_PeripheralClockConfig(CLK_PERIPHERAL_ADC, ENABLE);
    GPIO_Init(GPIOC, GPIO_PIN_4, GPIO_MODE_IN_FL_IT);

    // LCD Init
    Lcd_Begin();
    Lcd_Clear();

    Lcd_Set_Cursor(1, 1);
    Lcd_Print_String("ADC on STM8S");
    delay_ms(1000);

    Lcd_Clear();
    Lcd_Set_Cursor(1, 1);
    Lcd_Print_String("ADC:");

    Lcd_Set_Cursor(2, 1);
    Lcd_Print_String("VOUT:");

    // Main Loop
    while (1)
    {
        // ---- Read Averaged ADC Value ----
        ADC_value = ADC_Read_Average(AIN3);

        // Convert ADC step to mV (4.8875 mV per step for 5V ref)
        ADC_voltage = ADC_value * 4.8875;

        // Convert to volts
        NEW_ADC_VOLTAGE = ADC_voltage / 1000.0;

        // Apply voltage divider (11x)
        NEW_ADC_VOLTAGE = NEW_ADC_VOLTAGE * 11;

        // Print RAW ADC value
        Lcd_Set_Cursor(1, 5);
        LCD_Print_Var(ADC_value);

        // Format float value manually
        integer_part = (int)NEW_ADC_VOLTAGE;
        decimal_part = (int)((NEW_ADC_VOLTAGE - integer_part) * 100);

        sprintf(buffer, "%d.%02d V", integer_part, decimal_part);

        // Print voltage
        Lcd_Set_Cursor(2, 7);
        Lcd_Print_String(buffer);

        delay_ms(500);
    }
}
