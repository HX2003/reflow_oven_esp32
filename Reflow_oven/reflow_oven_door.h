/**
 * @file reflow_oven_door.h
 *
 */

#ifndef REFLOW_OVEN_DOOR_H
#define REFLOW_OVEN_DOOR_H

#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/
typedef struct {
	bool state;
	bool fromMenu;
}reflow_oven_door_queue_type_t;
/**********************
 * GLOBAL PROTOTYPES
 **********************/
void door_setup();
void door_open();
void door_close_stage_1();
void door_close_stage_2();
void door_set_moving_state(bool val);
bool door_get_moving_state();
void door_set_sw_state(bool val);
bool door_get_hw_state();
bool door_get_sw_state();

/**********************
 *      MACROS
 **********************/

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /*REFLOW_OVEN_DOOR_H*/
