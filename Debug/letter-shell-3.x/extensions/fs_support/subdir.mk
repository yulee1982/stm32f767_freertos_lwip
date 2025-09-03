################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../letter-shell-3.x/extensions/fs_support/shell_fs.c 

OBJS += \
./letter-shell-3.x/extensions/fs_support/shell_fs.o 

C_DEPS += \
./letter-shell-3.x/extensions/fs_support/shell_fs.d 


# Each subdirectory must supply rules for building sources it contributes
letter-shell-3.x/extensions/fs_support/%.o letter-shell-3.x/extensions/fs_support/%.su letter-shell-3.x/extensions/fs_support/%.cyclo: ../letter-shell-3.x/extensions/fs_support/%.c letter-shell-3.x/extensions/fs_support/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F767xx -DUSE_FULL_LL_DRIVER -c -I../Core/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/CMSIS/Include -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/freertos/portable" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/freertos/include" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/Bsp" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/lwip-2.2.2/src/include" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/lwip-2.2.2/system" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/fatfs_r0.15b/source" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/fatfs_r0.15b/drivers" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/letter-shell-3.x/src" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/letter-shell-3.x/drivers" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-letter-2d-shell-2d-3-2e-x-2f-extensions-2f-fs_support

clean-letter-2d-shell-2d-3-2e-x-2f-extensions-2f-fs_support:
	-$(RM) ./letter-shell-3.x/extensions/fs_support/shell_fs.cyclo ./letter-shell-3.x/extensions/fs_support/shell_fs.d ./letter-shell-3.x/extensions/fs_support/shell_fs.o ./letter-shell-3.x/extensions/fs_support/shell_fs.su

.PHONY: clean-letter-2d-shell-2d-3-2e-x-2f-extensions-2f-fs_support

