################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../system_monitor/system_monitor_1769_001-1_c13a.c 

OBJS += \
./system_monitor/system_monitor_1769_001-1_c13a.o 

C_DEPS += \
./system_monitor/system_monitor_1769_001-1_c13a.d 


# Each subdirectory must supply rules for building sources it contributes
system_monitor/%.o: ../system_monitor/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -DDEBUG -D__USE_CMSIS=CMSISv2p00_LPC17xx -D__REDLIB__ -D__CODE_RED -I"D:\mjp_lpc1769_ttrds\CMSISv2p00_LPC17xx" -I"D:\mjp_lpc1769_ttrds\CMSISv2p00_LPC17xx\inc" -I"D:\mjp_lpc1769_ttrds\Lib_MCU" -I"D:\mjp_lpc1769_ttrds\Lib_MCU\inc" -I"D:\mjp_lpc1769_ttrds\RDB1768cmsis2_usbstack\inc" -O0 -g3 -Wall -Werror -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -mcpu=cortex-m3 -mthumb -D__REDLIB__ -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


