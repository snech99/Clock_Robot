/*
*   Gerrit Hinrichs 06.2024
*   github.com/snech99
*
*   Clock_Robot
*   different move-functions
*/

#include "Robotik.h"

// function to clean the board
void clean_board (uint32_t num)
{
	pwm_timer_counter = 0;
	switch (num)
	{
		case 1:
			pwm_timer_end = 38900;
			break;
		case 2:
			pwm_timer_end = 32000;
			break;
		case 3:
			pwm_timer_end = 20000;
			break;
		case 4:
			pwm_timer_end = 10000;
			break;
	}

	prell_flag = false;
	GPIO_PinWrite(DIR_CLEAN_GPIO, DIR_CLEAN_PIN, CLEAR_LEFT);
	busy_flag_clean = true;

	CTIMER_StartTimer(CTIMER1);
	while(busy_flag_clean)
	{

	}

	prell_flag = true;
	pwm_timer_end = pwm_timer_end + 10000000;
	GPIO_PinWrite(DIR_CLEAN_GPIO, DIR_CLEAN_PIN, CLEAR_RIGHT);
	busy_flag_clean = true;

	CTIMER_StartTimer(CTIMER1);
	while(busy_flag_clean)
	{

	}
}

// function which checks the time for a change and draw the new number
void check_for_change()
{
	uint8_t h_0_old = h_0;
	uint8_t h_1_old = h_1;
	uint8_t min_0_old = min_0;
	uint8_t min_1_old = min_1;

	if (!read_gps())
	{
		return;
	}

	if(h_0 != h_0_old)
	{
		clean_board(1);

		move_to_start_numb(1);
		draw_number(h_0, START_POINT_1);
		move_to_end(1);

		move_to_start_numb(2);
		draw_number(h_1, START_POINT_2);
		move_to_end(2);

		move_to_start_numb(3);
		draw_number(min_0, START_POINT_3);
		move_to_end(3);

		move_to_start_numb(4);
		draw_number(min_1, START_POINT_4);
		move_to_end(4);

		return;
	}


	if(h_1 != h_1_old)
	{
		clean_board(2);

		move_to_start_numb(2);
		draw_number(h_1, START_POINT_2);
		move_to_end(2);

		move_to_start_numb(3);
		draw_number(min_0, START_POINT_3);
		move_to_end(3);

		move_to_start_numb(4);
		draw_number(min_1, START_POINT_4);
		move_to_end(4);

		return;
	}

	if(min_0 != min_0_old)
	{
		clean_board(3);

		move_to_start_numb(3);
		draw_number(min_0, START_POINT_3);
		move_to_end(3);

		move_to_start_numb(4);
		draw_number(min_1, START_POINT_4);
		move_to_end(4);

		return;
	}

	if(min_1 != min_1_old)
	{
		clean_board(4);

		move_to_start_numb(4);
		draw_number(min_1, START_POINT_4);
		move_to_end(4);

		return;
	}
}

// function which perform a homing of the robot
void do_homing(bool ret)
{
	if(ret)
	{
		control_motor(	150, 400, STICK_MOVE_IN,
			    		100, 200, AXIS_MOVE_DOWN,
			    		200, 400, TOWER_MOVE_LEFT);
	}

	homing_flag = true;
	control_motor(	1, 2000, STICK_MOVE_OUT,
	    			1, 500, AXIS_MOVE_UP,
					1, 2000, TOWER_MOVE_RIGHT);

	homing_flag = false;
	control_motor(	150, 400, STICK_MOVE_IN,
	    			100, 200, AXIS_MOVE_DOWN,
	    			200, 400, TOWER_MOVE_LEFT);

	homing_flag = true;
	control_motor(	1, 200, STICK_MOVE_OUT,
		    		1, 100, AXIS_MOVE_UP,
					1, 200, TOWER_MOVE_RIGHT);
	homing_flag = false;
}

