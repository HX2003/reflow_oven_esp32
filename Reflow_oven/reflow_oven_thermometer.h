/**
 * @file reflow_oven_thermometer.h
 *
 */

#ifndef REFLOW_OVEN_THERMOMETER_H
#define REFLOW_OVEN_THERMOMETER_H

#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/
#include <lvgl_main.h>
/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 * GLOBAL PROTOTYPES
 **********************/

void thermometer_setup();
float thermometer_read();
uint8_t thermometer_faultcheck();
/**********************
 *      MACROS
 **********************/

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /*REFLOW_OVEN_THERMOMETER_H*/
