/**
 * @file reflow_oven_thermometer.cpp
 *
 */
/*********************
 *      INCLUDES
 *********************/
#include "reflow_oven_thermometer.h"
#include "reflow_oven_pid_control.h"
#include "reflow_oven_ui.h"
#include "reflow_oven_settings.h"
#include <lvgl_main.h>
#include <Adafruit_MAX31865.h>
/*********************
 *      DEFINES
 *********************/
#define MAXCS   5

// The value of the Rref resistor. Use 430.0 for PT100 and 4300.0 for PT1000
#define RREF      430.0
// The 'nominal' 0-degrees-C resistance of the sensor
// 100.0 for PT100, 1000.0 for PT1000
#define RNOMINAL  100.0

 /**********************
 *  STATIC PROTOTYPES
 **********************/
static Adafruit_MAX31865 thermometer(MAXCS);

/**********************
 *   GLOBAL FUNCTIONS
 **********************/
void thermometer_setup(){
	if (!thermometer.begin(MAX31865_2WIRE)) {
		Serial.println("Thermometer failed");
	}
	thermometer.enable50Hz(true);
}

float thermometer_read(){
	return thermometer.temperature(RNOMINAL, RREF);
}

uint8_t thermometer_faultcheck(){
  uint8_t fault = thermometer.readFault();
  if (fault) {
    Serial.print("Fault 0x"); Serial.println(fault, HEX);
    if (fault & MAX31865_FAULT_HIGHTHRESH) {
      Serial.println("RTD High Threshold"); 
    }
    if (fault & MAX31865_FAULT_LOWTHRESH) {
      Serial.println("RTD Low Threshold"); 
    }
    if (fault & MAX31865_FAULT_REFINLOW) {
      Serial.println("REFIN- > 0.85 x Bias"); 
    }
    if (fault & MAX31865_FAULT_REFINHIGH) {
      Serial.println("REFIN- < 0.85 x Bias - FORCE- open"); 
    }
    if (fault & MAX31865_FAULT_RTDINLOW) {
      Serial.println("RTDIN- < 0.85 x Bias - FORCE- open"); 
    }
    if (fault & MAX31865_FAULT_OVUV) {
      Serial.println("Under/Over voltage"); 
    }
    thermometer.clearFault();
  }
	return fault;
}