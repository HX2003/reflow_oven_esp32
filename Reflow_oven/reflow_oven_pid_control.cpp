/**
 * @file reflow_oven_pid_control.cpp
 *
 */
/*********************
 *      INCLUDES
 *********************/
#include "reflow_oven_pid_control.h"
#include "reflow_oven_utils.h"
#include "reflow_oven_tasks.h"
#include "reflow_oven_ui.h"
#include "reflow_oven_profiles.h"
#include "reflow_oven_settings.h"
#include "reflow_oven_door.h"
#include "reflow_oven_thermometer.h"
#include "reflow_oven_ssr.h"
#include <lvgl_main.h>
#include <PID_Plus.h>
#include <Arduino.h>
/*********************
 *      DEFINES
 *********************/
 
 /*********************
 *       
 *********************/
 
lv_coord_t *temp_data;
reflow_oven_pid_status_type_t pid_state = REFLOW_OVEN_PID_STATUS_DISABLED;
uint16_t max_points;
double Setpoint, Input, Output;

PID myPID(&Setpoint, &Input, &Output);

uint16_t pidWindowSize = 2000;
float outputWindowRel = pidWindowSize/100.0;
uint32_t reflowStartTime;
uint32_t windowStartTime;

uint16_t reflow_profile_horizontal_max_dur;
uint16_t reflow_profile_presoak_start_time;
uint16_t reflow_profile_soak_start_time;
uint16_t reflow_profile_reflow_begin_start_time;
uint16_t reflow_profile_reflow_hold_start_time;
uint16_t reflow_profile_cooldown_start_time;
    
/**********************
 *   GLOBAL FUNCTIONS
 **********************/
void pid_control_setup(){
	Setpoint = reflow_profiles[reflow_oven_setting.current_reflow_profile]->presoak_start_temp;
	myPID.setSampleTime(pidWindowSize);
	myPID.setOutputLimits(0, 100.0);
	myPID.setProportional(reflow_oven_setting.pid_p_presoak);
	myPID.setIntegral(reflow_oven_setting.pid_i_presoak);
	myPID.setDerivative(reflow_oven_setting.pid_d_presoak);
	//myPID.setFeedForward(Kf);
	Serial.println("outputWindowRel: " + String(outputWindowRel));
	Serial.println("PID Parameters: P, " + String(reflow_oven_setting.pid_p_presoak) + ". I, " + String(reflow_oven_setting.pid_i_presoak) + ". D, " + String(reflow_oven_setting.pid_d_presoak));
	myPID.enableController();
	reflow_profile_horizontal_max_dur = reflow_profiles[reflow_oven_setting.current_reflow_profile]->presoak_dur +
                                                reflow_profiles[reflow_oven_setting.current_reflow_profile]->soak_dur +
                                                reflow_profiles[reflow_oven_setting.current_reflow_profile]->reflow_begin_dur +
                                                reflow_profiles[reflow_oven_setting.current_reflow_profile]->reflow_hold_dur +
                                                reflow_profiles[reflow_oven_setting.current_reflow_profile]->cooldown_dur;
												
    reflow_profile_presoak_start_time = 0;
    reflow_profile_soak_start_time = reflow_profiles[reflow_oven_setting.current_reflow_profile]->presoak_dur;
    reflow_profile_reflow_begin_start_time = reflow_profile_soak_start_time + reflow_profiles[reflow_oven_setting.current_reflow_profile]->soak_dur;
    reflow_profile_reflow_hold_start_time = reflow_profile_reflow_begin_start_time + reflow_profiles[reflow_oven_setting.current_reflow_profile]->reflow_begin_dur;
    reflow_profile_cooldown_start_time = reflow_profile_reflow_hold_start_time + reflow_profiles[reflow_oven_setting.current_reflow_profile]->reflow_hold_dur;
}

void pid_reflow_start(){
	reflowStartTime = millis();
	windowStartTime = millis();
}
void pid_control_stop(){
	Setpoint = 30.0;
	myPID.disableController();
	ssr_set(false);
}

void pid_update(float temp){
	Input = (double)temp;
	
	if (millis() - windowStartTime >= pidWindowSize)
    { //time to shift the window
      windowStartTime += pidWindowSize;
	  myPID.compute();
    }
	int timey = Output*1.0*outputWindowRel;
	Serial.println("Input:" + String(Input));
	Serial.println("SP:" + String(Setpoint));
	Serial.println("Raw Output:" + String(Output));
	if (Output*outputWindowRel > millis() - windowStartTime){
		ssr_set(true);
	} else {
		ssr_set(false);
	}
	
	if(pid_state==REFLOW_OVEN_PID_STATUS_PRE_PRESOAK && temp > reflow_profiles[reflow_oven_setting.current_reflow_profile]->presoak_start_temp){
		pid_reflow_start();
		pid_state = REFLOW_OVEN_PID_STATUS_ACTIVE;
	}
	
	if(pid_state==REFLOW_OVEN_PID_STATUS_ACTIVE){
		pid_setpoint_update();
	}
}

