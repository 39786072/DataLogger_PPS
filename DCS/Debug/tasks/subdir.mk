################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../tasks/heartbeat_1769_003-0_c02a.c \
../tasks/led.c \
../tasks/ptt.c \
../tasks/watchdog_1769_003-0_c02a.c \
../tasks/weather_station.c \
../tasks/wifi_esp8266.c 

OBJS += \
./tasks/heartbeat_1769_003-0_c02a.o \
./tasks/led.o \
./tasks/ptt.o \
./tasks/watchdog_1769_003-0_c02a.o \
./tasks/weather_station.o \
./tasks/wifi_esp8266.o 

C_DEPS += \
./tasks/heartbeat_1769_003-0_c02a.d \
./tasks/led.d \
./tasks/ptt.d \
./tasks/watchdog_1769_003-0_c02a.d \
./tasks/weather_station.d \
./tasks/wifi_esp8266.d 


# Each subdirectory must supply rules for building sources it contributes
tasks/%.o: ../tasks/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -DDEBUG -D__CODE_RED -DCORE_M3 -D__USE_CMSIS=CMSIS_CORE_LPC17xx -DCR_INTEGER_PRINTF -DCR_PRINTF_CHAR -D__LPC17XX__ -D__REDLIB__ -I"D:\Santiago\Universidad\PPS\Workspaces\Pruebas\CMSIS_CORE_LPC17xx\inc" -O0 -fno-common -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -mcpu=cortex-m3 -mthumb -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


