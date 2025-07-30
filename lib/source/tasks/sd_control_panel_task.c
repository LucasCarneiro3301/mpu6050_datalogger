#include "tasks.h"
#include "buzzer.h"

void vTaskSDControlPanel(void *params) 
{
    while (true)
    {
        if ((xSemaphoreTake(xDisplayMut, portMAX_DELAY)==pdTRUE))
        {
            if(screen == 0)                                             // Se já foi cheia ou esvaziada
            {
                ssd1306_fill(&ssd, false);                            // Limpa o display
                ssd1306_rect(&ssd, 2, 2, 123, 61, true, false);        // Desenha um retângulo

                if(sd_status!= 2) {
                    ssd1306_draw_string(&ssd, (option==0)?"} Montar":"- Montar", 20, 5);    // Escreve texto no display
                    ssd1306_draw_string(&ssd,(option==1)?"} Gravar":"- Gravar", 20, 17); // Escreve texto no display
                    ssd1306_draw_string(&ssd, (option==2)?"} Limpar":"- Limpar", 20, 29); // Escreve texto no display
                    ssd1306_draw_string(&ssd, (option==3)?"} Desmontar":"- Desmontar", 20, 41); // Escreve texto no display
                    ssd1306_draw_string(&ssd, (option==4)?"} Printar":"- Printar", 20, 53); // Escreve texto no display
                } else if(sd_status==2) {
                    ssd1306_draw_string(&ssd, "Gravando!!!", 30, 20);                   // Escreve texto no display
                    ssd1306_draw_string(&ssd, "os dados...", 30, 32);                  // Escreve texto no display
                }

                ssd1306_send_data(&ssd);                          // Atualiza o display
            }

            xSemaphoreGive(xDisplayMut);
        }
        vTaskDelay(pdMS_TO_TICKS(200));
    }
}