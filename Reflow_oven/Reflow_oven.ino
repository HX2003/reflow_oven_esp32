  /* LVGL Example project

   Basic project to test LVGL on ESP32 based projects.

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_freertos_hooks.h"
#include "freertos/semphr.h"
#include "esp_system.h"
#include "reflow_oven_tasks.h"
#include "reflow_oven_ui.h"
#include "reflow_oven_settings.h"
#include "reflow_oven_display_brightness.h"
#include "reflow_oven_pid_control.h"
#include "reflow_oven_thermometer.h"
#include "reflow_oven_door.h"
#include "reflow_oven_ssr.h"
#include "reflow_oven_profiles.h"

/* Littlevgl specific */
#include <lvgl_main.h>

/*********************
        DEFINES
 *********************/

/**********************
    STATIC PROTOTYPES
 **********************/
static void IRAM_ATTR lv_tick_task(void *arg);
void guiTask();

/**********************
     APPLICATION MAIN
 **********************/
void setup() {
  Serial.begin(115200);

  lv_init();

  lvgl_driver_init();

  Serial.println("Max memory block:" + String(heap_caps_get_largest_free_block(MALLOC_CAP_DEFAULT)));

  lv_color_t* buf1;
  buf1 = (lv_color_t*)malloc(DISP_BUF_SIZE * sizeof(lv_color_t));
  //It seems that one buffer with twice the size, is better than double buffer
  static lv_disp_buf_t disp_buf;
  lv_disp_buf_init(&disp_buf, buf1, NULL, DISP_BUF_SIZE);

  lv_disp_drv_t disp_drv;
  lv_disp_drv_init(&disp_drv);
  disp_drv.flush_cb = disp_driver_flush;

  disp_drv.buffer = &disp_buf;
  lv_disp_drv_register(&disp_drv);

  esp_timer_create_args_t periodic_timer_args;
  periodic_timer_args.callback = &lv_tick_task;
  periodic_timer_args.name = "periodic_gui";

  esp_timer_handle_t periodic_timer;
  ESP_ERROR_CHECK(esp_timer_create(&periodic_timer_args, &periodic_timer));
  //On ESP32 it's better to create a periodic task instead of esp_register_freertos_tick_hook
  ESP_ERROR_CHECK(esp_timer_start_periodic(periodic_timer, 1 * 1000)); //1ms (expressed as microseconds)

  lv_task_handler();

  //Setup settings
  settings_setup();
  settings_load();

  //Setup others
  display_brightness_setup();
  ssr_setup();
  thermometer_setup();
  pid_control_setup();
  door_setup();

  //Setup queues

  door_queue = xQueueCreate( 2, sizeof(reflow_oven_door_queue_type_t) );
  pid_queue = xQueueCreate( 2, sizeof(bool) );
  temp_queue = xQueueCreate( 2, sizeof(float) );

  xTaskCreatePinnedToCore(guiTask, "gui", 4096, NULL, 1, NULL, 1);
  xTaskCreatePinnedToCore(temperatureTask, "temperature", 2048, NULL, 1, NULL, 0);
  xTaskCreatePinnedToCore(pidTask, "pid", 4096, NULL, 1, NULL, 0);
  xTaskCreatePinnedToCore(doorTask, "door", 2048, NULL, 1, NULL, 0);
}

void loop() {
  vTaskDelay(5000);
  //Serial.println(ESP.getFreeHeap());
}
static void IRAM_ATTR lv_tick_task(void *arg) {
  (void) arg;

  lv_tick_inc(portTICK_RATE_MS); //Take note it is 1ms
}

//Creates a semaphore to handle concurrent call to lvgl stuff
//If you wish to call *any* lvgl function from other threads/tasks
//you should lock on the very same semaphore!
SemaphoreHandle_t xGuiSemaphore;

void guiTask(void *pvParameters) {
  xGuiSemaphore = xSemaphoreCreateMutex();
  //Build UI
  reflow_oven_ui();

  lv_task_handler();

  vTaskDelay(50 / portTICK_PERIOD_MS);

  display_brightness_set(reflow_oven_setting.display_brightness);

  uint32_t lv_delay = 1;
  while (1) {
    if (xSemaphoreTake(xGuiSemaphore, portMAX_DELAY ) == pdTRUE) {
      lv_delay = lv_task_handler();
      xSemaphoreGive(xGuiSemaphore);
    }
    vTaskDelay(lv_delay);
  }
  //A task should NEVER return
  vTaskDelete(NULL);
}

