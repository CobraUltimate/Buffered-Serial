
/**
 * \mainpage Buffered Serial
 * \author Ramsés F. Pérez
 * \date August 2020
 * \version 1.0.0
 *
 * **Features:**
 *
 * - Developed for the STM32F103.
 * - Serial communication with DMA in circular mode and IDLE interrupt.
 * - Configurable quantity of serials and size of rx and tx buffers.
 * - Simple communication with print string and read line functions.
 * - STM32CubeIDE project configuration guide.
 * - Error handling with buffered_serial_error_code.
 *
 * **Considerations:**
 *
 * - BUFFERED_SERIAL_SERIALS_QUANTITY must be configured to correspond the quantity of huart configured, by default is one.
 * - BUFFERED_SERIAL_BUFFERS_SIZE at most can be maximum value of uint16_t, since buffered_serial_available is this type.
 * - Buffers can hold at most BUFFERED_SERIAL_BUFFERS_SIZE - 1 data, because when rx_buffer_data_start and rx_buffer_data_finish pointers are equals it can be 0 data or maximum data but the library interpret as 0 data.
 *
 * \n
 * **GETTING STARTED**
 *
 * \n
 * **Configure IDLE interrupt in stm32f1xx_it.c**
 * \n
 *
 * Configure project as described in file project_configuration.pdf in root folder.
 * IDLE interrupt must be configured for all huart interrupt handlers.
 *
 * @code
 * void USART1_IRQHandler(void)
 * {
 *   HAL_UART_IRQHandler(&huart1);
 *   buffered_serial_update_rx_buffer_data(&huart1);
 * }
 * @endcode
 *
 * \n
 * **Initializing library and getting serial descriptor in main.c file**
 * \n
 *
 * @code
 * MX_GPIO_Init();
 * MX_DMA_Init();
 * MX_USART1_UART_Init();
 * UART_HandleTypeDef *huarts[] = {&huart1};
 * buffered_serial_init(huarts);
 * buffered_serial_serial_descriptor *serial1 = buffered_serial_get_huart_serial_descriptor(&huart1);
 * @endcode
 *
 * \n
 * **Writing a string**
 * \n
 *
 * @code
 * uint8_t test[40] = "2A6V7W5NL5ZZC6AYE84NKZ6MVFMZ5DZSYD9TM3\r\n";
 * static_strings_string_descriptor *string_descriptor = static_strings_save(test);
 * buffered_serial_print_string(test,string_descriptor);
 * static_strings_deallocate(string_descriptor);
 * @endcode
 *
 * DON'T FORGET TO DEALLOCATE STRING AFTER USING.\n
 *
 * \n
 * **Reading a line**
 * \n
 *
 * @code
 * if(buffered_serial_available(serial1) > 0){
 *   uint16_t available = buffered_serial_available(serial1);
 *   static_strings_string_descriptor *string_descriptor = buffered_serial_read_line(serial1);
 *   if(string_descriptor != NULL){
 *     buffered_serial_print_string(serial1,string_descriptor);
 *     static_strings_deallocate(string_descriptor);
 *   }
 *   else{
 *   	handle_error(buffered_serial_error_code);
 *   }
 * }
 * @endcode
 *
 * DON'T FORGET TO DEALLOCATE STRING AFTER USING.\n
 *
 * \n
 * **Configure serials quantity and size of the buffers**
 * \n
 *
 * Just edit these constants in buffered_serial.h
 *
 * @code
 * #define BUFFERED_SERIAL_SERIALS_QUANTITY 1
 * #define BUFFERED_SERIAL_BUFFERS_SIZE 500
 * @endcode
 *
 */

/** \file buffered_serial.h
 * \brief Serial communication based on a circular buffer, dma and huart with hal controls and Static Strings.
 */

#ifndef BUFFERED_SERIAL_H
#define BUFFERED_SERIAL_H

#include "stm32f1xx_hal.h"
#include "stm32f1xx_hal_uart.h"
#include "static_strings.h"

/** \defgroup serial_configuration Serial buffers size and quantity
 * \brief Constants to configure the quantity of serials and the size of their buffers.
 */
///@{
#define BUFFERED_SERIAL_SERIALS_QUANTITY 1
#define BUFFERED_SERIAL_BUFFERS_SIZE 500
///@}

