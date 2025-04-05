/*
*   Gerrit Hinrichs 06.2024
*   github.com/snech99
*
*   Clock_Robot
*   UART functions
*/
#include "UART.h"
#include "fsl_lpuart.h"
#include "fsl_ctimer.h"
#include "peripherals.h"
#include "TMC2209_UART.h"

#define CTIMER_OUT_VAL  (1u << kCTIMER_Match_0)

volatile bool txOnGoing_treiber                = false;
volatile bool rxOnGoing_treiber                = false;
volatile bool rxOnGoing_gps                = false;
volatile uint8_t err_count_read_gps = 0;


lpuart_handle_t g_lpuartHandle;
lpuart_transfer_t rxfer;

uint8_t h_0 = 0;
uint8_t h_1 = 0;
uint8_t min_0 = 0;
uint8_t min_1 = 0;
uint8_t sek_0 = 0;
uint8_t sek_1 = 0;


TMC2209_UNIT tmc_0(0);
TMC2209_UNIT tmc_1(1);
TMC2209_UNIT tmc_2(2);
TMC2209_UNIT tmc_3(3);
TMC2209 driver_0;
TMC2209 driver_1;
TMC2209 driver_2;
TMC2209 driver_3;

bool rx_read_ok = false;

uint8_t buffer_count = 0;
uint8_t g_txBuffer[8] = {0};
uint8_t g_rxBuffer[12] = {0};

void UART_GPS_Callback(LPUART_Type *base, lpuart_handle_t *handle, status_t status, void *userData)
{
    userData = userData;

    if (kStatus_LPUART_RxIdle == status)
    {
        rxOnGoing_gps     = false;
    }
}

