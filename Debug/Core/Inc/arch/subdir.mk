################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Inc/arch/sys_arch.c 

OBJS += \
./Core/Inc/arch/sys_arch.o 

C_DEPS += \
./Core/Inc/arch/sys_arch.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Inc/arch/%.o Core/Inc/arch/%.su Core/Inc/arch/%.cyclo: ../Core/Inc/arch/%.c Core/Inc/arch/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F767xx -DUSE_FULL_LL_DRIVER -c -I../Core/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/CMSIS/Include -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/freertos/portable" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/freertos/include" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/lwip/include" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/Bsp" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Inc-2f-arch

clean-Core-2f-Inc-2f-arch:
	-$(RM) ./Core/Inc/arch/sys_arch.cyclo ./Core/Inc/arch/sys_arch.d ./Core/Inc/arch/sys_arch.o ./Core/Inc/arch/sys_arch.su

.PHONY: clean-Core-2f-Inc-2f-arch