// main moving-function for the different motors
void control_motor(uint32_t f_stick_count, uint32_t freq_m_stick, uint32_t dir_stick,
					uint32_t f_axis_count, uint32_t freq_m_axis, uint32_t dir_axis,
					uint32_t f_tower_count, uint32_t freq_m_tower, uint32_t dir_tower)
{
    GPIO_PinWrite(DIR_STICK_GPIO, DIR_STICK_PIN, dir_stick);
    GPIO_PinWrite(DIR_AXIS_GPIO, DIR_AXIS_PIN, dir_axis);
    GPIO_PinWrite(DIR_TOWER_GPIO, DIR_TOWER_PIN, dir_tower);

	flank_count_stick = f_stick_count;
	freq_stick = freq_m_stick;

	flank_count_axis = f_axis_count;
	freq_axis = freq_m_axis;

	flank_count_tower = f_tower_count;
	freq_tower = freq_m_tower;

	busy_flag_stick = false;
	busy_flag_axis = false;
	busy_flag_tower = false;

	if(flank_count_stick != 0)
	{
		busy_flag_stick = true;
		FLEXPWM0_PERIPHERAL->MCTRL |= PWM_MCTRL_RUN(kPWM_Control_Module_0);
	}

	if(flank_count_axis != 0)
	{
		busy_flag_axis = true;
		FLEXPWM0_PERIPHERAL->MCTRL |= PWM_MCTRL_RUN(kPWM_Control_Module_1);
	}

	if(flank_count_tower != 0)
	{
		busy_flag_tower = true;
		FLEXPWM0_PERIPHERAL->MCTRL |= PWM_MCTRL_RUN(kPWM_Control_Module_2);
	}

	CTIMER_StartTimer(CTIMER2);
	pwm_ms_count = 0;

	while(busy_flag_stick || busy_flag_axis || busy_flag_tower)
	{

	}
}