// main function which reads the time from the GPS-module and parses the message
bool read_gps()
{
	uint8_t h_0_temp = 0;
	uint8_t h_1_temp = 0;
	uint8_t min_0_temp = 0;
	uint8_t min_1_temp = 0;


	GPIO_PinWrite(GREEN_LED_GPIO, GREEN_LED_PIN, 1);
	GPIO_PinWrite(BLUE_LED_GPIO, BLUE_LED_PIN, 1);
	GPIO_PinWrite(RED_LED_GPIO, RED_LED_PIN, 1);

	rxOnGoing_gps = true;
	uint16_t index = 0;

	LPUART1_rxBuffer[LPUART1_RX_BUFFER_SIZE] = {};
	LPUART_TransferReceiveNonBlocking(LPUART1_PERIPHERAL, &LPUART1_handle, &LPUART1_rxTransfer, NULL);

	GPIO_PinWrite(GREEN_LED_GPIO, GREEN_LED_PIN, 0);
	GPIO_PinWrite(BLUE_LED_GPIO, BLUE_LED_PIN, 0);
	GPIO_PinWrite(RED_LED_GPIO, RED_LED_PIN, 0);

	while(rxOnGoing_gps)
	{

	}

	GPIO_PinWrite(RED_LED_GPIO, RED_LED_PIN, 1);
	GPIO_PinWrite(GREEN_LED_GPIO, GREEN_LED_PIN, 1);
	GPIO_PinWrite(BLUE_LED_GPIO, BLUE_LED_PIN, 1);

	while ( (LPUART1_rxBuffer[index] != 0x52) || (LPUART1_rxBuffer[index+1] != 'M') )
	{
		index++;
		if(index > LPUART1_RX_BUFFER_SIZE)
		{
			GPIO_PinWrite(GREEN_LED_GPIO, GREEN_LED_PIN, 1);
			GPIO_PinWrite(BLUE_LED_GPIO, BLUE_LED_PIN, 0);
			GPIO_PinWrite(RED_LED_GPIO, RED_LED_PIN, 0);

			if(current_modus == automatic)
			{
				return false;

			}
			else
			{
				current_modus = time_out;
				return false;
			}
		}
	}


	if( (LPUART1_rxBuffer[index+4] == 0) && (LPUART1_rxBuffer[index+5] == 0)
			&& (LPUART1_rxBuffer[index+6] == 0) && (LPUART1_rxBuffer[index+7] == 0) )
	{
		GPIO_PinWrite(GREEN_LED_GPIO, GREEN_LED_PIN, 1);
		GPIO_PinWrite(BLUE_LED_GPIO, BLUE_LED_PIN, 0);
		GPIO_PinWrite(RED_LED_GPIO, RED_LED_PIN, 1);

		if(current_modus == automatic)
		{
			return false;

		}
		else
		{
			current_modus = time_out;
			return false;
		}
	}

	if(LPUART1_rxBuffer[index+4] == ',')
	{
		GPIO_PinWrite(GREEN_LED_GPIO, GREEN_LED_PIN, 1);
		GPIO_PinWrite(BLUE_LED_GPIO, BLUE_LED_PIN, 0);
		GPIO_PinWrite(RED_LED_GPIO, RED_LED_PIN, 1);

		if(current_modus == automatic)
		{
			return false;

		}
		else
		{
			current_modus = time_out;
			return false;
		}
	}

	h_0_temp = LPUART1_rxBuffer[index+4]-48;
	h_1_temp = LPUART1_rxBuffer[index+5]-48;
	min_0_temp = LPUART1_rxBuffer[index+6]-48;
	min_1_temp = LPUART1_rxBuffer[index+7]-48;

	uint16_t temp_h = 10*h_0_temp + h_1_temp + 2;
	h_1_temp = temp_h%10;
	h_0_temp = (temp_h - h_1_temp)/10;

	if( (h_0_temp >= 0) && (h_0_temp < 10) )
	{
	}
	else
	{
		GPIO_PinWrite(GREEN_LED_GPIO, GREEN_LED_PIN, 1);
		GPIO_PinWrite(BLUE_LED_GPIO, BLUE_LED_PIN, 0);
		GPIO_PinWrite(RED_LED_GPIO, RED_LED_PIN, 1);

		if(current_modus == automatic)
		{
			return false;

		}
		else
		{
			current_modus = time_out;
			return false;
		}
	}

	if( (h_1_temp >= 0) && (h_1_temp < 10) )
	{
	}
	else
	{
		GPIO_PinWrite(GREEN_LED_GPIO, GREEN_LED_PIN, 1);
		GPIO_PinWrite(BLUE_LED_GPIO, BLUE_LED_PIN, 0);
		GPIO_PinWrite(RED_LED_GPIO, RED_LED_PIN, 1);

		if(current_modus == automatic)
		{
			return false;

		}
		else
		{
			current_modus = time_out;
			return false;
		}
	}


	if( (min_0_temp >= 0) && (min_0_temp < 10) )
	{
	}
	else
	{
		GPIO_PinWrite(GREEN_LED_GPIO, GREEN_LED_PIN, 1);
		GPIO_PinWrite(BLUE_LED_GPIO, BLUE_LED_PIN, 0);
		GPIO_PinWrite(RED_LED_GPIO, RED_LED_PIN, 1);

		if(current_modus == automatic)
		{
			return false;

		}
		else
		{
			current_modus = time_out;
			return false;
		}
	}

	if( (min_1_temp >= 0) && (min_1_temp < 10) )
	{
	}
	else
	{
		GPIO_PinWrite(GREEN_LED_GPIO, GREEN_LED_PIN, 1);
		GPIO_PinWrite(BLUE_LED_GPIO, BLUE_LED_PIN, 0);
		GPIO_PinWrite(RED_LED_GPIO, RED_LED_PIN, 1);

		if(current_modus == automatic)
		{
			return false;

		}
		else
		{
			current_modus = time_out;
			return false;
		}
	}

	GPIO_PinWrite(RED_LED_GPIO, RED_LED_PIN, 1);
	GPIO_PinWrite(GREEN_LED_GPIO, GREEN_LED_PIN, 1);
	GPIO_PinWrite(BLUE_LED_GPIO, BLUE_LED_PIN, 1);

	h_0 = h_0_temp;
	h_1 = h_1_temp;
	min_0 = min_0_temp;
	min_1 = min_1_temp;

	current_modus = test;
	return true;
}


