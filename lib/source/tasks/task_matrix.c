#include "tasks.h"

void vTaskMatrix(void *params) 
{
    uint8_t  status; 
    
    while(true) 
    {
        status = (counter == 0) ? 0b001 : (counter < MAX_USERS - 1) ? 0b010 : (counter == MAX_USERS - 1) ? 0b110 : 0b100;

        ws2812_draw_count((uint8_t) counter, ((status >> 2) & 1)*0.035f, ((status >> 1) & 1)*0.035f, ((status >> 0) & 1)*0.035f);

        vTaskDelay(pdMS_TO_TICKS(200));
    }
}