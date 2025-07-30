#ifndef TASKS_H
#define TASKS_H

#include "globals.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

#define MAX_USERS 25

extern SemaphoreHandle_t xCounterSem;
extern SemaphoreHandle_t xDisplayMut;
extern SemaphoreHandle_t xActSem;
extern SemaphoreHandle_t xSensorMut;
extern SemaphoreHandle_t xSdMut;
extern UBaseType_t counter;
extern TaskHandle_t xSDTaskHandle;

void vTaskMonitoringScreen(void *params);
void vTaskSDControlPanel(void *params);
void vTaskSDAction(void *params);
void vTaskMPU6050(void *params);

#endif