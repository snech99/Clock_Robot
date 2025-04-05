/*
*   Gerrit Hinrichs 06.2024
*   github.com/snech99
*
*   Clock_Robot
*   UART header
*/

#ifndef UART_H_
#define UART_H_

#include "fsl_common.h"
#include "fsl_lpuart.h"
#include "Robotik.h"

extern lpuart_handle_t g_lpuartHandle;

extern "C" void LPUART2_SERIAL_RX_TX_IRQHANDLER(void);

void UART_UserCallback(LPUART_Type*, lpuart_handle_t*, status_t, void*);

void send_request_lpuart(uint32_t);
void send_config_lpuart(uint64_t);
uint64_t get_response_lpuart(void);
bool send_all_driver_configs();

#endif