// help-function for syncronic movement
void move_sync_p_to_p (float *p_begin, float *p_end, bool ramp)
{
	float erg_begin[3] = {};
	float erg_end[3] = {};

	inverse(p_begin[0], p_begin[1], p_begin[2], erg_begin);
	inverse(p_end[0], p_end[1], p_end[2], erg_end);

	float delta_t_1 = erg_end[0] - erg_begin[0];
	float delta_t_2 = erg_end[1] - erg_begin[1];
	float delta_d_4 = erg_end[2] - erg_begin[2];

	float way_t_1 = delta_t_1*STEP_FACTOR*18.5;
	float way_t_2 = delta_t_2*STEP_FACTOR*5;
	float way_d_4 = delta_d_4*STEP_FACTOR*(18.2*M_PI/360);

	float needed_time_stick = fabsf(way_d_4)/MAX_FREQU_STICK;
	float needed_time_axis = fabsf(way_t_2)/MAX_FREQU_AXIS;
	float needed_time_tower = fabsf(way_t_1)/MAX_FREQU_TOWER;

	float new_frequ_stick = MAX_FREQU_STICK;
	float new_frequ_axis = MAX_FREQU_AXIS;
	float new_frequ_tower = MAX_FREQU_TOWER;

	if(needed_time_stick >= needed_time_axis)
	{
		if(needed_time_stick >= needed_time_tower)
		{
			new_frequ_axis = fabsf(way_t_2)/needed_time_stick;
			new_frequ_tower = fabsf(way_t_1)/needed_time_stick;
		}
		else
		{
			new_frequ_stick = fabsf(way_d_4)/needed_time_tower;
			new_frequ_axis = fabsf(way_t_2)/needed_time_tower;
		}
	}
	else
	{
		if(needed_time_axis >= needed_time_tower)
		{
			new_frequ_stick = fabsf(way_d_4)/needed_time_axis;
			new_frequ_tower = fabsf(way_t_1)/needed_time_axis;
		}
		else
		{
			new_frequ_stick = fabsf(way_d_4)/needed_time_tower;
			new_frequ_axis = fabsf(way_t_2)/needed_time_tower;
		}
	}

	uint32_t dir_stick;
	uint32_t dir_axis;
	uint32_t dir_tower;

	dir_stick = way_d_4 < 0 ? STICK_MOVE_IN : STICK_MOVE_OUT;
	dir_axis = way_t_2 < 0 ? AXIS_MOVE_UP: AXIS_MOVE_DOWN;
	dir_tower = way_t_1 < 0 ? TOWER_MOVE_RIGHT : TOWER_MOVE_LEFT;

	flank_count_stick = (uint32_t)fabsf(way_d_4);
	freq_stick = (uint32_t) new_frequ_stick;

	flank_count_axis = (uint32_t)fabsf(way_t_2);
	freq_axis = (uint32_t) new_frequ_axis;

	flank_count_tower = (uint32_t)fabsf(way_t_1);
	freq_tower = (uint32_t) new_frequ_tower;

	GPIO_PinWrite(DIR_STICK_GPIO, DIR_STICK_PIN, dir_stick);
	GPIO_PinWrite(DIR_AXIS_GPIO, DIR_AXIS_PIN, dir_axis);
	GPIO_PinWrite(DIR_TOWER_GPIO, DIR_TOWER_PIN, dir_tower);

	busy_flag_stick = false;
	busy_flag_axis = false;
	busy_flag_tower = false;

	if(ramp)
	{
		if(flank_count_stick != 0)
		{
			busy_flag_stick = true;
			FLEXPWM0_PERIPHERAL->MCTRL |= PWM_MCTRL_RUN(kPWM_Control_Module_0);
		}

		if(flank_count_axis != 0)
		{
			busy_flag_axis = true;
			FLEXPWM0_PERIPHERAL->MCTRL |= PWM_MCTRL_RUN(kPWM_Control_Module_1);
		}

		if(flank_count_tower != 0)
		{
			busy_flag_tower = true;
			FLEXPWM0_PERIPHERAL->MCTRL |= PWM_MCTRL_RUN(kPWM_Control_Module_2);
		}

		CTIMER_StartTimer(CTIMER2);
		pwm_ms_count = 0;
	}
	else
	{
		if(freq_axis == 0)
		{
			freq_axis = 1;
		}

		if(freq_tower == 0)
		{
			freq_tower = 1;
		}

		if(freq_stick == 0)
		{
			freq_stick = 1;
		}

		PWM_SetupPwm(FLEXPWM0_PERIPHERAL, FLEXPWM0_STICK, FLEXPWM0_STICK_pwm_function_config, 1U, kPWM_SignedCenterAligned,
								freq_stick, FLEXPWM0_STICK_SM_CLK_SOURCE_FREQ_HZ);

		PWM_SetupPwm(FLEXPWM0_PERIPHERAL, FLEXPWM0_AXIS, FLEXPWM0_AXIS_pwm_function_config, 1U, kPWM_SignedCenterAligned,
								freq_axis, FLEXPWM0_AXIS_SM_CLK_SOURCE_FREQ_HZ);

		PWM_SetupPwm(FLEXPWM0_PERIPHERAL, FLEXPWM0_TOWER, FLEXPWM0_TOWER_pwm_function_config, 1U, kPWM_SignedCenterAligned,
								freq_tower, FLEXPWM0_TOWER_SM_CLK_SOURCE_FREQ_HZ);

		FLEXPWM0_PERIPHERAL->MCTRL |= PWM_MCTRL_LDOK(kPWM_Control_Module_0);
		FLEXPWM0_PERIPHERAL->MCTRL |= PWM_MCTRL_LDOK(kPWM_Control_Module_1);
		FLEXPWM0_PERIPHERAL->MCTRL |= PWM_MCTRL_LDOK(kPWM_Control_Module_2);

		if(flank_count_stick != 0)
		{
			busy_flag_stick = true;
			FLEXPWM0_PERIPHERAL->MCTRL |= PWM_MCTRL_RUN(kPWM_Control_Module_0);
		}

		if(flank_count_axis != 0)
		{
			busy_flag_axis = true;
			FLEXPWM0_PERIPHERAL->MCTRL |= PWM_MCTRL_RUN(kPWM_Control_Module_1);
		}

		if(flank_count_tower != 0)
		{
			busy_flag_tower = true;
			FLEXPWM0_PERIPHERAL->MCTRL |= PWM_MCTRL_RUN(kPWM_Control_Module_2);
		}

	}

	while(busy_flag_stick || busy_flag_axis || busy_flag_tower)
	{

	}
}