void pid_setpoint_update(){
	uint32_t reflowElapsedTime = (millis() -  reflowStartTime)/1000;
	int16_t y_temp = 0;
    if(reflowElapsedTime<reflow_profile_soak_start_time){
        y_temp = linear_interpolate(reflowElapsedTime, reflow_profiles[reflow_oven_setting.current_reflow_profile]->presoak_dur, reflow_profiles[reflow_oven_setting.current_reflow_profile]->presoak_start_temp, reflow_profiles[reflow_oven_setting.current_reflow_profile]->presoak_end_temp);
    }else if(reflowElapsedTime>=reflow_profile_soak_start_time&&reflowElapsedTime<reflow_profile_reflow_begin_start_time){
        y_temp = linear_interpolate(reflowElapsedTime - reflow_profile_soak_start_time, reflow_profiles[reflow_oven_setting.current_reflow_profile]->soak_dur, reflow_profiles[reflow_oven_setting.current_reflow_profile]->soak_start_temp, reflow_profiles[reflow_oven_setting.current_reflow_profile]->soak_end_temp);
		myPID.setProportional(reflow_oven_setting.pid_p_soak);
		myPID.setIntegral(reflow_oven_setting.pid_i_soak);
		myPID.setDerivative(reflow_oven_setting.pid_d_soak);
    }else if(reflowElapsedTime>=reflow_profile_reflow_begin_start_time&&reflowElapsedTime<reflow_profile_reflow_hold_start_time){
        y_temp = linear_interpolate(reflowElapsedTime - reflow_profile_reflow_begin_start_time, reflow_profiles[reflow_oven_setting.current_reflow_profile]->reflow_begin_dur, reflow_profiles[reflow_oven_setting.current_reflow_profile]->reflow_begin_start_temp, reflow_profiles[reflow_oven_setting.current_reflow_profile]->reflow_begin_end_temp);
		myPID.setProportional(reflow_oven_setting.pid_p_reflow);
		myPID.setIntegral(reflow_oven_setting.pid_i_reflow);
		myPID.setDerivative(reflow_oven_setting.pid_d_reflow);
    }else if(reflowElapsedTime>=reflow_profile_reflow_hold_start_time&&reflowElapsedTime<reflow_profile_cooldown_start_time){
        y_temp = linear_interpolate(reflowElapsedTime - reflow_profile_reflow_hold_start_time, reflow_profiles[reflow_oven_setting.current_reflow_profile]->reflow_hold_dur, reflow_profiles[reflow_oven_setting.current_reflow_profile]->reflow_hold_start_temp, reflow_profiles[reflow_oven_setting.current_reflow_profile]->reflow_hold_end_temp);
    }else if(reflowElapsedTime>=reflow_profile_cooldown_start_time&&reflowElapsedTime<reflow_profile_horizontal_max_dur){
        y_temp = linear_interpolate(reflowElapsedTime - reflow_profile_cooldown_start_time, reflow_profiles[reflow_oven_setting.current_reflow_profile]->cooldown_dur, reflow_profiles[reflow_oven_setting.current_reflow_profile]->cooldown_start_temp, reflow_profiles[reflow_oven_setting.current_reflow_profile]->cooldown_end_temp);
		//Open door once cooling process starts
		if(!door_get_sw_state() && !door_get_moving_state() && ( reflowElapsedTime>=reflow_profile_cooldown_start_time + 30)){
			reflow_oven_door_queue_type_t door_state = {false, false};
			xQueueSend(door_queue, &door_state, ( TickType_t ) 0);
		}
    }else {
		//Reflow completed
		pid_state = REFLOW_OVEN_PID_STATUS_COMPLETED;
		pid_control_stop();
	}
	Setpoint = (double)y_temp;
}
void pid_set_state(reflow_oven_pid_status_type_t val){
	pid_state = val;
}

reflow_oven_pid_status_type_t pid_get_state(){
	return pid_state;
}

void temp_log_setup(uint16_t max_points_param){
	max_points = max_points_param;
	temp_data = (lv_coord_t*)malloc(max_points*sizeof(lv_coord_t)); //Remember to free it later
}

lv_coord_t * temp_log_get(){
	return temp_data;
}

void temp_log_free(){
	free(temp_data);
}

void pid_update_chart(){
	uint32_t reflowElapsedTime = (millis() -  reflowStartTime)/1000;
	
	uint16_t index = reflowElapsedTime;
		
	if(index<max_points){
		temp_data[index] = (uint16_t)Input;
		Serial.println("index: " + String(index) + ", input: " + String(Input));
	}
	reflow_oven_update_chart();
}