/*
*   Gerrit Hinrichs 06.2024
*   github.com/snech99
*
*   Clock_Robot
*   main
*/

#include "Robotik.h"

volatile uint32_t flank_count_stick;
volatile uint32_t flank_count_axis;
volatile uint32_t flank_count_tower;

volatile uint32_t freq_stick;
volatile uint32_t freq_axis;
volatile uint32_t freq_tower;

volatile uint32_t pwm_ms_count;
volatile uint32_t pwm_timer_counter = 0;
volatile uint32_t pwm_timer_end = 0;

volatile bool busy_flag_stick = false;
volatile bool busy_flag_axis = false;
volatile bool busy_flag_tower = false;
volatile bool busy_flag_clean = false;

volatile bool homing_flag = false;
volatile bool automatic_flag = false;

volatile bool auto_flag_stick = true;
volatile bool auto_flag_axis = true;
volatile bool auto_flag_tower = true;

volatile bool prell_flag = false;

volatile bool timer_check_for_new_flag = false;

volatile uint8_t recv_buffer_time[6] = {};

float START_POINT_1[3] = {};
float START_POINT_2[3] = {};
float START_POINT_3[3] = {};
float START_POINT_4[3] = {};

float REF_POINT[3] = {};

modus current_modus;

//Timer for PWM ramp
void TIMER2_CALLBACK_RAMPE(uint32_t flags)
{
	if (pwm_ms_count < 199)
	{
		float add_f_stick = (float)freq_stick/200;
		uint32_t new_f_stick = (uint32_t) add_f_stick + (pwm_ms_count*add_f_stick);

		float add_f_axis = (float)freq_axis/200;
		uint32_t new_f_axis = (uint32_t) add_f_axis + (pwm_ms_count*add_f_axis);

		float add_f_tower = (float)freq_tower/200;
		uint32_t new_f_tower = (uint32_t) add_f_tower + (pwm_ms_count*add_f_tower);

		if(new_f_axis == 0)
		{
			new_f_axis = 1;
		}

		if(new_f_tower == 0)
		{
			new_f_tower = 1;
		}

		if(new_f_stick == 0)
		{
			new_f_stick = 1;
		}

		pwm_ms_count++;

		PWM_SetupPwm(FLEXPWM0_PERIPHERAL, FLEXPWM0_STICK, FLEXPWM0_STICK_pwm_function_config, 1U, kPWM_SignedCenterAligned,
				new_f_stick, FLEXPWM0_STICK_SM_CLK_SOURCE_FREQ_HZ);

		PWM_SetupPwm(FLEXPWM0_PERIPHERAL, FLEXPWM0_AXIS, FLEXPWM0_AXIS_pwm_function_config, 1U, kPWM_SignedCenterAligned,
				new_f_axis, FLEXPWM0_AXIS_SM_CLK_SOURCE_FREQ_HZ);

		PWM_SetupPwm(FLEXPWM0_PERIPHERAL, FLEXPWM0_TOWER, FLEXPWM0_TOWER_pwm_function_config, 1U, kPWM_SignedCenterAligned,
				new_f_tower, FLEXPWM0_TOWER_SM_CLK_SOURCE_FREQ_HZ);

		FLEXPWM0_PERIPHERAL->MCTRL |= PWM_MCTRL_LDOK(kPWM_Control_Module_0);
		FLEXPWM0_PERIPHERAL->MCTRL |= PWM_MCTRL_LDOK(kPWM_Control_Module_1);
		FLEXPWM0_PERIPHERAL->MCTRL |= PWM_MCTRL_LDOK(kPWM_Control_Module_2);
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

		CTIMER_StopTimer(CTIMER2);
	}
}


void PWM_TIMER_CALLBACK(uint32_t flags)
{
	pwm_timer_counter++;
	if (pwm_timer_counter >= pwm_timer_end)
	{
		busy_flag_clean = false;
		CTIMER_StopTimer(CTIMER1);
	}
}