// function for the syncronic movement (interpolation)
void sync_interpol (float *P_start, float *P_end)
{
	automatic_flag = true;
	auto_flag_stick = true;
	auto_flag_axis = true;
	auto_flag_tower = true;

	bool ramp = true;
	float t_array[6];

	t_array[0] = 0;

	for(int i=1; i<=5; i++)
	{
		t_array[i] = t_array[i-1]+0.2;
	}

	float inter_points[6][3] = {0};

    for(int i=0; i<=5; i++)
	{
		for(int k=0; k<3; k++)
		{
			inter_points[i][k] = (1-t_array[i])*P_start[k]+t_array[i]*P_end[k];
		}
	}

	float erg[3] = {};
	inverse(inter_points[0][0], inter_points[0][1], inter_points[0][2], erg);

	float delt[5][3] = {};

	float t1_old = erg[0];
	float t2_old = erg[1];
	float d4_old = erg[2];

	for(int i=1; i<=5; i++)
	{
		inverse(inter_points[i][0], inter_points[i][1], inter_points[i][2], erg);
		delt[i-1][0] = erg[0]-t1_old;
		delt[i-1][1] = erg[1]-t2_old;
		delt[i-1][2] = erg[2]-d4_old;

		t1_old = erg[0];
		t2_old = erg[1];
		d4_old = erg[2];
	}

	for (int i=0; i<5; i++)
	{
		float way_t_1 = delt[i][0]*STEP_FACTOR*18.5;
		float way_t_2 = delt[i][1]*STEP_FACTOR*5;
		float way_d_4 = delt[i][2]*STEP_FACTOR*(17.8*M_PI/360);

		float needed_time_stick = fabsf(way_d_4)/MAX_FREQU_STICK;
		float needed_time_axis = fabsf(way_t_2)/MAX_FREQU_AXIS;
		float needed_time_tower = fabsf(way_t_1)/MAX_FREQU_TOWER;

		float new_frequ_stick = MAX_FREQU_STICK;
		float new_frequ_axis = MAX_FREQU_AXIS;
		float new_frequ_tower = MAX_FREQU_TOWER;

		if(needed_time_stick >= needed_time_axis)
		{
			if(needed_time_stick >= needed_time_tower)
			{
				new_frequ_axis = fabsf(way_t_2)/needed_time_stick;
				new_frequ_tower = fabsf(way_t_1)/needed_time_stick;
			}
			else
			{
				new_frequ_stick = fabsf(way_d_4)/needed_time_tower;
				new_frequ_axis = fabsf(way_t_2)/needed_time_tower;
			}
		}
		else
		{
			if(needed_time_axis >= needed_time_tower)
			{
				new_frequ_stick = fabsf(way_d_4)/needed_time_axis;
				new_frequ_tower = fabsf(way_t_1)/needed_time_axis;
			}
			else
			{
				new_frequ_stick = fabsf(way_d_4)/needed_time_tower;
				new_frequ_axis = fabsf(way_t_2)/needed_time_tower;
			}
		}

		uint32_t dir_stick;
		uint32_t dir_axis;
		uint32_t dir_tower;

		dir_stick = way_d_4 < 0 ? STICK_MOVE_IN : STICK_MOVE_OUT;
		dir_axis = way_t_2 < 0 ? AXIS_MOVE_UP: AXIS_MOVE_DOWN;
		dir_tower = way_t_1 < 0 ? TOWER_MOVE_RIGHT : TOWER_MOVE_LEFT;

		busy_flag_stick = false;
		busy_flag_axis = false;
		busy_flag_tower = false;

		while(!auto_flag_stick || !auto_flag_axis || !auto_flag_tower)
		{

		}

		flank_count_stick = (uint32_t)fabsf(way_d_4);
		freq_stick = (uint32_t) new_frequ_stick;

		flank_count_axis = (uint32_t)fabsf(way_t_2);
		freq_axis = (uint32_t) new_frequ_axis;

		flank_count_tower = (uint32_t)fabsf(way_t_1);
		freq_tower = (uint32_t) new_frequ_tower;

		GPIO_PinWrite(DIR_STICK_GPIO, DIR_STICK_PIN, dir_stick);
		GPIO_PinWrite(DIR_AXIS_GPIO, DIR_AXIS_PIN, dir_axis);
		GPIO_PinWrite(DIR_TOWER_GPIO, DIR_TOWER_PIN, dir_tower);

		if(i == 4)
		{
			automatic_flag = false;
		}

		if(ramp)
		{
			ramp = false;
			if( (freq_stick != 0) && (flank_count_stick != 0) )
			{
				auto_flag_stick = false;
				busy_flag_stick = true;
				FLEXPWM0_PERIPHERAL->MCTRL |= PWM_MCTRL_RUN(kPWM_Control_Module_0);
			}

			if( (freq_axis != 0) && (flank_count_axis != 0) )
			{
				auto_flag_axis = false;
				busy_flag_axis = true;
				FLEXPWM0_PERIPHERAL->MCTRL |= PWM_MCTRL_RUN(kPWM_Control_Module_1);
			}

			if( (freq_tower != 0) && (flank_count_tower!= 0) )
			{
				auto_flag_tower = false;
				busy_flag_tower = true;
				FLEXPWM0_PERIPHERAL->MCTRL |= PWM_MCTRL_RUN(kPWM_Control_Module_2);
			}

			CTIMER_StartTimer(CTIMER2);
			pwm_ms_count = 0;
		}
		else
		{
			if( (freq_stick != 0) && (flank_count_stick != 0) )
			{
				auto_flag_stick = false;
				PWM_SetupPwm(FLEXPWM0_PERIPHERAL, FLEXPWM0_STICK, FLEXPWM0_STICK_pwm_function_config, 1U, kPWM_SignedCenterAligned,
								freq_stick, FLEXPWM0_STICK_SM_CLK_SOURCE_FREQ_HZ);
			}
			if( (freq_axis != 0) && (flank_count_axis != 0) )
			{
				auto_flag_axis = false;
				PWM_SetupPwm(FLEXPWM0_PERIPHERAL, FLEXPWM0_AXIS, FLEXPWM0_AXIS_pwm_function_config, 1U, kPWM_SignedCenterAligned,
								freq_axis, FLEXPWM0_AXIS_SM_CLK_SOURCE_FREQ_HZ);
			}

			if( (freq_tower != 0) && (flank_count_tower!= 0) )
			{
				auto_flag_tower = false;
				PWM_SetupPwm(FLEXPWM0_PERIPHERAL, FLEXPWM0_TOWER, FLEXPWM0_TOWER_pwm_function_config, 1U, kPWM_SignedCenterAligned,
								freq_tower, FLEXPWM0_TOWER_SM_CLK_SOURCE_FREQ_HZ);
			}

			FLEXPWM0_PERIPHERAL->MCTRL |= PWM_MCTRL_LDOK(kPWM_Control_Module_0);
			FLEXPWM0_PERIPHERAL->MCTRL |= PWM_MCTRL_LDOK(kPWM_Control_Module_1);
			FLEXPWM0_PERIPHERAL->MCTRL |= PWM_MCTRL_LDOK(kPWM_Control_Module_2);
		}
	}
}
