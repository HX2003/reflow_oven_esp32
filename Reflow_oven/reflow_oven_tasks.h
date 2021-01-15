/**
 * @file reflow_oven_tasks.h
 *
 */

#ifndef REFLOW_OVEN_TASKS_H
#define REFLOW_OVEN_TASKS_H

#ifdef __cplusplus
extern "C" {
#endif

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_freertos_hooks.h"
#include "freertos/semphr.h"

extern QueueHandle_t door_queue;
extern QueueHandle_t pid_queue;
extern QueueHandle_t temp_queue;
#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /*REFLOW_OVEN_TASKS_H*/
