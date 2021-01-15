/**
 * @file reflow_oven_pid_control.h
 *
 */

#ifndef REFLOW_OVEN_PID_CONTROL_H
#define REFLOW_OVEN_PID_CONTROL_H

#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/
#include <lvgl_main.h>
/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/
typedef enum {
    REFLOW_OVEN_PID_STATUS_PRE_PRESOAK,
    REFLOW_OVEN_PID_STATUS_ACTIVE,
    REFLOW_OVEN_PID_STATUS_DISABLED,
	REFLOW_OVEN_PID_STATUS_COMPLETED
}reflow_oven_pid_status_type_t;

/**********************
 * GLOBAL PROTOTYPES
 **********************/
extern lv_coord_t *temp_data;
extern uint16_t max_points;

void pid_control_setup();
void pid_control_stop();
void pid_update(float temp);
void pid_setpoint_update();
void pid_set_state(reflow_oven_pid_status_type_t val);
reflow_oven_pid_status_type_t pid_get_state();
void temp_log_setup(uint16_t max_points_param);
lv_coord_t * temp_log_get();
void temp_log_free();
void pid_update_chart();
/**********************
 *      MACROS
 **********************/

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /*REFLOW_OVEN_PID_CONTROL_H*/
