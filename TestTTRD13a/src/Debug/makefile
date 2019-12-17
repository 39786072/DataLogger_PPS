################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include tasks/subdir.mk
-include system_tests/subdir.mk
-include system_mode/subdir.mk
-include scheduler/subdir.mk
-include main/subdir.mk
-include bertrik_sikken/subdir.mk
-include arm-nxp/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: ttrd13a_lpc176x_ttc_usb_check_system_temperature.axf

# Tool invocations
ttrd13a_lpc176x_ttc_usb_check_system_temperature.axf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: MCU Linker'
	arm-none-eabi-gcc -nostdlib -Xlinker -Map=ttrd13a_lpc176x_ttc_usb_check_system_temperature.map -Xlinker --gc-sections -mcpu=cortex-m3 -mthumb -T "ttrd13a_lpc176x_ttc_usb_check_system_temperature_Debug.ld" -o "ttrd13a_lpc176x_ttc_usb_check_system_temperature.axf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '
	$(MAKE) --no-print-directory post-build

# Other Targets
clean:
	-$(RM) $(EXECUTABLES)$(OBJS)$(C_DEPS) ttrd13a_lpc176x_ttc_usb_check_system_temperature.axf
	-@echo ' '

post-build:
	-@echo 'Performing post-build steps'
	-arm-none-eabi-size ttrd13a_lpc176x_ttc_usb_check_system_temperature.axf; # arm-none-eabi-objcopy -O binary ttrd13a_lpc176x_ttc_usb_check_system_temperature.axf ttrd13a_lpc176x_ttc_usb_check_system_temperature.bin ; checksum -d ttrd13a_lpc176x_ttc_usb_check_system_temperature.bin;
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY: post-build

-include ../makefile.targets
