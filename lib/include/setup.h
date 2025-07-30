#ifndef CONFIG_H
#define CONFIG_H

#include "globals.h"

void input_setup(uint8_t pin);
void output_setup(uint8_t pin);
void i2c_setup(i2c_inst_t *i2c, uint8_t sda, uint8_t scl);
void ssd1306_clear(ssd1306_t *ssd);
void ssd1306_setup(ssd1306_t *ssd, i2c_inst_t *i2c_port);
void ws2812_setup();
void adc_setup(uint8_t pin);

#endif