################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/system_test/system_self_test_bist_1769_003-0_c15a.c \
../src/system_test/system_self_test_configuration_1769_003-0_c15a.c \
../src/system_test/system_self_test_mcu_temperature_1769_003-0_c15a.c \
../src/system_test/system_self_test_post_1769_003-0_c15a.c 

OBJS += \
./src/system_test/system_self_test_bist_1769_003-0_c15a.o \
./src/system_test/system_self_test_configuration_1769_003-0_c15a.o \
./src/system_test/system_self_test_mcu_temperature_1769_003-0_c15a.o \
./src/system_test/system_self_test_post_1769_003-0_c15a.o 

C_DEPS += \
./src/system_test/system_self_test_bist_1769_003-0_c15a.d \
./src/system_test/system_self_test_configuration_1769_003-0_c15a.d \
./src/system_test/system_self_test_mcu_temperature_1769_003-0_c15a.d \
./src/system_test/system_self_test_post_1769_003-0_c15a.d 


# Each subdirectory must supply rules for building sources it contributes
src/system_test/%.o: ../src/system_test/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -DDEBUG -D__CODE_RED -DCORE_M3 -D__USE_CMSIS=CMSIS_CORE_LPC17xx -D__LPC17XX__ -D__REDLIB__ -I"D:\Santiago\Universidad\PPS\Workspaces\Pruebas\TestTTRD15a\inc" -I"D:\Santiago\Universidad\PPS\Workspaces\Pruebas\CMSIS_CORE_LPC17xx\inc" -O0 -fno-common -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -mcpu=cortex-m3 -mthumb -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