/* FLEXPWM0_SUBMODULE0_IRQn interrupt handler */
extern "C" void PWM_STICK_ISR(void)
{
	if (PWM_GetStatusFlags(FLEXPWM0_PERIPHERAL, FLEXPWM0_STICK) & kPWM_CompareVal2Flag)
	{
		if (!homing_flag)
		{
			flank_count_stick--;
		}

		if (flank_count_stick == 0 && automatic_flag)
		{
			auto_flag_stick = true;
		}

		if(flank_count_stick <= 0)
		{
			if(!automatic_flag)
			{
				busy_flag_stick = false;
				FLEXPWM0_PERIPHERAL->MCTRL &= ~(PWM_MCTRL_RUN(kPWM_Control_Module_0));
			}
		}
	}
	PWM_ClearStatusFlags(FLEXPWM0_PERIPHERAL, FLEXPWM0_STICK, kPWM_CompareVal2Flag);
}


/* FLEXPWM0_SUBMODULE1_IRQn interrupt handler */
extern "C" void PWM_AXIS_ISR(void)
{
	if (PWM_GetStatusFlags(FLEXPWM0_PERIPHERAL, FLEXPWM0_AXIS) & kPWM_CompareVal2Flag)
	{
		if (!homing_flag)
		{
			flank_count_axis--;
		}

		if (flank_count_axis == 0 && automatic_flag)
		{
			auto_flag_axis = true;
		}

		if(flank_count_axis <= 0)
		{
			if(!automatic_flag)
			{
				busy_flag_axis = false;
				FLEXPWM0_PERIPHERAL->MCTRL &= ~(PWM_MCTRL_RUN(kPWM_Control_Module_1));
			}
		}
	}
	PWM_ClearStatusFlags(FLEXPWM0_PERIPHERAL, FLEXPWM0_AXIS, kPWM_CompareVal2Flag);
}


/* FLEXPWM0_SUBMODULE2_IRQn interrupt handler */
extern "C" void PWM_TOWER_ISR(void)
{
	if (PWM_GetStatusFlags(FLEXPWM0_PERIPHERAL, FLEXPWM0_TOWER) & kPWM_CompareVal2Flag)
	{
		if (!homing_flag)
		{
			flank_count_tower--;
		}

		if (flank_count_tower == 0 && automatic_flag)
		{
			auto_flag_tower = true;
		}

		if(flank_count_tower <= 0)
		{
			if(!automatic_flag)
			{
				busy_flag_tower = false;
				FLEXPWM0_PERIPHERAL->MCTRL &= ~(PWM_MCTRL_RUN(kPWM_Control_Module_2));
			}
		}
	}
	PWM_ClearStatusFlags(FLEXPWM0_PERIPHERAL, FLEXPWM0_TOWER, kPWM_CompareVal2Flag);
}

/* GPIO1_IRQn interrupt handler */
extern "C" void GPIO1_IRQHANDLER(void)
{
	uint32_t hit_flag = GPIO_GpioGetInterruptFlags(GPIO1);

	if((hit_flag>>HOMING_PIN) == 1) //(hit_flag>>HOMING_PIN) == 1  (hit_flag == 0x400)
	{
		if (current_modus != err)
		{
			current_modus = homing;
		}
	}

	if((hit_flag>>SW1_PIN) == 1)	//(hit_flag == 0x1000)
	{
		if (current_modus != err)
		{
			current_modus = automatic;
		}
	}

	if((hit_flag>>SW2_PIN) == 1) //(hit_flag == 0x2000)
	{
		if (current_modus != err)
		{
			current_modus = automatic;
		}
	}

	GPIO_GpioClearInterruptFlags(GPIO1, 1<<HOMING_PIN);
	GPIO_GpioClearInterruptFlags(GPIO1, 1<<SW1_PIN);
	GPIO_GpioClearInterruptFlags(GPIO1, 1<<SW2_PIN);
}

