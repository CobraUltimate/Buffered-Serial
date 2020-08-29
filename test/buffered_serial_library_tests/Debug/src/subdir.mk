################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
C:/Users/carro/STM32CubeIDE/workspace_1.4.0/buffered_serial/src/buffered_serial.c 

OBJS += \
./src/buffered_serial.o 

C_DEPS += \
./src/buffered_serial.d 


# Each subdirectory must supply rules for building sources it contributes
src/buffered_serial.o: C:/Users/carro/STM32CubeIDE/workspace_1.4.0/buffered_serial/src/buffered_serial.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F103xB -DDEBUG -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/carro/STM32CubeIDE/workspace_1.4.0/buffered_serial/test/buffered_serial_library_tests/../../lib/static_strings/inc" -I"C:/Users/carro/STM32CubeIDE/workspace_1.4.0/buffered_serial/test/buffered_serial_library_tests/../../inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"src/buffered_serial.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

