################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../system_tests/system_test_mcu_temperature_1769_003-0_c13a.c 

OBJS += \
./system_tests/system_test_mcu_temperature_1769_003-0_c13a.o 

C_DEPS += \
./system_tests/system_test_mcu_temperature_1769_003-0_c13a.d 


# Each subdirectory must supply rules for building sources it contributes
system_tests/%.o: ../system_tests/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -DDEBUG -D__USE_CMSIS=CMSISv2p00_LPC17xx -D__REDLIB__ -D__CODE_RED -O0 -g3 -Wall -Werror -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -mcpu=cortex-m3 -mthumb -D__REDLIB__ -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