/* GPIO2_IRQn interrupt handler */
extern "C" void GPIO2_IRQHANDLER(void)
{
	uint32_t hit_flag = GPIO_GpioGetInterruptFlags(GPIO2);
	if(hit_flag == 0x40)
	{
		busy_flag_stick = false;
		FLEXPWM0_PERIPHERAL->MCTRL &= ~(PWM_MCTRL_RUN(kPWM_Control_Module_0));
	}

	if(hit_flag == 0x1000)
	{
		busy_flag_axis = false;
		FLEXPWM0_PERIPHERAL->MCTRL &= ~(PWM_MCTRL_RUN(kPWM_Control_Module_1));
	}

	if(hit_flag == 0x2000)
	{
		busy_flag_tower = false;
		FLEXPWM0_PERIPHERAL->MCTRL &= ~(PWM_MCTRL_RUN(kPWM_Control_Module_2));
	}

	if(hit_flag == 0x10000)
	{
		if(prell_flag)
		{
			CTIMER_StopTimer(CTIMER1);
			busy_flag_clean = false;

		}
	}

	GPIO_GpioClearInterruptFlags(GPIO2, 1<<ENDSTOP_STICK_PIN);
	GPIO_GpioClearInterruptFlags(GPIO2, 1<<ENDSTOP_AXIS_PIN);
	GPIO_GpioClearInterruptFlags(GPIO2, 1<<ENDSTOP_TOWER_PIN);
	GPIO_GpioClearInterruptFlags(GPIO2, 1<<ENDSTOP_CLEAR_PIN);
}

void TIME_REQUEST_Callback(uint32_t flags)
{
	timer_check_for_new_flag = true;
}


/*
 * @brief   Application entry point.
 *
*/
int main(void) {

	BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitBootPeripherals();

    init_number_start_points();

    LPUART_TransferCreateHandle(LPUART2, &g_lpuartHandle, UART_UserCallback, NULL);

	current_modus = config;
	GPIO_PinWrite(RED_LED_GPIO, RED_LED_PIN, 1);
	GPIO_PinWrite(GREEN_LED_GPIO, GREEN_LED_PIN, 1);
	GPIO_PinWrite(BLUE_LED_GPIO, BLUE_LED_PIN, 1);

	clean_board(1);

    while(1)
    {
    	switch(current_modus)
    	{
    		case config:
    			if (send_all_driver_configs())
    	        {
    	        	LPUART_Deinit(LPUART2);
    	        	CTIMER_Deinit(CTIMER0);

    	        	CTIMER_Init(CTIMER0_PERIPHERAL, &CTIMER0_config);
    	        	CTIMER_SetupMatch(CTIMER0_PERIPHERAL, CTIMER0_MATCH_1_CHANNEL, &CTIMER0_Match_1_config);
    	        	CTIMER_RegisterCallBack(CTIMER0_PERIPHERAL, CTIMER0_callback, kCTIMER_SingleCallback);

    	        	GPIO_PinWrite(GREEN_LED_GPIO, GREEN_LED_PIN, 0);

        			do_homing(true);
    	        	current_modus = idle;

    				CTIMER_StartTimer(CTIMER0);
    	        }
    	        else
    	        {
        			GPIO_PinWrite(GREEN_LED_GPIO, GREEN_LED_PIN, 1);
        			GPIO_PinWrite(BLUE_LED_GPIO, BLUE_LED_PIN, 1);
        			GPIO_PinWrite(RED_LED_GPIO, RED_LED_PIN, 0);
    	        	current_modus = err;
    	        }
    	        break;

    		case homing:
    			do_homing(true);
    			clean_board(1);
    			current_modus = idle;
    			break;

    		case automatic:

				timer_check_for_new_flag = false;

    			if(read_gps())
    			{
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

    			}

    			while(!timer_check_for_new_flag)
    			{

    			}
    			break;

    		case test:

    			if(timer_check_for_new_flag)
    			{
    				timer_check_for_new_flag = false;
    				check_for_change();
    			}

    			break;

    		case time_out:

    			if(timer_check_for_new_flag)
    			{
    				timer_check_for_new_flag = false;
        			current_modus = test;
    			}

    			break;

    		case idle:
    			break;

    		case err:
    		    break;
    	}
    }
    return 0 ;
}



