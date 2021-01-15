/**
 * @file reflow_oven_door.cpp
 *
 */
/*********************
 *      INCLUDES
 *********************/
#include <Arduino.h>
#include "reflow_oven_door.h"
#include "reflow_oven_ui.h"
#include "reflow_oven_settings.h"
#include <lvgl_main.h>
#include <lx16a-servo.h>
 
/*********************
 *      DEFINES
 *********************/
 
 /**********************
 *  STATIC PROTOTYPES
 **********************/
static bool door_state = false; //false for closed`
static bool door_moving = false;
static bool door_queue = false;
static LX16ABus servoBus;
static LX16AServo servo(servoBus, 1);
/**********************
 *   GLOBAL FUNCTIONS
 **********************/
void door_setup(){ 
	servoBus.begin(Serial1, 17);
	door_get_hw_state();
}
 
void door_open(){
	servo.move_time(14000, 3000);
}

void door_close_stage_1(){
	servo.move_time(22000, 2000);
}

void door_close_stage_2(){
	servo.move_time(23000, 1000);
}

void door_set_moving_state(bool val){
	door_moving = val;
}

bool door_get_moving_state(){
	return door_moving;
}

void door_set_sw_state(bool val){
	door_state = val;
}

bool door_get_hw_state(){
	int16_t angle;
	servo.pos_read(angle);
	if(angle<17500){
		door_state = true;
	}else{
		door_state = false;
	}
	return door_state;
}

bool door_get_sw_state(){
	return door_state;
}
