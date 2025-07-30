#include "tasks.h"

void vTaskLed(void *params)
{
    uint8_t status;

    while (true)
    {
        status = (counter == 0) ? 0b001 : (counter < MAX_USERS - 1) ? 0b010 : (counter == MAX_USERS - 1) ? 0b110 : 0b100;

        // gpio_put(RED, (status >> 2) & 1);
        // gpio_put(GREEN, (status >> 1) & 1);
        // gpio_put(BLUE, (status >> 0) & 1);

        vTaskDelay(pdMS_TO_TICKS(200));
    }
}