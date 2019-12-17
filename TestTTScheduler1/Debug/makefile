################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include src/tasks/subdir.mk
-include src/system/subdir.mk
-include src/scheduler/subdir.mk
-include src/main/subdir.mk
-include src/subdir.mk
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
all: TestTTScheduler1.axf

# Tool invocations
TestTTScheduler1.axf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: MCU Linker'
	arm-none-eabi-gcc -nostdlib -L"D:\Santiago\Universidad\PPS\Workspaces\Pruebas\CMSIS_CORE_LPC17xx\Debug" -Xlinker -Map="TestTTScheduler1.map" -Xlinker --gc-sections -Xlinker -print-memory-usage -mcpu=cortex-m3 -mthumb -T "TestTTScheduler1_Debug.ld" -o "TestTTScheduler1.axf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '
	$(MAKE) --no-print-directory post-build

# Other Targets
clean:
	-$(RM) $(EXECUTABLES)$(OBJS)$(C_DEPS) TestTTScheduler1.axf
	-@echo ' '

post-build:
	-@echo 'Performing post-build steps'
	-arm-none-eabi-size "TestTTScheduler1.axf"; # arm-none-eabi-objcopy -v -O binary "TestTTScheduler1.axf" "TestTTScheduler1.bin" ; # checksum -p LPC1769 -d "TestTTScheduler1.bin";
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY: post-build

-include ../makefile.targets