void temperatureTask(void *pvParameters) {
  while (1) {
    float temp = thermometer_read();
    char* temp_buf = (char*)malloc(8 * sizeof(char));

    sprintf(temp_buf, "%.0f", temp);

    xQueueSend(temp_queue, &temp, portMAX_DELAY);

    while (xSemaphoreTake(xGuiSemaphore, (TickType_t)500) != pdTRUE) {
      vTaskDelay(10);
    }

    reflow_oven_update_temperature(temp_buf);
    free(temp_buf);

    uint8_t fault = thermometer_faultcheck();
    if (fault || temp < -40.0) {
      reflow_oven_error_popup("Thermometer error", 0);
    }

    xSemaphoreGive(xGuiSemaphore);

    vTaskDelay(200);
  }
  vTaskDelete(NULL);
}

void doorTask(void *pvParameters) {
  while (1) {
    reflow_oven_door_queue_type_t door_state;
    xQueueReceive(door_queue, &door_state, portMAX_DELAY);
    if (door_state.state) { //If true (opened), then close
      door_set_moving_state(true);
      door_close_stage_1();
      vTaskDelay(2000 / portTICK_PERIOD_MS);
      door_close_stage_2();
      vTaskDelay(1000 / portTICK_PERIOD_MS);
      door_set_moving_state(false);
      door_set_sw_state(!door_state.state);
    } else {
      door_set_moving_state(true);
      door_open();
      vTaskDelay(3000 / portTICK_PERIOD_MS);
      door_set_moving_state(false);
      door_set_sw_state(!door_state.state);
    }

    while (xSemaphoreTake(xGuiSemaphore, (TickType_t)500) != pdTRUE) {
      vTaskDelay(10);
    }

    door_state.state ? display_door_status(REFLOW_OVEN_DOOR_STATUS_LOCKED) : display_door_status(REFLOW_OVEN_DOOR_STATUS_UNLOCKED);
    if(door_state.fromMenu){
      reflow_oven_focus_door_icon();
    }

    xSemaphoreGive(xGuiSemaphore);
  }
  vTaskDelete(NULL);
}

void pidTask(void *pvParameters) {
  uint32_t graphStartTime;
  while (1) {
    reflow_oven_pid_status_type_t pid_state;
    float pid_temp;
    if (xQueueReceive(pid_queue, &pid_state, 0) == pdTRUE) {
      if (pid_state == REFLOW_OVEN_PID_STATUS_PRE_PRESOAK || pid_state == REFLOW_OVEN_PID_STATUS_ACTIVE) {
        display_heater_status(REFLOW_OVEN_HEATER_STATUS_INACTIVE);
      } else {
        display_heater_status(REFLOW_OVEN_HEATER_STATUS_DISABLED);
      }
      pid_set_state(pid_state);
      if (pid_state == REFLOW_OVEN_PID_STATUS_PRE_PRESOAK) {
        Serial.println("setting up pid");
        pid_control_setup();
      } else if (pid_state == REFLOW_OVEN_PID_STATUS_DISABLED) {
        Serial.println("turning off pid");
        pid_control_stop();
      }
    }

    if (xQueueReceive(temp_queue, &pid_temp, 0) == pdTRUE) {
      if (pid_get_state() == REFLOW_OVEN_PID_STATUS_PRE_PRESOAK && pid_temp > reflow_profiles[reflow_oven_setting.current_reflow_profile]->presoak_start_temp) {
        graphStartTime = millis();
      }
      if (pid_get_state() == REFLOW_OVEN_PID_STATUS_PRE_PRESOAK || pid_get_state() == REFLOW_OVEN_PID_STATUS_ACTIVE) {
        pid_update(pid_temp);
      }

      if (pid_get_state() == REFLOW_OVEN_PID_STATUS_ACTIVE) {
        if (millis() - graphStartTime >= 1000)
        {
          graphStartTime += 1000;
          while (xSemaphoreTake(xGuiSemaphore, (TickType_t)500) != pdTRUE) {
            vTaskDelay(10);
          }
          pid_update_chart();
          xSemaphoreGive(xGuiSemaphore);
        }
      }

       if (pid_get_state() == REFLOW_OVEN_PID_STATUS_COMPLETED) {
          while (xSemaphoreTake(xGuiSemaphore, (TickType_t)500) != pdTRUE) {
            vTaskDelay(10);
          }
          reflow_complete();
          xSemaphoreGive(xGuiSemaphore);
          pid_set_state(REFLOW_OVEN_PID_STATUS_DISABLED);
      }
    }

    vTaskDelay(100 / portTICK_PERIOD_MS);
  }
  vTaskDelete(NULL);
}
