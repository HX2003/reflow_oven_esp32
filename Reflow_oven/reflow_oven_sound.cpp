/**
 * @file reflow_oven_ui.c
 *
 */
/*********************
 *      INCLUDES
 *********************/
#include "reflow_oven_sound.h"
#include <lvgl_main.h>
#include <ESP32I2SAudio.h>
#include "sounds/soundsamples.h"
/*********************
 *      DEFINES
 *********************/
#define SAMPLING_RATE 48000 //Minimum tested 6000
#define BUFFER_COUNT 8
#define SAMPLE_BUFFER_SIZE 1024  //Maximum number of samples for i2s engine 1024, each sample is 2bytes
 /**********************
 *  STATIC PROTOTYPES
 **********************/
static AudioSystem audioSystem(SAMPLING_RATE, SAMPLE_BUFFER_SIZE, BUFFER_COUNT);
static AudioOutput audioOutput;
/**********************
 *   GLOBAL FUNCTIONS
 **********************/
void drv_sound_setup(){
	audioOutput.configPin(26, 25, 27);
	audioOutput.init(audioSystem);
}

void drv_sound_feedback(lv_indev_t *drv, lv_event_t e){
	if(e == LV_EVENT_FOCUSED) {
        soundsamples.play(audioSystem, 0, 1, 1, false);
    }
}
void sound_start_reflow(){
	soundsamples.play(audioSystem, 1, 1, 1, false);
	Serial.println("playing");
}
void sound_toggle_door(){
	soundsamples.play(audioSystem, 2, 1, 1, false);
}