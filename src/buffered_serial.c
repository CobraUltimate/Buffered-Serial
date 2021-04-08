/** \file buffered_serial.c
 * \brief Serial communication based on a circular buffer, dma and huart with hal controls and Static Strings.
 */

#include "buffered_serial.h"

void buffered_serial_init(UART_HandleTypeDef **huarts){
	static_strings_init();
	int i;
	for(i = 0;i < BUFFERED_SERIAL_SERIALS_QUANTITY;i++){
		buffered_serial_serial_descriptors[i].huart = *(huarts + i);
		buffered_serial_serial_descriptors[i].rx_buffer = buffered_serial_rx_buffers[i];
		buffered_serial_serial_descriptors[i].rx_buffer_data_start = buffered_serial_rx_buffers[i];
		buffered_serial_serial_descriptors[i].rx_buffer_data_finish = buffered_serial_rx_buffers[i];
		buffered_serial_serial_descriptors[i].tx_buffer = buffered_serial_tx_buffers[i];
		__HAL_UART_ENABLE_IT(buffered_serial_serial_descriptors[i].huart, UART_IT_IDLE);
		HAL_UART_Receive_DMA(buffered_serial_serial_descriptors[i].huart,buffered_serial_serial_descriptors[i].rx_buffer,BUFFERED_SERIAL_BUFFERS_SIZE);
	}
}

buffered_serial_serial_descriptor *buffered_serial_get_huart_serial_descriptor(UART_HandleTypeDef *huart){
	int i;
	for(i = 0; i < BUFFERED_SERIAL_SERIALS_QUANTITY;i++){
		if(buffered_serial_serial_descriptors[i].huart == huart){
			return &buffered_serial_serial_descriptors[i];
		}
	}
	return NULL;
}

uint16_t buffered_serial_available(buffered_serial_serial_descriptor *serial){
	uint8_t *local_rx_buffer_data_start = serial->rx_buffer_data_start;
	uint8_t *local_rx_buffer_data_finish = serial->rx_buffer_data_finish;
	if(local_rx_buffer_data_finish >= local_rx_buffer_data_start){
		return (uint16_t)(local_rx_buffer_data_finish - local_rx_buffer_data_start);
	}
	else{
		return ((uint16_t)((serial->rx_buffer + BUFFERED_SERIAL_BUFFERS_SIZE) - local_rx_buffer_data_start) + (uint16_t)(local_rx_buffer_data_finish - serial->rx_buffer));
	}
}

void buffered_serial_print_character(buffered_serial_serial_descriptor *serial,uint8_t character){
	while(serial->huart->gState == HAL_UART_STATE_BUSY_TX);
	HAL_UART_Transmit_DMA(serial->huart,&character,1);
}

void buffered_serial_print_string(buffered_serial_serial_descriptor *serial,static_strings_string_descriptor *string_descriptor){
	uint16_t remaining_data_to_send = string_descriptor->length;
	while(remaining_data_to_send > 0){
		while(serial->huart->gState == HAL_UART_STATE_BUSY_TX);
		if(remaining_data_to_send < BUFFERED_SERIAL_BUFFERS_SIZE){
			memcpy(serial->tx_buffer,string_descriptor->string + (string_descriptor->length - remaining_data_to_send),remaining_data_to_send);
			HAL_UART_Transmit_DMA(serial->huart,serial->tx_buffer,remaining_data_to_send);
			remaining_data_to_send = 0;
		}
		else{
			memcpy(serial->tx_buffer,string_descriptor->string + (string_descriptor->length - remaining_data_to_send),BUFFERED_SERIAL_BUFFERS_SIZE);
			HAL_UART_Transmit_DMA(serial->huart,serial->tx_buffer,BUFFERED_SERIAL_BUFFERS_SIZE);
			remaining_data_to_send -= BUFFERED_SERIAL_BUFFERS_SIZE;
		}
	}
}

int buffered_serial_print_line(buffered_serial_serial_descriptor *serial,static_strings_string_descriptor *string_descriptor){
	if(string_descriptor == NULL)
		return 1;
	static_strings_string_descriptor *line = static_strings_concatenate(string_descriptor,static_strings_new_line);
	if(line == NULL)
		return 1;
	buffered_serial_print_string(serial,line);
	static_strings_deallocate(line);
	return 0;
}

static_strings_string_descriptor *buffered_serial_read_line(buffered_serial_serial_descriptor *serial){
	uint16_t i;
	uint16_t availaible_data;
	uint8_t *serial_data;
	for(i=0,availaible_data = buffered_serial_available(serial),serial_data = serial->rx_buffer_data_start;i < availaible_data;i++,serial_data++){
		if(serial_data - serial->rx_buffer == BUFFERED_SERIAL_BUFFERS_SIZE){
			serial_data = serial->rx_buffer;
		}
		if(*serial_data == '\n'){
			uint8_t *serial_data_previous;
			if(serial_data == serial->rx_buffer){
				serial_data_previous = serial->rx_buffer + BUFFERED_SERIAL_BUFFERS_SIZE - 1;
			}
			else{
				serial_data_previous = serial_data - 1;
			}
			if(*serial_data_previous == '\r'){
				static_strings_string_descriptor *string_descriptor = static_strings_allocate(i + 1);
				if(string_descriptor == NULL){
					buffered_serial_error_code = BUFFERED_SERIAL_ERROR_CODE_STATIC_STRINGS_ERROR;
					return string_descriptor;
				}
				string_descriptor->length = i + 1;
				if(serial_data >= serial->rx_buffer_data_start){
					memcpy(string_descriptor->string,serial->rx_buffer_data_start, i + 1);
				}
				else{
					uint16_t data_at_buffer_end = (serial->rx_buffer + BUFFERED_SERIAL_BUFFERS_SIZE) - serial->rx_buffer_data_start;
					memcpy(string_descriptor->string,serial->rx_buffer_data_start,data_at_buffer_end);
					memcpy(string_descriptor->string + data_at_buffer_end,serial->rx_buffer,i - data_at_buffer_end + 1);
				}
				if(serial_data == (serial->rx_buffer + BUFFERED_SERIAL_BUFFERS_SIZE) - 1){
					serial->rx_buffer_data_start = serial->rx_buffer;
				}
				else{
					serial->rx_buffer_data_start = serial_data + 1;
				}
				return string_descriptor;
			}
		}
	}
	buffered_serial_error_code = BUFFERED_SERIAL_ERROR_CODE_NO_LINE_ENDING_DETECTED;
	return NULL;
}

void buffered_serial_update_rx_buffer_data(UART_HandleTypeDef *huart){
	if(__HAL_UART_GET_FLAG(huart, UART_FLAG_IDLE) != RESET){
		__HAL_UART_CLEAR_IDLEFLAG(huart);
		int i;
		for(i = 0; i < BUFFERED_SERIAL_SERIALS_QUANTITY;i++){
			if(buffered_serial_serial_descriptors[i].huart == huart){
				buffered_serial_serial_descriptors[i].rx_buffer_data_finish = buffered_serial_serial_descriptors[i].rx_buffer + (BUFFERED_SERIAL_BUFFERS_SIZE - __HAL_DMA_GET_COUNTER(huart->hdmarx));
			}
		}
	}
}

void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
{
	HAL_UART_Receive_DMA(huart,buffered_serial_get_huart_serial_descriptor(huart)->rx_buffer,BUFFERED_SERIAL_BUFFERS_SIZE);
}
