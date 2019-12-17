################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../task_support_fns/report_number_7seg_1769_001-0_c03a.c 

OBJS += \
./task_support_fns/report_number_7seg_1769_001-0_c03a.o 

C_DEPS += \
./task_support_fns/report_number_7seg_1769_001-0_c03a.d 


# Each subdirectory must supply rules for building sources it contributes
task_support_fns/%.o: ../task_support_fns/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -DDEBUG -D__USE_CMSIS=CMSISv2p00_LPC17xx -D__REDLIB__ -D__CODE_RED -I"C:\mjp_lpcx_projects2\CMSISv2p00_LPC17xx" -I"C:\mjp_lpcx_projects2\CMSISv2p00_LPC17xx\inc" -I"C:\mjp_lpcx_projects2\Lib_MCU" -I"C:\mjp_lpcx_projects2\Lib_MCU\inc" -I"C:\mjp_lpcx_projects2\RDB1768cmsis2_usbstack\inc" -O0 -g3 -Wall -Werror -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -mcpu=cortex-m3 -mthumb -D__REDLIB__ -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


