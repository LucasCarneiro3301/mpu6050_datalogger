#include "setup.h"

void input_setup(uint8_t pin) {
    gpio_init(pin);
    gpio_set_dir(pin, GPIO_IN);
    gpio_pull_up(pin);
}

void output_setup(uint8_t pin) {
    gpio_init(pin);
    gpio_set_dir(pin, GPIO_OUT);
    gpio_put(pin, false);
}

// Inicializa e configura a comunicação serial I2C 
void i2c_setup(i2c_inst_t *i2c, uint8_t sda, uint8_t scl) {
    i2c_init(i2c, 4e2 * 1e3);              // Inicialização I2C.
  
    gpio_set_function(sda, GPIO_FUNC_I2C);  // Define a função do pino GPIO para I2C
    gpio_set_function(scl, GPIO_FUNC_I2C);  // Define a função do pino GPIO para I2C
    gpio_pull_up(sda);                      // Pull up para linha de dados
    gpio_pull_up(scl);                      // Pull up para linha de clock
}

// Limpa o display
void ssd1306_clear(ssd1306_t *ssd) {
    ssd1306_fill(ssd, false);
    ssd1306_send_data(ssd);
}

// Inicializa e configura o display
void ssd1306_setup(ssd1306_t *ssd, i2c_inst_t *i2c_port) {
    ssd1306_init(ssd, WIDTH, HEIGHT, false, DISPLAY_ADDRESS, i2c_port);
    ssd1306_config(ssd);
    ssd1306_send_data(ssd);
}

// Inicializa e configura a matriz de LEDs 5x5 
void ws2812_setup() {
    PIO pio = pio0;
    int sm = 0;

    uint offset = pio_add_program(pio, &ws2812_program);
  
    ws2812_program_init(pio, sm, offset, WS2812_PIN, 800000, IS_RGBW); //Inicializa a matriz de leds
}

// Inicializa e configura os pinos do joystick como periféricos ADC
void adc_setup(uint8_t pin) {
    adc_gpio_init(pin);
    adc_init();
}