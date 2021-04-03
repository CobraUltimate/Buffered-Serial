Buffered Serial {#mainpage}
============

## Features:

 - Developed for the STM32F103.
 - Serial communication with DMA in circular mode and IDLE interrupt.
 - Configurable quantity of serials and size of rx and tx buffers.
 - Simple communication with print string, print character, print line and read line functions.
 - STM32CubeIDE project configuration guide.
 - Error handling with buffered_serial_error_code.
 - UART Error handling

## Considerations:

 - BUFFERED_SERIAL_SERIALS_QUANTITY must be configured to correspond the quantity of huart configured, by default is one.
 - BUFFERED_SERIAL_BUFFERS_SIZE at most can be maximum value of uint16_t, since buffered_serial_available is this type.
 - Buffers can hold at most BUFFERED_SERIAL_BUFFERS_SIZE - 1 data, because when rx_buffer_data_start and rx_buffer_data_finish pointers are equals it can be 0 data or maximum data but the library interpret as 0 data.

 # Getting Started

 ## UART Error handling in buffered_serial.c

 ```C
 void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
 {
   HAL_UART_Receive_DMA(huart,buffered_serial_get_huart_serial_descriptor(huart)->rx_buffer,BUFFERED_SERIAL_BUFFERS_SIZE);
 }
 ```

 ## Configure IDLE interrupt in stm32f1xx_it.c

 Configure project as described in file project_configuration.pdf in root folder.
 IDLE interrupt must be configured for all huart interrupt handlers.

 ```C
 void USART1_IRQHandler(void)
 {
   HAL_UART_IRQHandler(&huart1);
   buffered_serial_update_rx_buffer_data(&huart1);
 }
 ```

 ## Initializing library and getting serial descriptor in main.c file

 ```C
 MX_GPIO_Init();
 MX_DMA_Init();
 MX_USART1_UART_Init();
 UART_HandleTypeDef *huarts[] = {&huart1};
 buffered_serial_init(huarts);
 buffered_serial_serial_descriptor *serial1 = buffered_serial_get_huart_serial_descriptor(&huart1);
 ```

 ## Writing a string

 ```C
 uint8_t test[40] = "2A6V7W5NL5ZZC6AYE84NKZ6MVFMZ5DZSYD9TM3\r\n";
 static_strings_string_descriptor *string_descriptor = static_strings_save(test);
 buffered_serial_print_string(test,string_descriptor);
 static_strings_deallocate(string_descriptor);
 ```

 DON'T FORGET TO DEALLOCATE STRING AFTER USING.

 ## Writing a string as a line

 ```C
  static_strings_string_descriptor *print_line_test = static_strings_save((uint8_t *)"this is not a line");
  buffered_serial_print_line(serial1,print_line_test);
  static_strings_deallocate(print_line_test);
 ```

 DON'T FORGET TO DEALLOCATE STRING AFTER USING.

 ## Reading a line

 ```C
 if(buffered_serial_available(serial1) > 0){
   uint16_t available = buffered_serial_available(serial1);
   static_strings_string_descriptor *string_descriptor = buffered_serial_read_line(serial1);
   if(string_descriptor != NULL){
     buffered_serial_print_string(serial1,string_descriptor);
     static_strings_deallocate(string_descriptor);
   }
   else{
     handle_error(buffered_serial_error_code);
   }
 }
 ```

 DON'T FORGET TO DEALLOCATE STRING AFTER USING.

 ## Writing a character

 ```C
 uint8_t character = 'A';
 buffered_serial_print_character(serial1,character);
 ```

 ## Configure serials quantity and size of the buffers

 Just edit these constants in buffered_serial.h

 ```C
 #define BUFFERED_SERIAL_SERIALS_QUANTITY 1
 #define BUFFERED_SERIAL_BUFFERS_SIZE 500
 ```
