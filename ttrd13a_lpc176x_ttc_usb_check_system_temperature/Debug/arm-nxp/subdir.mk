################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../arm-nxp/cr_startup_lpc176x.c \
../arm-nxp/lpc17xx_adc.c \
../arm-nxp/lpc17xx_can.c \
../arm-nxp/lpc17xx_clkpwr.c \
../arm-nxp/lpc17xx_dac.c \
../arm-nxp/lpc17xx_emac.c \
../arm-nxp/lpc17xx_exti.c \
../arm-nxp/lpc17xx_gpdma.c \
../arm-nxp/lpc17xx_gpio.c \
../arm-nxp/lpc17xx_i2c.c \
../arm-nxp/lpc17xx_i2s.c \
../arm-nxp/lpc17xx_iap.c \
../arm-nxp/lpc17xx_libcfg_default.c \
../arm-nxp/lpc17xx_mcpwm.c \
../arm-nxp/lpc17xx_nvic.c \
../arm-nxp/lpc17xx_pinsel.c \
../arm-nxp/lpc17xx_pwm.c \
../arm-nxp/lpc17xx_qei.c \
../arm-nxp/lpc17xx_rit.c \
../arm-nxp/lpc17xx_rtc.c \
../arm-nxp/lpc17xx_spi.c \
../arm-nxp/lpc17xx_ssp.c \
../arm-nxp/lpc17xx_systick.c \
../arm-nxp/lpc17xx_timer.c \
../arm-nxp/lpc17xx_uart.c \
../arm-nxp/lpc17xx_wdt.c 

OBJS += \
./arm-nxp/cr_startup_lpc176x.o \
./arm-nxp/lpc17xx_adc.o \
./arm-nxp/lpc17xx_can.o \
./arm-nxp/lpc17xx_clkpwr.o \
./arm-nxp/lpc17xx_dac.o \
./arm-nxp/lpc17xx_emac.o \
./arm-nxp/lpc17xx_exti.o \
./arm-nxp/lpc17xx_gpdma.o \
./arm-nxp/lpc17xx_gpio.o \
./arm-nxp/lpc17xx_i2c.o \
./arm-nxp/lpc17xx_i2s.o \
./arm-nxp/lpc17xx_iap.o \
./arm-nxp/lpc17xx_libcfg_default.o \
./arm-nxp/lpc17xx_mcpwm.o \
./arm-nxp/lpc17xx_nvic.o \
./arm-nxp/lpc17xx_pinsel.o \
./arm-nxp/lpc17xx_pwm.o \
./arm-nxp/lpc17xx_qei.o \
./arm-nxp/lpc17xx_rit.o \
./arm-nxp/lpc17xx_rtc.o \
./arm-nxp/lpc17xx_spi.o \
./arm-nxp/lpc17xx_ssp.o \
./arm-nxp/lpc17xx_systick.o \
./arm-nxp/lpc17xx_timer.o \
./arm-nxp/lpc17xx_uart.o \
./arm-nxp/lpc17xx_wdt.o 

C_DEPS += \
./arm-nxp/cr_startup_lpc176x.d \
./arm-nxp/lpc17xx_adc.d \
./arm-nxp/lpc17xx_can.d \
./arm-nxp/lpc17xx_clkpwr.d \
./arm-nxp/lpc17xx_dac.d \
./arm-nxp/lpc17xx_emac.d \
./arm-nxp/lpc17xx_exti.d \
./arm-nxp/lpc17xx_gpdma.d \
./arm-nxp/lpc17xx_gpio.d \
./arm-nxp/lpc17xx_i2c.d \
./arm-nxp/lpc17xx_i2s.d \
./arm-nxp/lpc17xx_iap.d \
./arm-nxp/lpc17xx_libcfg_default.d \
./arm-nxp/lpc17xx_mcpwm.d \
./arm-nxp/lpc17xx_nvic.d \
./arm-nxp/lpc17xx_pinsel.d \
./arm-nxp/lpc17xx_pwm.d \
./arm-nxp/lpc17xx_qei.d \
./arm-nxp/lpc17xx_rit.d \
./arm-nxp/lpc17xx_rtc.d \
./arm-nxp/lpc17xx_spi.d \
./arm-nxp/lpc17xx_ssp.d \
./arm-nxp/lpc17xx_systick.d \
./arm-nxp/lpc17xx_timer.d \
./arm-nxp/lpc17xx_uart.d \
./arm-nxp/lpc17xx_wdt.d 


# Each subdirectory must supply rules for building sources it contributes
arm-nxp/%.o: ../arm-nxp/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -DDEBUG -D__USE_CMSIS=CMSISv2p00_LPC17xx -D__REDLIB__ -D__CODE_RED -O0 -g3 -Wall -Werror -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -mcpu=cortex-m3 -mthumb -D__REDLIB__ -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