/* LPUART user callback */
void UART_UserCallback(LPUART_Type *base, lpuart_handle_t *handle, status_t status, void *userData)
{
    userData = userData;

    if (kStatus_LPUART_TxIdle == status)
    {
        txOnGoing_treiber    = false;
    }

    if (kStatus_LPUART_RxIdle == status)
    {
        rxOnGoing_treiber     = false;
    }
};


void send_config_lpuart(uint64_t message)
{
	lpuart_transfer_t txfer;
	union union_m
	{
		uint64_t long_message;
		uint8_t short_message [8];
	}m;

	m.long_message = message;

	txfer.data     = m.short_message;
	txfer.dataSize = sizeof(m.short_message);

    if (!txOnGoing_treiber)
    {
    	txOnGoing_treiber = true;
		LPUART_TransferSendNonBlocking(LPUART2, &g_lpuartHandle, &txfer);
    }
    while (txOnGoing_treiber)
    {

    }
}

void send_request_lpuart(uint32_t message)
{
	lpuart_transfer_t txfer;
	union union_m
	{
		uint32_t long_message;
		uint8_t short_message [4];
	}m;

	m.long_message = message;

	txfer.data     = m.short_message;
	txfer.dataSize = sizeof(m.short_message);

    if (!txOnGoing_treiber)
    {
    	txOnGoing_treiber = true;
		LPUART_TransferSendNonBlocking(LPUART2, &g_lpuartHandle, &txfer);
    }
    while (txOnGoing_treiber)
    {

    }

}

uint64_t get_response_lpuart (void)
{
	bool run = true;

    g_rxBuffer[12] = {0};

    rxfer.data = g_rxBuffer;
    rxfer.dataSize = sizeof(g_rxBuffer);

    rxOnGoing_treiber = true;
	LPUART_TransferReceiveNonBlocking(LPUART2, &g_lpuartHandle, &rxfer, NULL);


	CTIMER_Init(CTIMER0_PERIPHERAL, &CTIMER0_config);
	CTIMER_SetupMatch(CTIMER0_PERIPHERAL, CTIMER0_MATCH_0_CHANNEL, &CTIMER0_Match_0_config);
	CTIMER0->TCR |= CTIMER_TCR_CEN_MASK;

	while(run)
	{
		if (CTIMER_GetOutputMatchStatus(CTIMER0, CTIMER_OUT_VAL) & kCTIMER_External_Match_0)
		{
			run = false;
		}
	}

	CTIMER0->TCR &= ~CTIMER_TCR_CEN_MASK;

	union union_m
	{
		uint64_t long_message;
		uint8_t short_message [8];
	}m;

	for(uint8_t i=0; i<8; i++)
	{
		m.short_message[i] = rxfer.data[4+i];;
	}


	if (rxOnGoing_treiber)
	{
		rxOnGoing_treiber = false;
		return UINT64_MAX;
	}
	else
	{
		return m.long_message;
	}
}

bool send_all_driver_configs()
{
	bool check_0 = false;
	bool check_1 = false;
	bool check_2 = false;

	check_0 = driver_0.setupDefault(&tmc_0);

	if (check_0)
	{
		check_1 = driver_1.setupDefault(&tmc_1);
	}
	else
	{
		return false;
	}

	if(check_1)
	{
		check_2 = driver_2.setupDefault(&tmc_2);
	}
	else
	{
		return false;
	}

	if(check_2)
	{
		driver_3.setupDefault(&tmc_3);
	}
	else
	{
		return false;
	}

	return true;
}

