/**
 * @file reflow_oven_display_brightness.h
 *
 */

#ifndef REFLOW_OVEN_DISPLAY_BRIGHTNESS_H
#define REFLOW_OVEN_DISPLAY_BRIGHTNESS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdlib.h>

void display_brightness_setup();

void display_brightness_set(uint8_t brightness_val);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /*REFLOW_OVEN_DISPLAY_BRIGHTNESS_H*/
