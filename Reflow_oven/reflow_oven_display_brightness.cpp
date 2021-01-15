/**
 * @file reflow_oven_display_brightness.cpp
 *
 */
/*********************
 *      INCLUDES
 *********************/
#include "reflow_oven_display_brightness.h"
#include "esp32-hal-ledc.h"
/*********************
 *      DEFINES
 *********************/
 
void display_brightness_setup(){
	ledcSetup(0, 5000, 4);
	ledcAttachPin(4, 0);
}

void display_brightness_set(uint8_t brightness_val){
	ledcWrite(0, brightness_val);
}