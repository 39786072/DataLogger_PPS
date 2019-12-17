################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../measure_jitter/measure_jitter_t2_1769_001-0_c12a.c 

OBJS += \
./measure_jitter/measure_jitter_t2_1769_001-0_c12a.o 

C_DEPS += \
./measure_jitter/measure_jitter_t2_1769_001-0_c12a.d 


# Each subdirectory must supply rules for building sources it contributes
measure_jitter/%.o: ../measure_jitter/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -DDEBUG -D__USE_CMSIS=CMSISv2p00_LPC17xx -D__REDLIB__ -D__CODE_RED -I"D:\mjp_lpcx_projects3\CMSISv2p00_LPC17xx" -I"D:\mjp_lpcx_projects3\CMSISv2p00_LPC17xx\inc" -I"D:\mjp_lpcx_projects3\Lib_MCU" -I"D:\mjp_lpcx_projects3\Lib_MCU\inc" -I"D:\mjp_lpcx_projects3\RDB1768cmsis2_usbstack\inc" -O0 -g3 -Wall -Werror -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -mcpu=cortex-m3 -mthumb -D__REDLIB__ -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


