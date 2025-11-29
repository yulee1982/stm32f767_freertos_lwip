################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../lwip-2.2.2/src/apps/netbiosns/netbiosns.c 

OBJS += \
./lwip-2.2.2/src/apps/netbiosns/netbiosns.o 

C_DEPS += \
./lwip-2.2.2/src/apps/netbiosns/netbiosns.d 


# Each subdirectory must supply rules for building sources it contributes
lwip-2.2.2/src/apps/netbiosns/%.o lwip-2.2.2/src/apps/netbiosns/%.su lwip-2.2.2/src/apps/netbiosns/%.cyclo: ../lwip-2.2.2/src/apps/netbiosns/%.c lwip-2.2.2/src/apps/netbiosns/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F767xx -DUSE_FULL_LL_DRIVER -c -I../Core/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/CMSIS/Include -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/freertos/portable" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/freertos/include" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/Bsp" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/lwip-2.2.2/src/include" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/lwip-2.2.2/system" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/letter-shell-3.x/src" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/letter-shell-3.x/drivers" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/fatfs/source" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/fatfs/drivers" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/tinyusb/src" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/tinyusb/demo" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-lwip-2d-2-2e-2-2e-2-2f-src-2f-apps-2f-netbiosns

clean-lwip-2d-2-2e-2-2e-2-2f-src-2f-apps-2f-netbiosns:
	-$(RM) ./lwip-2.2.2/src/apps/netbiosns/netbiosns.cyclo ./lwip-2.2.2/src/apps/netbiosns/netbiosns.d ./lwip-2.2.2/src/apps/netbiosns/netbiosns.o ./lwip-2.2.2/src/apps/netbiosns/netbiosns.su

.PHONY: clean-lwip-2d-2-2e-2-2e-2-2f-src-2f-apps-2f-netbiosns

