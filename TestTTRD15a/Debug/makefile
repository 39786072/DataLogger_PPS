################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include src/watchdog/subdir.mk
-include src/tasks/subdir.mk
-include src/task_support_fns/subdir.mk
-include src/system_test/subdir.mk
-include src/system_state/subdir.mk
-include src/system_mode/subdir.mk
-include src/system/subdir.mk
-include src/scheduler/subdir.mk
-include src/resource_barriers_light/subdir.mk
-include src/predicttor/subdir.mk
-include src/monittor/subdir.mk
-include src/main/subdir.mk
-include src/heartbeat/subdir.mk
-include src/arm-nxp/subdir.mk
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
all: TestTTRD15a.axf

# Tool invocations
TestTTRD15a.axf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: MCU Linker'
	arm-none-eabi-gcc -nostdlib -L"D:\Santiago\Universidad\PPS\Workspaces\Pruebas\CMSIS_CORE_LPC17xx\Debug" -Xlinker -Map="TestTTRD15a.map" -Xlinker --gc-sections -Xlinker -print-memory-usage -mcpu=cortex-m3 -mthumb -T "TestTTRD15a_Debug.ld" -o "TestTTRD15a.axf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '
	$(MAKE) --no-print-directory post-build

# Other Targets
clean:
	-$(RM) $(EXECUTABLES)$(OBJS)$(C_DEPS) TestTTRD15a.axf
	-@echo ' '

post-build:
	-@echo 'Performing post-build steps'
	-arm-none-eabi-size "TestTTRD15a.axf"; # arm-none-eabi-objcopy -v -O binary "TestTTRD15a.axf" "TestTTRD15a.bin" ; # checksum -p LPC1769 -d "TestTTRD15a.bin";
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY: post-build

-include ../makefile.targets
