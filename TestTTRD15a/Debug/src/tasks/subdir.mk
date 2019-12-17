################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/tasks/detergent_hatch_1769_003-0_c15a.c \
../src/tasks/door_lock_1769_003-0_c15a.c \
../src/tasks/door_sensor_1769_003-0_c15a.c \
../src/tasks/drum_motor_1769_003-0_c15a.c \
../src/tasks/drum_sensor_1769_003-0_c15a.c \
../src/tasks/selector_dial_1769_003-0_c15a.c \
../src/tasks/start_switch_1769_003-0_c15a.c \
../src/tasks/water_heater_1769_003-0_c15a.c \
../src/tasks/water_level_1769_003-0_c15a.c \
../src/tasks/water_pump_1769_003-0_c15a.c \
../src/tasks/water_temperature_1769_003-0_c15a.c \
../src/tasks/water_valve_1769_003-0_c15a.c 

OBJS += \
./src/tasks/detergent_hatch_1769_003-0_c15a.o \
./src/tasks/door_lock_1769_003-0_c15a.o \
./src/tasks/door_sensor_1769_003-0_c15a.o \
./src/tasks/drum_motor_1769_003-0_c15a.o \
./src/tasks/drum_sensor_1769_003-0_c15a.o \
./src/tasks/selector_dial_1769_003-0_c15a.o \
./src/tasks/start_switch_1769_003-0_c15a.o \
./src/tasks/water_heater_1769_003-0_c15a.o \
./src/tasks/water_level_1769_003-0_c15a.o \
./src/tasks/water_pump_1769_003-0_c15a.o \
./src/tasks/water_temperature_1769_003-0_c15a.o \
./src/tasks/water_valve_1769_003-0_c15a.o 

C_DEPS += \
./src/tasks/detergent_hatch_1769_003-0_c15a.d \
./src/tasks/door_lock_1769_003-0_c15a.d \
./src/tasks/door_sensor_1769_003-0_c15a.d \
./src/tasks/drum_motor_1769_003-0_c15a.d \
./src/tasks/drum_sensor_1769_003-0_c15a.d \
./src/tasks/selector_dial_1769_003-0_c15a.d \
./src/tasks/start_switch_1769_003-0_c15a.d \
./src/tasks/water_heater_1769_003-0_c15a.d \
./src/tasks/water_level_1769_003-0_c15a.d \
./src/tasks/water_pump_1769_003-0_c15a.d \
./src/tasks/water_temperature_1769_003-0_c15a.d \
./src/tasks/water_valve_1769_003-0_c15a.d 


# Each subdirectory must supply rules for building sources it contributes
src/tasks/%.o: ../src/tasks/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -DDEBUG -D__CODE_RED -DCORE_M3 -D__USE_CMSIS=CMSIS_CORE_LPC17xx -D__LPC17XX__ -D__REDLIB__ -I"D:\Santiago\Universidad\PPS\Workspaces\Pruebas\TestTTRD15a\inc" -I"D:\Santiago\Universidad\PPS\Workspaces\Pruebas\CMSIS_CORE_LPC17xx\inc" -O0 -fno-common -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -mcpu=cortex-m3 -mthumb -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


