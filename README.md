# PWM Battery Charging Circuit

## Project Overview
A battery charging circuit project implemented on STM8S microcontroller using Pulse Width Modulation (PWM) and ADC for voltage monitoring.

## Hardware Requirements
- STM8S103 microcontroller
- 16x2 LCD display
- Voltage divider circuit (11x attenuation)
- Battery to be charged
- Power supply for the circuit

## Pin Configuration
### LCD Connections:
- LCD_RS --> PC7
- LCD_EN --> PC6
- LCD_DB4 --> PC5
- LCD_DB5 --> PC4
- LCD_DB6 --> PC3
- LCD_DB7 --> PB4
- LCD Backlight --> PB5

### ADC Connections:
- ADC Input: Channel 3 (AIN3)

## How It Works
1. The ADC reads the battery voltage (via voltage divider)
2. Raw ADC value is converted to voltage (4.8875mV per step)
3. Voltage is compensated by multiplying by 11 (voltage divider ratio)
4. Results are displayed on the LCD:
   - First line: Raw ADC value
   - Second line: Calculated voltage (V)
