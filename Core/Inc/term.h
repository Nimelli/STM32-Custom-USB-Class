#ifndef TERM_H
#define TERM_H

#include "main.h"

#define TERM_UART              (1) // 1=TRUE
#define TERM_UART_BUF_SIZE     128

#if (TERM_UART)
#include "stdio.h"
#include "usart.h"
extern char gl_uart_tx_buf[TERM_UART_BUF_SIZE];
extern UART_HandleTypeDef huart2;
#define term_printf(...) do{ \
                          int n_tx = sprintf(gl_uart_tx_buf, __VA_ARGS__); \
                          HAL_UART_Transmit(&huart2, (uint8_t*)gl_uart_tx_buf, n_tx, 10); \
                          }while(0)
#else
#define term_printf(...)    {}
#endif


#endif /* TERM_H */
