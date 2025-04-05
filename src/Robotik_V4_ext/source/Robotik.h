/*
*   Gerrit Hinrichs 06.2024
*   github.com/snech99
*
*   Clock_Robot
*   main header
*/
#ifndef ROBOTIK_H_
#define ROBOTIK_H_

#include "fsl_common.h"
#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MCXA153.h"
#include "fsl_debug_console.h"
#include "fsl_ctimer.h"
#include "TMC2209_UART.h"
#include <math.h>
#include "UART.h"
#include "kinematik.h"

#define DIR_STICK_GPIO INIT_DIR_STOP_PIN_DIR_STICK_GPIO
#define DIR_AXIS_GPIO INIT_DIR_STOP_PIN_DIR_AXIS_GPIO
#define DIR_TOWER_GPIO INIT_DIR_STOP_PIN_DIR_TOWER_GPIO
#define DIR_CLEAN_GPIO INIT_DIR_STOP_PIN_DIR_CLEAR_GPIO
#define ENDSTOP_STICK_GPIO INIT_DIR_STOP_PIN_ENDSTOP_STICK_GPIO
#define ENDSTOP_AXIS_GPIO INIT_DIR_STOP_PIN_ENDSTOP_AXIS_GPIO
#define ENDSTOP_TOWER_GPIO INIT_DIR_STOP_PIN_ENDSTOP_TOWER_GPIO
#define ENDSTOP_CLEAR_GPIO INIT_DIR_STOP_PIN_ENDSTOP_CLEAR_GPIO
#define HOMING_GPIO INIT_DIR_STOP_PIN_HOME_GPIO
#define SW1_GPIO INIT_DIR_STOP_PIN_SW1_GPIO
#define SW2_GPIO INIT_DIR_STOP_PIN_SW2_GPIO

#define DIR_STICK_PIN INIT_DIR_STOP_PIN_DIR_STICK_PIN
#define DIR_AXIS_PIN INIT_DIR_STOP_PIN_DIR_AXIS_PIN
#define DIR_TOWER_PIN INIT_DIR_STOP_PIN_DIR_TOWER_PIN
#define DIR_CLEAN_PIN INIT_DIR_STOP_PIN_DIR_CLEAR_PIN
#define ENDSTOP_STICK_PIN INIT_DIR_STOP_PIN_ENDSTOP_STICK_PIN
#define ENDSTOP_AXIS_PIN INIT_DIR_STOP_PIN_ENDSTOP_AXIS_PIN
#define ENDSTOP_TOWER_PIN INIT_DIR_STOP_PIN_ENDSTOP_TOWER_PIN
#define ENDSTOP_CLEAR_PIN INIT_DIR_STOP_PIN_ENDSTOP_CLEAR_PIN
#define HOMING_PIN INIT_DIR_STOP_PIN_HOME_PIN
#define SW1_PIN INIT_DIR_STOP_PIN_SW1_PIN
#define SW2_PIN INIT_DIR_STOP_PIN_SW2_PIN

#define PWM_OUTPUT_STICK 0
#define PWM_OUTPUT_AXIS 1
#define PWM_OUTPUT_TOWER 2
#define PWM_OUTPUT_CLEAR 3

#define TOWER_MOVE_LEFT 1
#define TOWER_MOVE_RIGHT 0
#define AXIS_MOVE_UP 0
#define AXIS_MOVE_DOWN 1
#define STICK_MOVE_OUT 0
#define STICK_MOVE_IN 1
#define CLEAR_LEFT 1
#define CLEAR_RIGHT 0

#define STICK 0
#define AXIS 1
#define TOWER 2
#define CLEAR 3

#define RED_LED_PIN INIT_STATUS_LED_LED_RED_PIN
#define RED_LED_GPIO INIT_STATUS_LED_LED_RED_GPIO
#define GREEN_LED_PIN INIT_STATUS_LED_LED_GREEN_PIN
#define	GREEN_LED_GPIO INIT_STATUS_LED_LED_GREEN_GPIO
#define BLUE_LED_PIN INIT_STATUS_LED_LED_BLUE_PIN
#define	BLUE_LED_GPIO INIT_STATUS_LED_LED_BLUE_GPIO

#define START_X_1	181
#define START_Y_1	315
#define START_Z_1	170

#define START_X_2	184
#define START_Y_2	265
#define START_Z_2	170

#define START_X_3	187
#define START_Y_3	210
#define START_Z_3	169

#define START_X_4	186
#define START_Y_4	160
#define START_Z_4	169

#define REF_X	120
#define REF_Y	170
#define REF_Z	190

extern float START_POINT_1[3];
extern float START_POINT_2[3];
extern float START_POINT_3[3];
extern float START_POINT_4[3];

extern uint32_t MAX_FREQU_STICK;
extern uint32_t MAX_FREQU_AXIS;
extern uint32_t MAX_FREQU_TOWER;

extern float REF_POINT[3];

enum modus
{
		err = 0,
		homing = 1,
		automatic = 2,
		config = 3,
		idle = 4,
		test = 5,
		time_out = 6,
};

extern uint8_t h_0;
extern uint8_t h_1;
extern uint8_t min_0;
extern uint8_t min_1;

extern uint8_t h_0_raw;
extern uint8_t h_1_raw;
extern uint8_t min_0_raw;
extern uint8_t min_1_raw;

extern modus current_modus;

extern volatile bool homing_flag;
extern volatile bool automatic_flag;

extern volatile bool auto_flag_stick;
extern volatile bool auto_flag_axis;
extern volatile bool auto_flag_tower;

extern volatile bool busy_flag_stick;
extern volatile bool busy_flag_axis;
extern volatile bool busy_flag_tower;
extern volatile bool busy_flag_clean;

extern volatile bool prell_flag;

extern volatile uint32_t flank_count_stick;
extern volatile uint32_t flank_count_axis;
extern volatile uint32_t flank_count_tower;

extern volatile uint32_t freq_stick;
extern volatile uint32_t freq_axis;
extern volatile uint32_t freq_tower;

extern volatile uint32_t pwm_ms_count;
extern volatile uint32_t pwm_timer_counter;
extern volatile uint32_t pwm_timer_end;

extern volatile uint8_t err_count_read_gps;

extern volatile uint8_t recv_buffer_time[6];

void control_motor(uint32_t, uint32_t, uint32_t,
					uint32_t, uint32_t, uint32_t,
					uint32_t, uint32_t, uint32_t);

void do_homing(bool);
void move_sync_p_to_p (float *, float *, bool);
void sync_interpol (float *, float *);
void draw_number(uint8_t, float *);
void init_number_start_points();
void move_to_start_numb(uint8_t);
void move_to_end(uint8_t);
void check_for_change();
bool read_gps();
void clean_board (uint32_t);

#endif
