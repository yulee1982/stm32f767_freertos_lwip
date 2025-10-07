################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../lwip-2.2.2/src/apps/sntp/sntp.c 

OBJS += \
./lwip-2.2.2/src/apps/sntp/sntp.o 

C_DEPS += \
./lwip-2.2.2/src/apps/sntp/sntp.d 


# Each subdirectory must supply rules for building sources it contributes
lwip-2.2.2/src/apps/sntp/%.o lwip-2.2.2/src/apps/sntp/%.su lwip-2.2.2/src/apps/sntp/%.cyclo: ../lwip-2.2.2/src/apps/sntp/%.c lwip-2.2.2/src/apps/sntp/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F767xx -DUSE_FULL_LL_DRIVER -c -I../Core/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/CMSIS/Include -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/freertos/portable" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/freertos/include" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/Bsp" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/lwip-2.2.2/src/include" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/lwip-2.2.2/system" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/fatfs_r0.15b/source" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/fatfs_r0.15b/drivers" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/letter-shell-3.x/src" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/letter-shell-3.x/drivers" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/usb-framework/libusb_stm32/inc" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/usb-framework/libusb_stm32/src" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/usb-framework/usb_app" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-lwip-2d-2-2e-2-2e-2-2f-src-2f-apps-2f-sntp

clean-lwip-2d-2-2e-2-2e-2-2f-src-2f-apps-2f-sntp:
	-$(RM) ./lwip-2.2.2/src/apps/sntp/sntp.cyclo ./lwip-2.2.2/src/apps/sntp/sntp.d ./lwip-2.2.2/src/apps/sntp/sntp.o ./lwip-2.2.2/src/apps/sntp/sntp.su

.PHONY: clean-lwip-2d-2-2e-2-2e-2-2f-src-2f-apps-2f-sntp

