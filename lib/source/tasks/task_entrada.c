#include "tasks.h"

// Tarefa responsável por registrar entradas de usuários
void vTaskEntrada(void *params) 
{
    bool last_state = true;
    while (true) 
    {
        // bool state = !gpio_get(BTNJ);

        // if (state && state!=last_state)
        // {
        //     if(xSemaphoreGive(xCounterSem) == pdTRUE)   // Tenta adicionar um "token" ao semáforo — representando uma nova entrada
        //         counter = uxSemaphoreGetCount(xCounterSem); // Se conseguiu adicionar, atualiza o contador com o valor atual do semáforo
        //     else 
        //         itsAlreadyFull = true;
        // }
        
        // last_state = state;

        vTaskDelay(pdMS_TO_TICKS(100)); // Debounce + polling
    }
}