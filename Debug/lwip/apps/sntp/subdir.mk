################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../lwip/apps/sntp/sntp.c 

OBJS += \
./lwip/apps/sntp/sntp.o 

C_DEPS += \
./lwip/apps/sntp/sntp.d 


# Each subdirectory must supply rules for building sources it contributes
lwip/apps/sntp/%.o lwip/apps/sntp/%.su lwip/apps/sntp/%.cyclo: ../lwip/apps/sntp/%.c lwip/apps/sntp/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F767xx -DUSE_FULL_LL_DRIVER -c -I../Core/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/CMSIS/Include -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/freertos/portable" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/freertos/include" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/lwip/include" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/Bsp" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-lwip-2f-apps-2f-sntp

clean-lwip-2f-apps-2f-sntp:
	-$(RM) ./lwip/apps/sntp/sntp.cyclo ./lwip/apps/sntp/sntp.d ./lwip/apps/sntp/sntp.o ./lwip/apps/sntp/sntp.su

.PHONY: clean-lwip-2f-apps-2f-sntp