/** \defgroup error_handling Error handling
 * \brief Error codes.
 */
///@{
#define BUFFERED_SERIAL_ERROR_CODE_STATIC_STRINGS_ERROR 0
#define BUFFERED_SERIAL_ERROR_CODE_NO_LINE_ENDING_DETECTED 1

/** static_strings_error_code
 * \brief Global variable to store error code.
 */
uint8_t buffered_serial_error_code;
///@}

/** \struct buffered_serial_serial_descriptor
 * \brief Meta data of a buffered serial.
 */
typedef struct buffered_serial_serial_descriptor{
	UART_HandleTypeDef *huart;
	uint8_t *rx_buffer;
	/** Pointer to the first readable character on the buffer. */
	uint8_t *rx_buffer_data_start;
	/** Pointer to the position ahead the last readable character on buffer. */
	uint8_t *rx_buffer_data_finish;
	uint8_t *tx_buffer;
}buffered_serial_serial_descriptor;

/** \defgroup serial_buffers Serial buffers
 * \brief rx and tx buffers to receive and transmit data.
 */
///@{
uint8_t buffered_serial_rx_buffers[BUFFERED_SERIAL_SERIALS_QUANTITY][BUFFERED_SERIAL_BUFFERS_SIZE];
uint8_t buffered_serial_tx_buffers[BUFFERED_SERIAL_SERIALS_QUANTITY][BUFFERED_SERIAL_BUFFERS_SIZE];
///@}

buffered_serial_serial_descriptor buffered_serial_serial_descriptors[BUFFERED_SERIAL_SERIALS_QUANTITY];

/** void buffered_serial_init(UART_HandleTypeDef **huarts)
 * \brief Link huarts and buffers with serial descriptors and init rx data receiving and idle interrupt. Also init the Static Strings library.
 * \param huarts Array of pointers to huart pointer.
 */
void buffered_serial_init(UART_HandleTypeDef **huarts);

/** buffered_serial_serial_descriptor buffered_serial_get_huart_serial_descriptor(UART_HandleTypeDef *huart)
 * \brief Returns the serial_descriptor of the provided huart.
 * \param huart Pointer to a UART_HandleTypeDef.
 * \return A pointer to the serial descriptor of the provided huart. Return NULL if there is no serial descriptor attached to the huart provided.
 */
buffered_serial_serial_descriptor *buffered_serial_get_huart_serial_descriptor(UART_HandleTypeDef *huart);

/** uint16_t buffered_serial_available(buffered_serial_serial_descriptor *serial)
 * \brief Calculates and returns the number of characters that can be read from the rx buffer.
 * \param serial Pointer to the serial descriptor of the target huart.
 * \return Number of characters that can be read from the rx buffer.
 */
uint16_t buffered_serial_available(buffered_serial_serial_descriptor *serial);

/** void buffered_serial_print_string(static_strings_string_descriptor *string,buffered_serial_serial_descriptor *serial)
 * \brief Transmit a string with the specific huart in the serial descriptor. Strings larger than BUFFERED_SERIAL_BUFFERS_SIZE will be transmitted in blocks of that size.
 * \param string_descriptor Pointer to the descriptor of the string to transmit.
 * \param serial Pointer to the serial descriptor of the target huart.
 */
void buffered_serial_print_string(buffered_serial_serial_descriptor *serial,static_strings_string_descriptor *string_descriptor);

/** static_strings_string_descriptor *buffered_serial_read_line(buffered_serial_serial_descriptor *serial)
 * \brief Read a string in the specific huart buffer in the serial descriptor. String must have \\r\\n line ending.
 * \param serial Pointer to the serial descriptor of the target huart.
 * \return Pointer to the string descriptor of the line read (See library Static Strings), if NULL check buffered_serial_error_code.
 */
static_strings_string_descriptor *buffered_serial_read_line(buffered_serial_serial_descriptor *serial);

/** void buffered_serial_update_rx_buffer_data(UART_HandleTypeDef *huart)
 * \brief When IDLE line interruption is fired this function updates the rx buffer meta data.
 * \param Pointer to the huart IDLE line interruption source.
 */
void buffered_serial_update_rx_buffer_data(UART_HandleTypeDef *huart);

#endif
