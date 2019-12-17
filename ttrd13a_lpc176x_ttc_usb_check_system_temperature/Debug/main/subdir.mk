################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../main/lpc17xx_adc.c \
../main/lpc17xx_can.c \
../main/lpc17xx_clkpwr.c \
../main/lpc17xx_dac.c \
../main/lpc17xx_emac.c \
../main/lpc17xx_exti.c \
../main/lpc17xx_gpdma.c \
../main/lpc17xx_gpio.c \
../main/lpc17xx_i2c.c \
../main/lpc17xx_i2s.c \
../main/lpc17xx_iap.c \
../main/lpc17xx_libcfg_default.c \
../main/lpc17xx_mcpwm.c \
../main/lpc17xx_nvic.c \
../main/lpc17xx_pinsel.c \
../main/lpc17xx_pwm.c \
../main/lpc17xx_qei.c \
../main/lpc17xx_rit.c \
../main/lpc17xx_rtc.c \
../main/lpc17xx_spi.c \
../main/lpc17xx_ssp.c \
../main/lpc17xx_systick.c \
../main/lpc17xx_timer.c \
../main/lpc17xx_uart.c \
../main/lpc17xx_wdt.c \
../main/main.c 

OBJS += \
./main/lpc17xx_adc.o \
./main/lpc17xx_can.o \
./main/lpc17xx_clkpwr.o \
./main/lpc17xx_dac.o \
./main/lpc17xx_emac.o \
./main/lpc17xx_exti.o \
./main/lpc17xx_gpdma.o \
./main/lpc17xx_gpio.o \
./main/lpc17xx_i2c.o \
./main/lpc17xx_i2s.o \
./main/lpc17xx_iap.o \
./main/lpc17xx_libcfg_default.o \
./main/lpc17xx_mcpwm.o \
./main/lpc17xx_nvic.o \
./main/lpc17xx_pinsel.o \
./main/lpc17xx_pwm.o \
./main/lpc17xx_qei.o \
./main/lpc17xx_rit.o \
./main/lpc17xx_rtc.o \
./main/lpc17xx_spi.o \
./main/lpc17xx_ssp.o \
./main/lpc17xx_systick.o \
./main/lpc17xx_timer.o \
./main/lpc17xx_uart.o \
./main/lpc17xx_wdt.o \
./main/main.o 

C_DEPS += \
./main/lpc17xx_adc.d \
./main/lpc17xx_can.d \
./main/lpc17xx_clkpwr.d \
./main/lpc17xx_dac.d \
./main/lpc17xx_emac.d \
./main/lpc17xx_exti.d \
./main/lpc17xx_gpdma.d \
./main/lpc17xx_gpio.d \
./main/lpc17xx_i2c.d \
./main/lpc17xx_i2s.d \
./main/lpc17xx_iap.d \
./main/lpc17xx_libcfg_default.d \
./main/lpc17xx_mcpwm.d \
./main/lpc17xx_nvic.d \
./main/lpc17xx_pinsel.d \
./main/lpc17xx_pwm.d \
./main/lpc17xx_qei.d \
./main/lpc17xx_rit.d \
./main/lpc17xx_rtc.d \
./main/lpc17xx_spi.d \
./main/lpc17xx_ssp.d \
./main/lpc17xx_systick.d \
./main/lpc17xx_timer.d \
./main/lpc17xx_uart.d \
./main/lpc17xx_wdt.d \
./main/main.d 


# Each subdirectory must supply rules for building sources it contributes
main/%.o: ../main/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -DDEBUG -D__USE_CMSIS=CMSISv2p00_LPC17xx -D__REDLIB__ -D__CODE_RED -O0 -g3 -Wall -Werror -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -mcpu=cortex-m3 -mthumb -D__REDLIB__ -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


