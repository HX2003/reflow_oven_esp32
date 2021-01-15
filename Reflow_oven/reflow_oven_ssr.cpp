/**
 * @file reflow_oven_ssr.cpp
 *
 */
/*********************
 *      INCLUDES
 *********************/
#include <Arduino.h>
#include "reflow_oven_ssr.h"
/*********************
 *      DEFINES
 *********************/
 
void ssr_setup(){
	pinMode(16, OUTPUT);
	digitalWrite(16, LOW);
}

void ssr_set(bool val){
	digitalWrite(16, val);
}