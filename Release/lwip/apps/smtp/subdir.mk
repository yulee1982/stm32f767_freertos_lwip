################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../lwip/apps/smtp/smtp.c 

OBJS += \
./lwip/apps/smtp/smtp.o 

C_DEPS += \
./lwip/apps/smtp/smtp.d 


# Each subdirectory must supply rules for building sources it contributes
lwip/apps/smtp/%.o lwip/apps/smtp/%.su lwip/apps/smtp/%.cyclo: ../lwip/apps/smtp/%.c lwip/apps/smtp/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -DUSE_HAL_DRIVER -DSTM32F767xx -DUSE_FULL_LL_DRIVER -c -I../Core/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/CMSIS/Include -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/freertos/include" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/lwip/include" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/freertos/portable" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/Bsp" -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-lwip-2f-apps-2f-smtp

clean-lwip-2f-apps-2f-smtp:
	-$(RM) ./lwip/apps/smtp/smtp.cyclo ./lwip/apps/smtp/smtp.d ./lwip/apps/smtp/smtp.o ./lwip/apps/smtp/smtp.su

.PHONY: clean-lwip-2f-apps-2f-smtp

