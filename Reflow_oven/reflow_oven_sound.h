/**
 * @file reflow_oven_sound.h
 *
 */

#ifndef REFLOW_OVEN_SOUND_H
#define REFLOW_OVEN_SOUND_H

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
void drv_sound_setup();
void drv_sound_feedback(lv_indev_t *drv, lv_event_t e);
void sound_start_reflow();
void sound_toggle_door();

/**********************
 *      MACROS
 **********************/

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /*REFLOW_OVEN_SOUND_H*/
