#include "tasks.h"

// Tarefa responsável por detectar saídas de usuários
void vTaskSaida(void *params) 
{
    bool last_state = true;

    while (true) 
    {
        // bool state = !gpio_get(BTNB);

        // if (state && state!=last_state)
        // {            
        //     if(xSemaphoreTake(xCounterSem, 0) == pdTRUE)    // Tenta "liberar" uma vaga — ou seja, alguém saiu, então decrementa o contador
        //         counter = uxSemaphoreGetCount(xCounterSem); // Se conseguiu tirar um token do semáforo, atualiza o contador
        //     else 
        //         //itsAlreadyEmpty= true;
        // }
        
        // last_state = state;

        vTaskDelay(pdMS_TO_TICKS(100)); // Debounce + polling
    }
}