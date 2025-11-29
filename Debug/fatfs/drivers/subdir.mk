################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../fatfs/drivers/sdio_sd.c \
../fatfs/drivers/tm_stm32f7_fatfs.c 

OBJS += \
./fatfs/drivers/sdio_sd.o \
./fatfs/drivers/tm_stm32f7_fatfs.o 

C_DEPS += \
./fatfs/drivers/sdio_sd.d \
./fatfs/drivers/tm_stm32f7_fatfs.d 


# Each subdirectory must supply rules for building sources it contributes
fatfs/drivers/%.o fatfs/drivers/%.su fatfs/drivers/%.cyclo: ../fatfs/drivers/%.c fatfs/drivers/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F767xx -DUSE_FULL_LL_DRIVER -c -I../Core/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/CMSIS/Include -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/freertos/portable" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/freertos/include" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/Bsp" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/lwip-2.2.2/src/include" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/lwip-2.2.2/system" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/letter-shell-3.x/src" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/letter-shell-3.x/drivers" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/fatfs/source" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/fatfs/drivers" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/tinyusb/src" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/tinyusb/demo" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-fatfs-2f-drivers

clean-fatfs-2f-drivers:
	-$(RM) ./fatfs/drivers/sdio_sd.cyclo ./fatfs/drivers/sdio_sd.d ./fatfs/drivers/sdio_sd.o ./fatfs/drivers/sdio_sd.su ./fatfs/drivers/tm_stm32f7_fatfs.cyclo ./fatfs/drivers/tm_stm32f7_fatfs.d ./fatfs/drivers/tm_stm32f7_fatfs.o ./fatfs/drivers/tm_stm32f7_fatfs.su

.PHONY: clean-fatfs-2f-drivers

