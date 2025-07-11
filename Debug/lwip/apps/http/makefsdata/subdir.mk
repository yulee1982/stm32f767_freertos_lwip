################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../lwip/apps/http/makefsdata/makefsdata.c 

OBJS += \
./lwip/apps/http/makefsdata/makefsdata.o 

C_DEPS += \
./lwip/apps/http/makefsdata/makefsdata.d 


# Each subdirectory must supply rules for building sources it contributes
lwip/apps/http/makefsdata/%.o lwip/apps/http/makefsdata/%.su lwip/apps/http/makefsdata/%.cyclo: ../lwip/apps/http/makefsdata/%.c lwip/apps/http/makefsdata/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F767xx -DUSE_FULL_LL_DRIVER -c -I../Core/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/CMSIS/Include -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/freertos/portable" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/freertos/include" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/lwip/include" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/Bsp" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-lwip-2f-apps-2f-http-2f-makefsdata

clean-lwip-2f-apps-2f-http-2f-makefsdata:
	-$(RM) ./lwip/apps/http/makefsdata/makefsdata.cyclo ./lwip/apps/http/makefsdata/makefsdata.d ./lwip/apps/http/makefsdata/makefsdata.o ./lwip/apps/http/makefsdata/makefsdata.su

.PHONY: clean-lwip-2f-apps-2f-http-2f-makefsdata

