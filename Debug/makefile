################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include usb-framework/libusb_stm32/src/subdir.mk
-include usb-framework/subdir.mk
-include lwip-2.2.2/system/os/subdir.mk
-include lwip-2.2.2/src/netif/ppp/polarssl/subdir.mk
-include lwip-2.2.2/src/netif/ppp/subdir.mk
-include lwip-2.2.2/src/netif/subdir.mk
-include lwip-2.2.2/src/core/ipv6/subdir.mk
-include lwip-2.2.2/src/core/ipv4/subdir.mk
-include lwip-2.2.2/src/core/subdir.mk
-include lwip-2.2.2/src/apps/tftp/subdir.mk
-include lwip-2.2.2/src/apps/sntp/subdir.mk
-include lwip-2.2.2/src/apps/snmp/subdir.mk
-include lwip-2.2.2/src/apps/smtp/subdir.mk
-include lwip-2.2.2/src/apps/netbiosns/subdir.mk
-include lwip-2.2.2/src/apps/mqtt/subdir.mk
-include lwip-2.2.2/src/apps/mdns/subdir.mk
-include lwip-2.2.2/src/apps/lwiperf/subdir.mk
-include lwip-2.2.2/src/apps/http/subdir.mk
-include lwip-2.2.2/src/apps/altcp_tls/subdir.mk
-include lwip-2.2.2/src/api/subdir.mk
-include letter-shell-3.x/src/subdir.mk
-include letter-shell-3.x/extensions/shell_enhance/subdir.mk
-include letter-shell-3.x/extensions/log/subdir.mk
-include letter-shell-3.x/drivers/subdir.mk
-include freertos/portable/subdir.mk
-include freertos/subdir.mk
-include fatfs_r0.15b/source/subdir.mk
-include fatfs_r0.15b/drivers/subdir.mk
-include Drivers/STM32F7xx_HAL_Driver/Src/subdir.mk
-include Core/Startup/subdir.mk
-include Core/Src/subdir.mk
-include Bsp/subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

OPTIONAL_TOOL_DEPS := \
$(wildcard ../makefile.defs) \
$(wildcard ../makefile.init) \
$(wildcard ../makefile.targets) \


BUILD_ARTIFACT_NAME := stm32f767_freertos
BUILD_ARTIFACT_EXTENSION := elf
BUILD_ARTIFACT_PREFIX :=
BUILD_ARTIFACT := $(BUILD_ARTIFACT_PREFIX)$(BUILD_ARTIFACT_NAME)$(if $(BUILD_ARTIFACT_EXTENSION),.$(BUILD_ARTIFACT_EXTENSION),)

# Add inputs and outputs from these tool invocations to the build variables 
EXECUTABLES += \
stm32f767_freertos.elf \

MAP_FILES += \
stm32f767_freertos.map \

SIZE_OUTPUT += \
default.size.stdout \

OBJDUMP_LIST += \
stm32f767_freertos.list \


# All Target
all: main-build

# Main-build Target
main-build: stm32f767_freertos.elf secondary-outputs

dependents:
	-cd D:/stm32/workspace_1.12.1/stm32f767_freertos/Release && $(MAKE) clean all

# Tool invocations
stm32f767_freertos.elf stm32f767_freertos.map: $(OBJS) $(USER_OBJS) D:/stm32/workspace_1.12.1/stm32f767_freertos/Release/stm32f767_freertos.elf D:\stm32\workspace_1.12.1\stm32f767_freertos\STM32F767ZITX_FLASH.ld makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-gcc -o "stm32f767_freertos.elf" @"objects.list" $(USER_OBJS) $(LIBS) -mcpu=cortex-m7 -T"D:\stm32\workspace_1.12.1\stm32f767_freertos\STM32F767ZITX_FLASH.ld" --specs=nosys.specs -Wl,-Map="stm32f767_freertos.map" -Wl,--gc-sections -static --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -Wl,--start-group -lc -lm -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

default.size.stdout: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-size  $(EXECUTABLES)
	@echo 'Finished building: $@'
	@echo ' '

stm32f767_freertos.list: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-objdump -h -S $(EXECUTABLES) > "stm32f767_freertos.list"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) default.size.stdout stm32f767_freertos.elf stm32f767_freertos.list stm32f767_freertos.map
	-@echo ' '

secondary-outputs: $(SIZE_OUTPUT) $(OBJDUMP_LIST)

fail-specified-linker-script-missing:
	@echo 'Error: Cannot find the specified linker script. Check the linker settings in the build configuration.'
	@exit 2

warn-no-linker-script-specified:
	@echo 'Warning: No linker script specified. Check the linker settings in the build configuration.'

.PHONY: all clean dependents main-build fail-specified-linker-script-missing warn-no-linker-script-specified
D:/stm32/workspace_1.12.1/stm32f767_freertos/Release/stm32f767_freertos.elf:

-include ../makefile.targets
