#include "setup.h"
#include "tasks.h"
#include "pico/binary_info.h"
#include "mpu6050.h"
#include "sd_utils.h"

SemaphoreHandle_t xCounterSem;     // Semáforo usado para controlar o número de usuários presentes
SemaphoreHandle_t xDisplayMut;     // Mutex para proteger o acesso ao display
SemaphoreHandle_t xSensorMut;      // Mutex para proteger o acesso aos dados do MPU6050
SemaphoreHandle_t xResetSem;       // Semáforo binário usado para sinalizar a tarefa de reset
SemaphoreHandle_t xSdMut;          // Semáforo para gerenciar o uso do arquivo csv
TaskHandle_t xSDTaskHandle = NULL;
UBaseType_t counter = 0;           // Contador de usuários atuais no 

ssd1306_t ssd;

float accel[3], gyro[3], temp;
int samples = 0;
uint8_t screen = 1, option = 0;
int8_t sd_status = 0;
FIL file;
char filename[20] = "arq.csv";

volatile uint32_t last_time = 0;    // Armazena o último tempo em microssegundos

void gpio_irq_handler(uint gpio, uint32_t events);
void init();

int main()
{
    init();

    xCounterSem = xSemaphoreCreateCounting(MAX_USERS, 0);
    xDisplayMut = xSemaphoreCreateMutex();
    xResetSem = xSemaphoreCreateBinary();
    xSensorMut = xSemaphoreCreateMutex();
    xSdMut = xSemaphoreCreateMutex();

    xTaskCreate(vTaskMonitoringScreen, "Monitoring Screen Task",  configMINIMAL_STACK_SIZE + 128, NULL, 1, NULL);    // Exibe status, vagas e total
    xTaskCreate(vTaskSDControlPanel,        "SD Control Panel Task",  configMINIMAL_STACK_SIZE + 128, NULL, 1,  &xSDTaskHandle);   // Mostra alertas
    xTaskCreate(vTaskEntrada,      "Entrada Task",        configMINIMAL_STACK_SIZE + 128, NULL, 1, NULL);   // Lida com entradas (acessos)
    xTaskCreate(vTaskSaida,        "Saída Task",          configMINIMAL_STACK_SIZE + 128, NULL, 1, NULL);   // Lida com saídas (liberações)
    xTaskCreate(vTaskLed,          "LEDs Task",           configMINIMAL_STACK_SIZE + 128, NULL, 1, NULL);   // Gerencia LEDs de status
    xTaskCreate(vTaskSDAction,        "SD Action Task",          configMINIMAL_STACK_SIZE + 128, NULL, 1, NULL);   // Responsável por resetar o contador
    xTaskCreate(vTaskMatrix,       "Matrix Task",         configMINIMAL_STACK_SIZE + 128, NULL, 1, NULL);   // Controla matriz de LEDs (vagas ocupadas)
    xTaskCreate(vTaskMPU6050,      "MPU6050 Task",        configMINIMAL_STACK_SIZE + 128, NULL, 1, NULL);   // Tarefa que coleta os valores do sensor MPU6050

    vTaskStartScheduler();
    panic_unsupported();
}


// ISR do botão
void gpio_callback(uint gpio, uint32_t events) {
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;                  // Nenhum contexto de tarefa foi despertado
    xSemaphoreGiveFromISR(xResetSem, &xHigherPriorityTaskWoken);    // Libera o semáforo
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);                   // Troca o contexto da tarefa
}

void gpio_irq_handler(uint gpio, uint32_t events) {
    uint32_t current_time = to_us_since_boot(get_absolute_time());  // Obtém o tempo atual em microssegundos

    if (current_time - last_time > 5e5) {   // 200 ms de debouncing
        last_time = current_time; 
        if (gpio == BTNJ) {
            if(screen) reset_usb_boot(0, 0);
            else {
               gpio_callback(gpio, events);
            }
        } else if(gpio == BTNB) {
            screen = (screen + 1)%2;
        } else if(gpio == BTNA) {
            option = (option + 1 - screen)%5;
        }
    }
}

void init() {
    stdio_init_all();                               // Inicializa as saídas padrão
    output_setup(RED);                              // Inicializa os LEDs
    output_setup(BLUE);                             // Inicializa os LEDs
    output_setup(GREEN);                            // Inicializa os LEDs
    i2c_setup(I2C_PORT_0, I2C_SDA_0, I2C_SCL_0);    // Inicializa o barramento I2C
    i2c_setup(I2C_PORT_1, I2C_SDA_1, I2C_SCL_1);    // Inicializa o barramento I2C
    ssd1306_setup(&ssd, I2C_PORT_1);                // Inicializa o display OLED
    input_setup(BTNJ);                              // Inicializa os botões
    input_setup(BTNB);                              // Inicializa os botões
    input_setup(BTNA);                              // Inicializa os botões
    ws2812_setup();                                 // Inicializa a matriz de LEDs

    bi_decl(bi_2pins_with_func(I2C_SDA_0, I2C_SCL_0, GPIO_FUNC_I2C));
    mpu6050_reset();

    gpio_set_irq_enabled_with_callback(BTNJ, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler);   // Ativa a interrupção do botão de confirmação (BTNJ) na borda de descida
    gpio_set_irq_enabled_with_callback(BTNB, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler);   // Ativa a interrupção do botão de seleção de tela (BTNB) na borda de descida
    gpio_set_irq_enabled_with_callback(BTNA, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler);   // Ativa a interrupção do botão de seleção de opção (BTNA) na borda de descida
} 