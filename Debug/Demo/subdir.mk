################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Demo/button.c \
../Demo/lan8742.c 

OBJS += \
./Demo/button.o \
./Demo/lan8742.o 

C_DEPS += \
./Demo/button.d \
./Demo/lan8742.d 


# Each subdirectory must supply rules for building sources it contributes
Demo/%.o Demo/%.su Demo/%.cyclo: ../Demo/%.c Demo/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F767xx -DUSE_FULL_LL_DRIVER -c -I../Core/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/CMSIS/Include -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/freertos/include" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/freertos/portable" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/Demo" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/lwip/include" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Demo

clean-Demo:
	-$(RM) ./Demo/button.cyclo ./Demo/button.d ./Demo/button.o ./Demo/button.su ./Demo/lan8742.cyclo ./Demo/lan8742.d ./Demo/lan8742.o ./Demo/lan8742.su

.PHONY: clean-Demo

