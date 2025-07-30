#include "tasks.h"
#include "ff.h"
#include "mpu6050.h"

void vTaskMPU6050(void *params)
{
    char buffer[100];
    int16_t raw_accel[3], raw_gyro[3];

    while (true)
    {
        mpu6050_read_raw(raw_accel, raw_gyro, 0);

        if (xSemaphoreTake(xSensorMut, pdMS_TO_TICKS(50)) == pdTRUE)
        {
            accel[0] = raw_accel[0] / 16384.0f;
            accel[1] = raw_accel[1] / 16384.0f;
            accel[2] = raw_accel[2] / 16384.0f;

            gyro[0] = raw_gyro[0]/131.0;
            gyro[1] = raw_gyro[1]/131.0;
            gyro[2] = raw_gyro[2]/131.0;

            if(sd_status == 2) {
                sprintf(buffer, "\n%i,%.1f,%.1f,%.1f,%.1f,%.1f,%.1f",
                        samples, accel[0], accel[1], accel[2], gyro[0], gyro[1], gyro[2]);

                UINT bw;
                FRESULT res = f_write(&file, buffer, strlen(buffer), &bw);
                if (res != FR_OK)
                {
                    printf("[ERRO] Não foi possível escrever no arquivo. Monte o Cartao.\n");
                }
                samples++;
            }


            xSemaphoreGive(xSensorMut);
        }

        vTaskDelay(pdMS_TO_TICKS(100));
    }
}