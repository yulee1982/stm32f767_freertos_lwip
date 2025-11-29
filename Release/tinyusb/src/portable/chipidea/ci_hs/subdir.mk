################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../tinyusb/src/portable/chipidea/ci_hs/dcd_ci_hs.c \
../tinyusb/src/portable/chipidea/ci_hs/hcd_ci_hs.c 

OBJS += \
./tinyusb/src/portable/chipidea/ci_hs/dcd_ci_hs.o \
./tinyusb/src/portable/chipidea/ci_hs/hcd_ci_hs.o 

C_DEPS += \
./tinyusb/src/portable/chipidea/ci_hs/dcd_ci_hs.d \
./tinyusb/src/portable/chipidea/ci_hs/hcd_ci_hs.d 


# Each subdirectory must supply rules for building sources it contributes
tinyusb/src/portable/chipidea/ci_hs/%.o tinyusb/src/portable/chipidea/ci_hs/%.su tinyusb/src/portable/chipidea/ci_hs/%.cyclo: ../tinyusb/src/portable/chipidea/ci_hs/%.c tinyusb/src/portable/chipidea/ci_hs/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F767xx -DUSE_FULL_LL_DRIVER -c -I../Core/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/CMSIS/Include -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/freertos/include" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/freertos/portable" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/Bsp" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/lwip-2.2.2/src/include" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/lwip-2.2.2/system" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/letter-shell-3.x/src" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/letter-shell-3.x/drivers" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/fatfs/source" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/fatfs/drivers" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/tinyusb/src" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/tinyusb/demo" -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-tinyusb-2f-src-2f-portable-2f-chipidea-2f-ci_hs

clean-tinyusb-2f-src-2f-portable-2f-chipidea-2f-ci_hs:
	-$(RM) ./tinyusb/src/portable/chipidea/ci_hs/dcd_ci_hs.cyclo ./tinyusb/src/portable/chipidea/ci_hs/dcd_ci_hs.d ./tinyusb/src/portable/chipidea/ci_hs/dcd_ci_hs.o ./tinyusb/src/portable/chipidea/ci_hs/dcd_ci_hs.su ./tinyusb/src/portable/chipidea/ci_hs/hcd_ci_hs.cyclo ./tinyusb/src/portable/chipidea/ci_hs/hcd_ci_hs.d ./tinyusb/src/portable/chipidea/ci_hs/hcd_ci_hs.o ./tinyusb/src/portable/chipidea/ci_hs/hcd_ci_hs.su

.PHONY: clean-tinyusb-2f-src-2f-portable-2f-chipidea-2f-ci_hs

