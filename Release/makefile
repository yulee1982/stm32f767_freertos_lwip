################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include tinyusb/src/typec/subdir.mk
-include tinyusb/src/portable/wch/subdir.mk
-include tinyusb/src/portable/valentyusb/eptri/subdir.mk
-include tinyusb/src/portable/ti/msp430x5xx/subdir.mk
-include tinyusb/src/portable/template/subdir.mk
-include tinyusb/src/portable/synopsys/dwc2/subdir.mk
-include tinyusb/src/portable/sunxi/subdir.mk
-include tinyusb/src/portable/st/typec/subdir.mk
-include tinyusb/src/portable/st/stm32_fsdev/subdir.mk
-include tinyusb/src/portable/sony/cxd56/subdir.mk
-include tinyusb/src/portable/renesas/rusb2/subdir.mk
-include tinyusb/src/portable/raspberrypi/rp2040/subdir.mk
-include tinyusb/src/portable/raspberrypi/pio_usb/subdir.mk
-include tinyusb/src/portable/ohci/subdir.mk
-include tinyusb/src/portable/nxp/lpc_ip3511/subdir.mk
-include tinyusb/src/portable/nxp/lpc17_40/subdir.mk
-include tinyusb/src/portable/nxp/khci/subdir.mk
-include tinyusb/src/portable/nuvoton/nuc505/subdir.mk
-include tinyusb/src/portable/nuvoton/nuc121/subdir.mk
-include tinyusb/src/portable/nuvoton/nuc120/subdir.mk
-include tinyusb/src/portable/nordic/nrf5x/subdir.mk
-include tinyusb/src/portable/mindmotion/mm32/subdir.mk
-include tinyusb/src/portable/microchip/samx7x/subdir.mk
-include tinyusb/src/portable/microchip/samg/subdir.mk
-include tinyusb/src/portable/microchip/samd/subdir.mk
-include tinyusb/src/portable/microchip/pic32mz/subdir.mk
-include tinyusb/src/portable/microchip/pic/subdir.mk
-include tinyusb/src/portable/mentor/musb/subdir.mk
-include tinyusb/src/portable/espressif/esp32sx/subdir.mk
-include tinyusb/src/portable/ehci/subdir.mk
-include tinyusb/src/portable/dialog/da146xx/subdir.mk
-include tinyusb/src/portable/chipidea/ci_hs/subdir.mk
-include tinyusb/src/portable/chipidea/ci_fs/subdir.mk
-include tinyusb/src/portable/bridgetek/ft9xx/subdir.mk
-include tinyusb/src/portable/analog/max3421/subdir.mk
-include tinyusb/src/host/subdir.mk
-include tinyusb/src/device/subdir.mk
-include tinyusb/src/common/subdir.mk
-include tinyusb/src/class/video/subdir.mk
-include tinyusb/src/class/vendor/subdir.mk
-include tinyusb/src/class/usbtmc/subdir.mk
-include tinyusb/src/class/net/subdir.mk
-include tinyusb/src/class/msc/subdir.mk
-include tinyusb/src/class/midi/subdir.mk
-include tinyusb/src/class/hid/subdir.mk
-include tinyusb/src/class/dfu/subdir.mk
-include tinyusb/src/class/cdc/subdir.mk
-include tinyusb/src/class/bth/subdir.mk
-include tinyusb/src/class/audio/subdir.mk
-include tinyusb/src/bsp/subdir.mk
-include tinyusb/src/subdir.mk
-include tinyusb/demo/subdir.mk
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

OBJCOPY_HEX += \
stm32f767_freertos.hex \

OBJCOPY_BIN += \
stm32f767_freertos.bin \


# All Target
all: main-build

# Main-build Target
main-build: stm32f767_freertos.elf secondary-outputs

# Tool invocations
stm32f767_freertos.elf stm32f767_freertos.map: $(OBJS) $(USER_OBJS) D:\stm32\workspace_1.12.1\stm32f767_freertos\STM32F767ZITX_FLASH.ld makefile objects.list $(OPTIONAL_TOOL_DEPS)
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

stm32f767_freertos.hex: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-objcopy  -O ihex $(EXECUTABLES) "stm32f767_freertos.hex"
	@echo 'Finished building: $@'
	@echo ' '

stm32f767_freertos.bin: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-objcopy  -O binary $(EXECUTABLES) "stm32f767_freertos.bin"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) default.size.stdout stm32f767_freertos.bin stm32f767_freertos.elf stm32f767_freertos.hex stm32f767_freertos.list stm32f767_freertos.map
	-@echo ' '

secondary-outputs: $(SIZE_OUTPUT) $(OBJDUMP_LIST) $(OBJCOPY_HEX) $(OBJCOPY_BIN)

fail-specified-linker-script-missing:
	@echo 'Error: Cannot find the specified linker script. Check the linker settings in the build configuration.'
	@exit 2

warn-no-linker-script-specified:
	@echo 'Warning: No linker script specified. Check the linker settings in the build configuration.'

.PHONY: all clean dependents main-build fail-specified-linker-script-missing warn-no-linker-script-specified

-include ../makefile.targets
