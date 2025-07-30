#include "globals.h"
#include "tasks.h"
#include <math.h>

#define M_PI		3.14159265358979323846

void vTaskMonitoringScreen(void *params)
{
    char str_roll[16], str_pitch[16], ax[16], ay[16], az[16], gx[16], gy[16], gz[16];
    float roll = 0.0, pitch = 0.0;

    while (true)
    {
        if ((xSemaphoreTake(xDisplayMut, portMAX_DELAY)==pdTRUE))
        {
            if(screen == 1) 
            {
                if (xSemaphoreTake(xSensorMut, pdMS_TO_TICKS(50)) == pdTRUE)
                {
                    roll  = atan2(accel[1], accel[2]) * 180.0f / M_PI;
                    pitch = atan2(-accel[0], sqrt(accel[1]*accel[1] + accel[2]*accel[2])) * 180.0f / M_PI;
                    xSemaphoreGive(xSensorMut);
                }

                snprintf(ax,  sizeof(ax),  "%5.1f", accel[0]);
                snprintf(ay,  sizeof(ay),  "%5.1f", accel[1]);
                snprintf(az,  sizeof(az),  "%5.1f", accel[2]);
                snprintf(gx,  sizeof(gx),  "%5.1f", gyro[0]);
                snprintf(gy,  sizeof(gy),  "%5.1f", gyro[1]);
                snprintf(gz,  sizeof(gz),  "%5.1f", gyro[2]);
                snprintf(str_roll,  sizeof(str_roll),  "%5.1f", roll);
                snprintf(str_pitch, sizeof(str_pitch), "%5.1f", pitch);

                ssd1306_fill(&ssd, false);                            // Limpa o display
                ssd1306_rect(&ssd, 3, 3, 122, 60, true, false);        // Desenha um retângulo
                ssd1306_line(&ssd, 3, 13, 123, 13, true);             // Desenha uma linha horizontal
                ssd1306_line(&ssd, 3, 25, 123, 25, true);             // Desenha uma linha horizontal
                ssd1306_line(&ssd, 3, 37, 123, 37, true);             // Desenha uma linha horizontal
                ssd1306_line(&ssd, 3, 49, 123, 49, true);             // Desenha outra linha horizontal
                ssd1306_line(&ssd, 3, 50, 123, 50, true);             // Desenha outra linha horizontal
                ssd1306_draw_string(&ssd, "A", 40, 5);   // Escreve texto no display
                ssd1306_draw_string(&ssd, "G", 93, 5);   // Escreve texto no display

                ssd1306_draw_string(&ssd, "X", 7, 16);    // Escreve texto no display
                ssd1306_draw_string(&ssd, "Y", 7, 28); // Escreve texto no display
                ssd1306_draw_string(&ssd, "Z", 7, 40); // Escreve texto no display
                ssd1306_draw_string(&ssd, ax, 15, 16);    // Escreve texto no display
                ssd1306_draw_string(&ssd, gx, 76, 16);    // Escreve texto no display
                ssd1306_draw_string(&ssd, ay, 15, 28);    // Escreve texto no display
                ssd1306_draw_string(&ssd, gy, 76, 28);    // Escreve texto no display
                ssd1306_draw_string(&ssd, az, 15, 40);    // Escreve texto no display
                ssd1306_draw_string(&ssd, gz, 76, 40);    // Escreve texto no display


                ssd1306_line(&ssd, 17, 3, 17, 49, true);             // Desenha uma linha vertical
                ssd1306_line(&ssd, 70, 3, 70, 49, true);             // Desenha uma linha vertical

                ssd1306_line(&ssd, 63, 49, 63, 61, true);             // Desenha uma linha vertical
                ssd1306_line(&ssd, 61, 49, 61, 61, true);             // Desenha uma linha vertical
                ssd1306_line(&ssd, 62, 49, 62, 61, true);             // Desenha uma linha vertical
                ssd1306_draw_string(&ssd, str_roll, 10, 53);         // Exibe Roll       
                ssd1306_draw_string(&ssd, str_pitch, 74, 53);        // Exibe Pitch
                ssd1306_send_data(&ssd);                          // Atualiza o display
            }
            xSemaphoreGive(xDisplayMut);
        }
        vTaskDelay(pdMS_TO_TICKS(200));
    }
}