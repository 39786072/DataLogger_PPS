################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/arm-nxp/cr_startup_lpc176x.c \
../src/arm-nxp/lpc17xx_adc.c \
../src/arm-nxp/lpc17xx_can.c \
../src/arm-nxp/lpc17xx_clkpwr.c \
../src/arm-nxp/lpc17xx_dac.c \
../src/arm-nxp/lpc17xx_emac.c \
../src/arm-nxp/lpc17xx_exti.c \
../src/arm-nxp/lpc17xx_gpdma.c \
../src/arm-nxp/lpc17xx_gpio.c \
../src/arm-nxp/lpc17xx_i2c.c \
../src/arm-nxp/lpc17xx_i2s.c \
../src/arm-nxp/lpc17xx_iap.c \
../src/arm-nxp/lpc17xx_libcfg_default.c \
../src/arm-nxp/lpc17xx_mcpwm.c \
../src/arm-nxp/lpc17xx_nvic.c \
../src/arm-nxp/lpc17xx_pinsel.c \
../src/arm-nxp/lpc17xx_pwm.c \
../src/arm-nxp/lpc17xx_qei.c \
../src/arm-nxp/lpc17xx_rit.c \
../src/arm-nxp/lpc17xx_rtc.c \
../src/arm-nxp/lpc17xx_spi.c \
../src/arm-nxp/lpc17xx_ssp.c \
../src/arm-nxp/lpc17xx_systick.c \
../src/arm-nxp/lpc17xx_timer.c \
../src/arm-nxp/lpc17xx_uart.c \
../src/arm-nxp/lpc17xx_wdt.c \
../src/arm-nxp/usbcontrol.c \
../src/arm-nxp/usbhw_lpc.c \
../src/arm-nxp/usbinit.c \
../src/arm-nxp/usbstdreq.c 

OBJS += \
./src/arm-nxp/cr_startup_lpc176x.o \
./src/arm-nxp/lpc17xx_adc.o \
./src/arm-nxp/lpc17xx_can.o \
./src/arm-nxp/lpc17xx_clkpwr.o \
./src/arm-nxp/lpc17xx_dac.o \
./src/arm-nxp/lpc17xx_emac.o \
./src/arm-nxp/lpc17xx_exti.o \
./src/arm-nxp/lpc17xx_gpdma.o \
./src/arm-nxp/lpc17xx_gpio.o \
./src/arm-nxp/lpc17xx_i2c.o \
./src/arm-nxp/lpc17xx_i2s.o \
./src/arm-nxp/lpc17xx_iap.o \
./src/arm-nxp/lpc17xx_libcfg_default.o \
./src/arm-nxp/lpc17xx_mcpwm.o \
./src/arm-nxp/lpc17xx_nvic.o \
./src/arm-nxp/lpc17xx_pinsel.o \
./src/arm-nxp/lpc17xx_pwm.o \
./src/arm-nxp/lpc17xx_qei.o \
./src/arm-nxp/lpc17xx_rit.o \
./src/arm-nxp/lpc17xx_rtc.o \
./src/arm-nxp/lpc17xx_spi.o \
./src/arm-nxp/lpc17xx_ssp.o \
./src/arm-nxp/lpc17xx_systick.o \
./src/arm-nxp/lpc17xx_timer.o \
./src/arm-nxp/lpc17xx_uart.o \
./src/arm-nxp/lpc17xx_wdt.o \
./src/arm-nxp/usbcontrol.o \
./src/arm-nxp/usbhw_lpc.o \
./src/arm-nxp/usbinit.o \
./src/arm-nxp/usbstdreq.o 

C_DEPS += \
./src/arm-nxp/cr_startup_lpc176x.d \
./src/arm-nxp/lpc17xx_adc.d \
./src/arm-nxp/lpc17xx_can.d \
./src/arm-nxp/lpc17xx_clkpwr.d \
./src/arm-nxp/lpc17xx_dac.d \
./src/arm-nxp/lpc17xx_emac.d \
./src/arm-nxp/lpc17xx_exti.d \
./src/arm-nxp/lpc17xx_gpdma.d \
./src/arm-nxp/lpc17xx_gpio.d \
./src/arm-nxp/lpc17xx_i2c.d \
./src/arm-nxp/lpc17xx_i2s.d \
./src/arm-nxp/lpc17xx_iap.d \
./src/arm-nxp/lpc17xx_libcfg_default.d \
./src/arm-nxp/lpc17xx_mcpwm.d \
./src/arm-nxp/lpc17xx_nvic.d \
./src/arm-nxp/lpc17xx_pinsel.d \
./src/arm-nxp/lpc17xx_pwm.d \
./src/arm-nxp/lpc17xx_qei.d \
./src/arm-nxp/lpc17xx_rit.d \
./src/arm-nxp/lpc17xx_rtc.d \
./src/arm-nxp/lpc17xx_spi.d \
./src/arm-nxp/lpc17xx_ssp.d \
./src/arm-nxp/lpc17xx_systick.d \
./src/arm-nxp/lpc17xx_timer.d \
./src/arm-nxp/lpc17xx_uart.d \
./src/arm-nxp/lpc17xx_wdt.d \
./src/arm-nxp/usbcontrol.d \
./src/arm-nxp/usbhw_lpc.d \
./src/arm-nxp/usbinit.d \
./src/arm-nxp/usbstdreq.d 


# Each subdirectory must supply rules for building sources it contributes
src/arm-nxp/%.o: ../src/arm-nxp/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -DDEBUG -D__CODE_RED -DCORE_M3 -D__USE_CMSIS=CMSIS_CORE_LPC17xx -D__LPC17XX__ -D__REDLIB__ -I"D:\Santiago\Universidad\PPS\Workspaces\Pruebas\TestTTRD13a\inc" -I"D:\Santiago\Universidad\PPS\Workspaces\Pruebas\CMSIS_CORE_LPC17xx\inc" -O0 -fno-common -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -mcpu=cortex-m3 -mthumb -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


