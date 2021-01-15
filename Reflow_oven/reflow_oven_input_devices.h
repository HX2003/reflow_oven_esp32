/**
 * @file reflow_oven_input_devices.h
 *
 */

#ifndef REFLOW_OVEN_INPUT_DEVICES_H
#define REFLOW_OVEN_INPUT_DEVICES_H

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
void drv_encoder_setup();
bool drv_encoder_read(lv_indev_t *drv, lv_indev_data_t * data);

/**********************
 *      MACROS
 **********************/

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /*REFLOW_OVEN_INPUT_DEVICES_H*/
