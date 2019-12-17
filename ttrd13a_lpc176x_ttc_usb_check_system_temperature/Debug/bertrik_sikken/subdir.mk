################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../bertrik_sikken/lpcusb.c \
../bertrik_sikken/serial_fifo.c 

OBJS += \
./bertrik_sikken/lpcusb.o \
./bertrik_sikken/serial_fifo.o 

C_DEPS += \
./bertrik_sikken/lpcusb.d \
./bertrik_sikken/serial_fifo.d 


# Each subdirectory must supply rules for building sources it contributes
bertrik_sikken/%.o: ../bertrik_sikken/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -DDEBUG -D__USE_CMSIS=CMSISv2p00_LPC17xx -D__REDLIB__ -D__CODE_RED -O0 -g3 -Wall -Werror -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -mcpu=cortex-m3 -mthumb -D__REDLIB__ -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


