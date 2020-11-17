################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include src/subdir.mk
-include lib/static_strings/src/subdir.mk
-include Drivers/STM32F1xx_HAL_Driver/Src/subdir.mk
-include Core/Startup/subdir.mk
-include Core/Src/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
EXECUTABLES += \
buffered_serial_library_tests.elf \

SIZE_OUTPUT += \
default.size.stdout \

OBJDUMP_LIST += \
buffered_serial_library_tests.list \

OBJCOPY_BIN += \
buffered_serial_library_tests.bin \


# All Target
all: buffered_serial_library_tests.elf secondary-outputs

# Tool invocations
buffered_serial_library_tests.elf: $(OBJS) $(USER_OBJS) C:\Users\carro\STM32CubeIDE\workspace_1.4.0\buffered_serial\test\buffered_serial_library_tests\STM32F103C8TX_FLASH.ld
	arm-none-eabi-gcc -o "buffered_serial_library_tests.elf" @"objects.list" $(USER_OBJS) $(LIBS) -mcpu=cortex-m3 -T"C:\Users\carro\STM32CubeIDE\workspace_1.4.0\buffered_serial\test\buffered_serial_library_tests\STM32F103C8TX_FLASH.ld" -Wl,-Map="buffered_serial_library_tests.map" -Wl,--gc-sections -static -u _printf_float --specs=nano.specs -mfloat-abi=soft -mthumb -Wl,--start-group -lc -lm -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

default.size.stdout: $(EXECUTABLES)
	arm-none-eabi-size  $(EXECUTABLES)
	@echo 'Finished building: $@'
	@echo ' '

buffered_serial_library_tests.list: $(EXECUTABLES)
	arm-none-eabi-objdump -h -S $(EXECUTABLES) > "buffered_serial_library_tests.list"
	@echo 'Finished building: $@'
	@echo ' '

buffered_serial_library_tests.bin: $(EXECUTABLES)
	arm-none-eabi-objcopy  -O binary $(EXECUTABLES) "buffered_serial_library_tests.bin"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) *
	-@echo ' '

secondary-outputs: $(SIZE_OUTPUT) $(OBJDUMP_LIST) $(OBJCOPY_BIN)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
