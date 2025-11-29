################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../tinyusb/src/portable/synopsys/dwc2/dcd_dwc2.c \
../tinyusb/src/portable/synopsys/dwc2/dwc2_common.c \
../tinyusb/src/portable/synopsys/dwc2/hcd_dwc2.c 

OBJS += \
./tinyusb/src/portable/synopsys/dwc2/dcd_dwc2.o \
./tinyusb/src/portable/synopsys/dwc2/dwc2_common.o \
./tinyusb/src/portable/synopsys/dwc2/hcd_dwc2.o 

C_DEPS += \
./tinyusb/src/portable/synopsys/dwc2/dcd_dwc2.d \
./tinyusb/src/portable/synopsys/dwc2/dwc2_common.d \
./tinyusb/src/portable/synopsys/dwc2/hcd_dwc2.d 


# Each subdirectory must supply rules for building sources it contributes
tinyusb/src/portable/synopsys/dwc2/%.o tinyusb/src/portable/synopsys/dwc2/%.su tinyusb/src/portable/synopsys/dwc2/%.cyclo: ../tinyusb/src/portable/synopsys/dwc2/%.c tinyusb/src/portable/synopsys/dwc2/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F767xx -DUSE_FULL_LL_DRIVER -c -I../Core/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/CMSIS/Include -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/freertos/include" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/freertos/portable" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/Bsp" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/lwip-2.2.2/src/include" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/lwip-2.2.2/system" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/letter-shell-3.x/src" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/letter-shell-3.x/drivers" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/fatfs/source" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/fatfs/drivers" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/tinyusb/src" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/tinyusb/demo" -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-tinyusb-2f-src-2f-portable-2f-synopsys-2f-dwc2

clean-tinyusb-2f-src-2f-portable-2f-synopsys-2f-dwc2:
	-$(RM) ./tinyusb/src/portable/synopsys/dwc2/dcd_dwc2.cyclo ./tinyusb/src/portable/synopsys/dwc2/dcd_dwc2.d ./tinyusb/src/portable/synopsys/dwc2/dcd_dwc2.o ./tinyusb/src/portable/synopsys/dwc2/dcd_dwc2.su ./tinyusb/src/portable/synopsys/dwc2/dwc2_common.cyclo ./tinyusb/src/portable/synopsys/dwc2/dwc2_common.d ./tinyusb/src/portable/synopsys/dwc2/dwc2_common.o ./tinyusb/src/portable/synopsys/dwc2/dwc2_common.su ./tinyusb/src/portable/synopsys/dwc2/hcd_dwc2.cyclo ./tinyusb/src/portable/synopsys/dwc2/hcd_dwc2.d ./tinyusb/src/portable/synopsys/dwc2/hcd_dwc2.o ./tinyusb/src/portable/synopsys/dwc2/hcd_dwc2.su

.PHONY: clean-tinyusb-2f-src-2f-portable-2f-synopsys-2f-dwc2

