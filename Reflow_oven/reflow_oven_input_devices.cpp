 /**
 * @file reflow_oven_ui.c
 *
 */
/*********************
 *      INCLUDES
 *********************/
#include "reflow_oven_input_devices.h"
#include <lvgl_main.h>
#include <ESP32Encoder.h>
#include <EasyButton.h>
/*********************
 *      DEFINES
 *********************/
#define ENCODER_A_PIN 35
#define ENCODER_B_PIN 34
#define ENCODER_SW_PIN 32
 
/*
int time = 0;
int counter = 0;
 */
 /**********************
 *  STATIC PROTOTYPES
 **********************/
static ESP32Encoder encoder;
static EasyButton encoder_button(ENCODER_SW_PIN);
/**********************
 *  STATIC VARIABLES
 **********************/
static int previousCount = 0;
static bool buttonPressed = false;
/**********************
 *   GLOBAL FUNCTIONS
 **********************/
void buttonISR()
{
  encoder_button.read();
}
void drv_encoder_setup(){
	ESP32Encoder::useInternalWeakPullResistors=UP;
	
	// Attache pins for use as encoder pins
	encoder.attachHalfQuad(ENCODER_A_PIN, ENCODER_B_PIN);
	encoder.setCount(0);
	
	encoder_button.begin();
	
	if (encoder_button.supportsInterrupt())
    {
    encoder_button.enableInterrupt(buttonISR);
    }
}

bool drv_encoder_read(lv_indev_t *drv, lv_indev_data_t * data){
	/*
	if(time+1000<millis()){
	if(counter==1){
		data->state = LV_INDEV_STATE_PR;
	}else{
		data->state = LV_INDEV_STATE_REL; 
	}
    data->enc_diff = 1;
	time = millis();
	counter++;
	if(counter==4){
		counter=0;
	}
	}
	*/
	if(encoder_button.read()){
		data->state = LV_INDEV_STATE_PR;
	}else{
		data->state = LV_INDEV_STATE_REL; 
	}
	int newCount = encoder.getCount();
	int enc_diff = 0;
	if(newCount-previousCount<-1||newCount-previousCount>1){ //This encoder clicks in steps of 2
		enc_diff = (newCount-previousCount)/2;
		previousCount = newCount;
	}  
	  
	data->enc_diff = enc_diff;
	return 0;
}