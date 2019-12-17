################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include tasks/subdir.mk
-include system/subdir.mk
-include scheduler/subdir.mk
-include main/subdir.mk
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
all: ttrd02c_lpc176x_ttc_sch_timer_0.axf

# Tool invocations
ttrd02c_lpc176x_ttc_sch_timer_0.axf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: MCU Linker'
	arm-none-eabi-gcc -nostdlib -Xlinker -Map=ttrd02c_lpc176x_ttc_sch_timer_0.map -Xlinker --gc-sections -mcpu=cortex-m3 -mthumb -T "ttrd02d_lpc176x_ttc_sch_timer_0_Debug.ld" -o "ttrd02c_lpc176x_ttc_sch_timer_0.axf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '
	$(MAKE) --no-print-directory post-build

# Other Targets
clean:
	-$(RM) $(EXECUTABLES)$(OBJS)$(C_DEPS) ttrd02c_lpc176x_ttc_sch_timer_0.axf
	-@echo ' '

post-build:
	-@echo 'Performing post-build steps'
	-arm-none-eabi-size ttrd02c_lpc176x_ttc_sch_timer_0.axf; # arm-none-eabi-objcopy -O binary ttrd02c_lpc176x_ttc_sch_timer_0.axf ttrd02c_lpc176x_ttc_sch_timer_0.bin ; checksum -d ttrd02c_lpc176x_ttc_sch_timer_0.bin;
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY: post-build

-include ../makefile.targets
