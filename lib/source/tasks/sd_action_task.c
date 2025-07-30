#include "tasks.h"
#include "buzzer.h"
#include "sd_utils.h"

void jingle(bool option) {
    for (int i = 0; i < 2; i++) {
        play_buzzer(option?RE:DO);
        vTaskDelay(pdMS_TO_TICKS(300));
        
        stop_buzzer();
        vTaskDelay(pdMS_TO_TICKS(200));
    }

    stop_buzzer();
    vTaskDelay(pdMS_TO_TICKS(600));
}

void leds(bool r, bool g, bool b) {
    gpio_put(RED, r);
    gpio_put(GREEN, g);
    gpio_put(BLUE, b);
}

void vTaskSDAction(void *params)
{
    while(true) 
    {
        if (xSemaphoreTake(xActSem, portMAX_DELAY) == pdTRUE) 
        {
            if(sd_status==2) {
                printf("Gravação concluída!\n");
                sd_status=1;
                f_close(&file);
                if ((xSemaphoreTake(xDisplayMut, portMAX_DELAY)==pdTRUE)) {
                    ssd1306_fill(&ssd, false);                              // Limpa o display
                    ssd1306_rect(&ssd, 3, 3, 122, 60, true, false);         // Desenha um retângulo
                    ssd1306_draw_string(&ssd, "Gravacao", 20, 20);          // Escreve texto no display
                    ssd1306_draw_string(&ssd, "concluida!!", 20, 32);      // Escreve texto no display
                    ssd1306_send_data(&ssd);                                // Atualiza o display

                    jingle(false);

                    vTaskDelay(pdMS_TO_TICKS(1000));

                    leds(0,1,0);

                    xSemaphoreGive(xDisplayMut);
                }
            }
            else {
                if(!option) {
                    if ((xSemaphoreTake(xDisplayMut, portMAX_DELAY)==pdTRUE)) {
                        ssd1306_fill(&ssd, false);                              // Limpa o display
                        ssd1306_rect(&ssd, 3, 3, 122, 60, true, false);         // Desenha um retângulo
                        ssd1306_draw_string(&ssd, "Montando", 30, 20);          // Escreve texto no display
                        ssd1306_draw_string(&ssd, "Cartao SD...", 20, 32);      // Escreve texto no display
                        ssd1306_send_data(&ssd);                                // Atualiza o display

                        leds(1,1,0);

                        vTaskDelay(pdMS_TO_TICKS(1000));
                    
                        xSemaphoreGive(xDisplayMut);
                    }

                    printf("\nMontando o SD...\n");

                    sd_status = run_mount();

                    if(sd_status==1) {
                        if ((xSemaphoreTake(xDisplayMut, portMAX_DELAY)==pdTRUE)) {
                            leds(0,1,0);

                            ssd1306_fill(&ssd, false);                              // Limpa o display
                            ssd1306_rect(&ssd, 3, 3, 122, 60, true, false);         // Desenha um retângulo
                            ssd1306_draw_string(&ssd, "Cartao SD", 30, 16);         // Escreve texto no display
                            ssd1306_draw_string(&ssd, "Montado com", 20, 28);       // Escreve texto no display
                            ssd1306_draw_string(&ssd, "sucesso!!!", 20, 40);        // Escreve texto no display
                            ssd1306_send_data(&ssd);                                // Atualiza o display

                            vTaskDelay(pdMS_TO_TICKS(2000));

                            xSemaphoreGive(xDisplayMut);
                        }
                    }
                    else if(sd_status==-1) {
                        printf("\nErro ao montar o cartão SD...\n");

                        if ((xSemaphoreTake(xDisplayMut, portMAX_DELAY)==pdTRUE)) {
                            ssd1306_fill(&ssd, false);                                      // Limpa o display
                            ssd1306_rect(&ssd, 3, 3, 122, 60, true, false);                 // Desenha um retângulo
                            ssd1306_draw_string(&ssd, "ERRO!!!", 30, 20);                   // Escreve texto no display
                            ssd1306_draw_string(&ssd, "Tente", 30, 32);                  // Escreve texto no display
                            ssd1306_draw_string(&ssd, "novamente...", 25, 44);              // Escreve texto no display
                            ssd1306_send_data(&ssd);                                        // Atualiza o display
                            
                            for(int i = 0; i < 10; i++) {
                                leds((i+1)%2,0,0);

                                vTaskDelay(pdMS_TO_TICKS(200));
                            }

                            xSemaphoreGive(xDisplayMut);
                        }
                        
                    } 
                } else if(option==1) {
                    FRESULT res = f_open(&file, filename, FA_WRITE | FA_OPEN_ALWAYS);

                    if ((xSemaphoreTake(xDisplayMut, portMAX_DELAY)==pdTRUE)) {
                        ssd1306_fill(&ssd, false);                                      // Limpa o display
                        ssd1306_rect(&ssd, 3, 3, 122, 60, true, false);                 // Desenha um retângulo
                        ssd1306_draw_string(&ssd, "Acessando", 30, 20);                   // Escreve texto no display
                        ssd1306_draw_string(&ssd, "o cartao...", 30, 32);                  // Escreve texto no display
                        ssd1306_send_data(&ssd);                                        // Atualiza o display
                        
                        leds(1,1,0);

                        vTaskDelay(pdMS_TO_TICKS(1000));

                        if(sd_status<1 || res != FR_OK) {
                            printf("Erro! Não foi possível acessar o arquivo!\n");
                            ssd1306_fill(&ssd, false);                                      // Limpa o display
                            ssd1306_rect(&ssd, 3, 3, 122, 60, true, false);                 // Desenha um retângulo
                            ssd1306_draw_string(&ssd, "ERRO!!!", 30, 20);                   // Escreve texto no display
                            ssd1306_draw_string(&ssd, "Monte o", 30, 32);                  // Escreve texto no display
                            ssd1306_draw_string(&ssd, "cartao SD!", 26, 44);              // Escreve texto no display
                            ssd1306_send_data(&ssd);                                        // Atualiza o display
                            
                            for(int i = 0; i < 10; i++) {
                                leds((i+1)%2,0,0);

                                vTaskDelay(pdMS_TO_TICKS(200));
                            }

                            xSemaphoreGive(xDisplayMut);
                        } else {    
                            printf("Gravando os dados...\n");

                            // Verifica se o arquivo está vazio
                            if (f_size(&file) == 0) {
                                const char *header = "numero_amostra,accel_x,accel_y,accel_z,giro_x,giro_y,giro_z";
                                UINT bw;
                                f_write(&file, header, strlen(header), &bw);
                            }

                            f_lseek(&file, f_size(&file));   

                            sd_status = 2;

                            xSemaphoreGive(xDisplayMut);

                            leds(0,0,1);

                            jingle(true);

                        }
                    }
                } else if(option==2) {
                    FRESULT res = f_open(&file, filename, FA_WRITE | FA_CREATE_ALWAYS);

                    if ((xSemaphoreTake(xDisplayMut, portMAX_DELAY)==pdTRUE)) {
                        printf("Limpando os dados...\n");
                        ssd1306_fill(&ssd, false);                                      // Limpa o display
                        ssd1306_rect(&ssd, 3, 3, 122, 60, true, false);                 // Desenha um retângulo
                        ssd1306_draw_string(&ssd, "Limpando", 20, 20);               // Escreve texto no display
                        ssd1306_draw_string(&ssd, "os dados...", 25, 32);               // Escreve texto no display
                        ssd1306_send_data(&ssd);                                       // Atualiza o display

                        leds(1,1,0);

                        vTaskDelay(pdMS_TO_TICKS(2000));

                        if (res == FR_OK) {
                            printf("Dados limpos com sucesso!\n");
                            samples = 0;

                            ssd1306_fill(&ssd, false);
                            ssd1306_rect(&ssd, 3, 3, 122, 60, true, false);
                            ssd1306_draw_string(&ssd, "Dados", 30, 20);
                            ssd1306_draw_string(&ssd, "limpos com", 25, 32);
                            ssd1306_draw_string(&ssd, "sucesso!", 30, 44);
                            ssd1306_send_data(&ssd);
                            leds(0,1,0);

                            vTaskDelay(pdMS_TO_TICKS(2000));
                        } else {
                            printf("Erro! Não foi possível acessar o arquivo\n");
                            ssd1306_fill(&ssd, false);
                            ssd1306_rect(&ssd, 3, 3, 122, 60, true, false);
                            ssd1306_draw_string(&ssd, "ERRO!!!", 30, 20);
                            ssd1306_draw_string(&ssd, "Nao foi", 35, 32);
                            ssd1306_draw_string(&ssd, "possivel limpar", 7, 44);
                            ssd1306_send_data(&ssd);

                            for(int i = 0; i < 10; i++) {
                                leds((i+1)%2,0,0);

                                vTaskDelay(pdMS_TO_TICKS(200));
                            }
                        }

                        xSemaphoreGive(xDisplayMut);
                    }

                    f_close(&file);
                } else if(option==3) {
                    printf("\nDesmontando o SD...\n");

                    if(sd_status<1) {
                        if ((xSemaphoreTake(xDisplayMut, portMAX_DELAY)==pdTRUE)) {
                            ssd1306_fill(&ssd, false);                                      // Limpa o display
                            ssd1306_rect(&ssd, 3, 3, 122, 60, true, false);                 // Desenha um retângulo
                            ssd1306_draw_string(&ssd, "ERRO!!!", 30, 20);                   // Escreve texto no display
                            ssd1306_draw_string(&ssd, "Monte o", 30, 32);                  // Escreve texto no display
                            ssd1306_draw_string(&ssd, "cartao SD!", 26, 44);              // Escreve texto no display
                            ssd1306_send_data(&ssd);                                        // Atualiza o display
                            
                            for(int i = 0; i < 10; i++) {
                                leds((i+1)%2,0,0);

                                vTaskDelay(pdMS_TO_TICKS(200));
                            }

                            xSemaphoreGive(xDisplayMut);
                        }
                    }
                    else {
                        sd_status = run_unmount(); // Montando cartão SD

                        if ((xSemaphoreTake(xDisplayMut, portMAX_DELAY)==pdTRUE)) {
                            ssd1306_fill(&ssd, false);                              // Limpa o display
                            ssd1306_rect(&ssd, 3, 3, 122, 60, true, false);         // Desenha um retângulo
                            ssd1306_draw_string(&ssd, "Desmontando", 20, 20);          // Escreve texto no display
                            ssd1306_draw_string(&ssd, "Cartao SD...", 20, 32);      // Escreve texto no display
                            ssd1306_send_data(&ssd);                                // Atualiza o display

                            leds(1,1,0);

                            vTaskDelay(pdMS_TO_TICKS(2000));
                        
                            xSemaphoreGive(xDisplayMut);
                        }

                        if(sd_status==0) {
                            if ((xSemaphoreTake(xDisplayMut, portMAX_DELAY)==pdTRUE)) {
                                gpio_put(RED, false);

                                ssd1306_fill(&ssd, false);                              // Limpa o display
                                ssd1306_rect(&ssd, 3, 3, 122, 60, true, false);         // Desenha um retângulo
                                ssd1306_draw_string(&ssd, "Cartao SD", 30, 16);         // Escreve texto no display
                                ssd1306_draw_string(&ssd, "Desmontado com", 10, 28);       // Escreve texto no display
                                ssd1306_draw_string(&ssd, "sucesso!!!", 20, 40);        // Escreve texto no display
                                ssd1306_send_data(&ssd);                                // Atualiza o display

                                vTaskDelay(pdMS_TO_TICKS(2000));

                                leds(0,0,0);

                                xSemaphoreGive(xDisplayMut);
                            }
                        }
                        else if(sd_status == -1) {
                            gpio_put(GREEN, false);
                            
                            printf("\nErro ao desmontar o cartão SD...\n");

                            if ((xSemaphoreTake(xDisplayMut, portMAX_DELAY)==pdTRUE)) {
                                ssd1306_fill(&ssd, false);                                      // Limpa o display
                                ssd1306_rect(&ssd, 3, 3, 122, 60, true, false);                 // Desenha um retângulo
                                ssd1306_draw_string(&ssd, "ERRO!!!", 30, 20);                   // Escreve texto no display
                                ssd1306_draw_string(&ssd, "Tente", 30, 32);                  // Escreve texto no display
                                ssd1306_draw_string(&ssd, "novamente...", 25, 44);              // Escreve texto no display
                                ssd1306_send_data(&ssd);                                        // Atualiza o display
                                
                                for(int i = 0; i < 10; i++) {
                                    gpio_put(RED, (i+1)%2);

                                    vTaskDelay(pdMS_TO_TICKS(200));
                                }

                                xSemaphoreGive(xDisplayMut);
                            }
                            
                        } 
                    }
                } else if(option==4) {
                    FRESULT res = f_open(&file, filename, FA_READ);

                    if (res != FR_OK)
                    {
                        printf("Não foi possível abrir o arquivo para leitura. Verifique se o Cartão está montado ou se o arquivo existe.\n");

                        if ((xSemaphoreTake(xDisplayMut, portMAX_DELAY)==pdTRUE)) {
                            ssd1306_fill(&ssd, false);                                      // Limpa o display
                            ssd1306_rect(&ssd, 3, 3, 122, 60, true, false);                 // Desenha um retângulo
                            ssd1306_draw_string(&ssd, "ERRO!!!", 30, 20);                   // Escreve texto no display
                            ssd1306_draw_string(&ssd, "Monte o", 30, 32);                  // Escreve texto no display
                            ssd1306_draw_string(&ssd, "cartao SD!", 26, 44);              // Escreve texto no display
                            ssd1306_send_data(&ssd);                                        // Atualiza o display
                            
                            for(int i = 0; i < 10; i++) {
                                leds((i+1)%2,0,0);

                                vTaskDelay(pdMS_TO_TICKS(200));
                            }

                            xSemaphoreGive(xDisplayMut);
                        }
                    } else {
                        if ((xSemaphoreTake(xDisplayMut, portMAX_DELAY)==pdTRUE)) {
                            ssd1306_fill(&ssd, false);                              // Limpa o display
                            ssd1306_rect(&ssd, 3, 3, 122, 60, true, false);         // Desenha um retângulo
                            ssd1306_draw_string(&ssd, "Imprimindo", 20, 20);          // Escreve texto no display
                            ssd1306_draw_string(&ssd, "no monitor", 20, 32);      // Escreve texto no display
                            ssd1306_draw_string(&ssd, "serial...", 20, 44);      // Escreve texto no display
                            ssd1306_send_data(&ssd);                                // Atualiza o display

                            leds(0,0,1);

                            char buffer[128];
                            UINT br;
                            printf("\033[2J\033[H\n"); // Limpa tela
                            stdio_flush();
                            while (f_read(&file, buffer, sizeof(buffer) - 1, &br) == FR_OK && br > 0)
                            {
                                buffer[br] = '\0';
                                printf("%s", buffer);
                            }
                            f_close(&file);
                            printf("\n\nLeitura do arquivo %s concluída.\n\n", filename);

                            ssd1306_fill(&ssd, false);                              // Limpa o display
                            ssd1306_rect(&ssd, 3, 3, 122, 60, true, false);         // Desenha um retângulo
                            ssd1306_draw_string(&ssd, "Impressao", 20, 20);          // Escreve texto no display
                            ssd1306_draw_string(&ssd, "concluida!!", 20, 32);      // Escreve texto no display
                            ssd1306_send_data(&ssd);                                // Atualiza o display

                            vTaskDelay(pdMS_TO_TICKS(1000));

                            leds(0,1,0);
                        
                            xSemaphoreGive(xDisplayMut);
                        }
                    }
                }
            }
        }
        vTaskDelay(pdMS_TO_TICKS(200));
    }
}