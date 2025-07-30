#ifndef GLOBALS_H
#define GLOBALS_H


#include "hardware/i2c.h"
#include <stdio.h>
#include <stdbool.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/adc.h"
#include "ssd1306.h"
#include "ws2812.h"
#include "pico/bootrom.h"
#include "hardware/pwm.h"
#include "hardware/timer.h"
#include "ff.h"


#define I2C_PORT_0 i2c0         // Barramento I2C "i2c0" para os sensores
#define I2C_SDA_0 0             // Define que o pino GPIO 0 será usado como SDA (linha de dados do I2C)
#define I2C_SCL_0 1             // Define que o pino GPIO 1 será usado como SCL (linha de clock do I2C)

#define I2C_PORT_1 i2c1   	    // Barramento I2C "i2c1" para o display
#define I2C_SDA_1 14      	    // Define que o pino GPIO 14 será usado como SDA (linha de dados do I2C)
#define I2C_SCL_1 15      	    // Define que o pino GPIO 15 será usado como SCL (linha de clock do I2C)

#define DISPLAY_ADDRESS 0x3C    // Define o endereço I2C do dispositivo (0x3C é o endereço padrão de muitos displays OLED SSD1306)
#define DISPLAY_WIDTH   128
#define DISPLAY_HEIGHT  64

#define BTNA 5          	// Botão A
#define BTNB 6          	// Botão B
#define BTNJ 22             // Botão joystick
#define SW_BNT 22           // Botão SW
#define WS2812_PIN 7    	// Matriz de LEDs 5x5
#define GREEN 11        // Led verde
#define RED 13          // Led vermelho
#define BLUE 12         // Led azul
#define BUZZER 10       // Buzzer

extern float accel[3], gyro[3], temp;
extern int samples;

extern float roll, pitch;

extern uint8_t screen;
extern uint8_t option;
extern int8_t sd_status;

extern bool itsAlreadyFull;
extern bool itsAlreadyEmpty;

extern char filename[20];
extern FIL file;

// Frequências das notas musicais (em Hz)
enum Notes {
    DO = 2640, // Dó
    RE = 2970, // Ré
    MI = 3300, // Mi
    FA = 3520, // Fá
    SOL = 3960, // Sol
    LA = 4400, // Lá
    SI = 4950,  // Si
    DO_ALTO = 5280,  // Dó uma oitava acima (C5)
    LA_BAIXO = 880
};

extern ssd1306_t ssd;

extern char msg[256];

#endif