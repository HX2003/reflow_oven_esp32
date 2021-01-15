/**
 * @file reflow_oven_tasks.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
#include "reflow_oven_tasks.h"

QueueHandle_t door_queue;
QueueHandle_t pid_queue;
QueueHandle_t temp_queue;