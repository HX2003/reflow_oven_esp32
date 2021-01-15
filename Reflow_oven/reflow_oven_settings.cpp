/**
 * @file reflow_oven_settings.cpp
 *
 */
/*********************
 *      INCLUDES
 *********************/
#include "reflow_oven_settings.h"
#include <Preferences.h>

/*********************
 *      DEFINES
 *********************/
reflow_oven_settings_t reflow_oven_setting = {
    .major_version = REFLOW_OVEN_FIRMWARE_MAJOR_VERSION,
    .minor_version = REFLOW_OVEN_FIRMWARE_MINOR_VERSION,
    .patch_version = REFLOW_OVEN_FIRMWARE_PATCH_VERSION,
    .display_brightness = 16,
    .speaker_volume = 16,
    .pid_p_presoak = 100,
    .pid_i_presoak = 0.01,
    .pid_d_presoak = 50,
    .pid_p_soak = 250,
    .pid_i_soak = 0.05,
    .pid_d_soak = 300,
    .pid_p_reflow = 300,
    .pid_i_reflow = 0.05,
    .pid_d_reflow = 350,
    .current_reflow_profile = 1
};

Preferences prefs;

bool prefsSuccess = false; 

void settings_setup(){
    prefsSuccess = prefs.begin("oven_settings");
}
void settings_load(){
    if(prefsSuccess){
    size_t prefLen = prefs.getBytesLength("oven_settings");
    
    if(prefLen == sizeof(reflow_oven_settings_t)){
      reflow_oven_settings_t *temp_reflow_oven_setting = (reflow_oven_settings_t*)malloc(sizeof(reflow_oven_settings_t));
      prefs.getBytes("oven_settings", temp_reflow_oven_setting, prefLen);
	  Serial.println(String(temp_reflow_oven_setting->display_brightness));
      if(temp_reflow_oven_setting->major_version==REFLOW_OVEN_FIRMWARE_MAJOR_VERSION&&temp_reflow_oven_setting->minor_version==REFLOW_OVEN_FIRMWARE_MINOR_VERSION){
        Serial.println(F("Preferences successfully loaded"));
        reflow_oven_setting = *temp_reflow_oven_setting;
      }else{
        settings_save();
      }
      free(temp_reflow_oven_setting);
	  return;
    }
  }
  settings_save();
}
void settings_save(){
	Serial.println(F("Saving settings"));
    Serial.println(String(prefs.putBytes("oven_settings", &reflow_oven_setting, sizeof(reflow_oven_settings_t))));
}
void settings_reset(){
	prefs.remove("oven_settings");
	ESP.restart();
}