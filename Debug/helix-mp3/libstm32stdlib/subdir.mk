################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../helix-mp3/libstm32stdlib/misc.c \
../helix-mp3/libstm32stdlib/stm32f4xx_adc.c \
../helix-mp3/libstm32stdlib/stm32f4xx_dma.c \
../helix-mp3/libstm32stdlib/stm32f4xx_flash.c \
../helix-mp3/libstm32stdlib/stm32f4xx_gpio.c \
../helix-mp3/libstm32stdlib/stm32f4xx_iwdg.c \
../helix-mp3/libstm32stdlib/stm32f4xx_pwr.c \
../helix-mp3/libstm32stdlib/stm32f4xx_rcc.c \
../helix-mp3/libstm32stdlib/stm32f4xx_sdio.c \
../helix-mp3/libstm32stdlib/stm32f4xx_spi.c \
../helix-mp3/libstm32stdlib/stm32f4xx_tim.c 

OBJS += \
./helix-mp3/libstm32stdlib/misc.o \
./helix-mp3/libstm32stdlib/stm32f4xx_adc.o \
./helix-mp3/libstm32stdlib/stm32f4xx_dma.o \
./helix-mp3/libstm32stdlib/stm32f4xx_flash.o \
./helix-mp3/libstm32stdlib/stm32f4xx_gpio.o \
./helix-mp3/libstm32stdlib/stm32f4xx_iwdg.o \
./helix-mp3/libstm32stdlib/stm32f4xx_pwr.o \
./helix-mp3/libstm32stdlib/stm32f4xx_rcc.o \
./helix-mp3/libstm32stdlib/stm32f4xx_sdio.o \
./helix-mp3/libstm32stdlib/stm32f4xx_spi.o \
./helix-mp3/libstm32stdlib/stm32f4xx_tim.o 

C_DEPS += \
./helix-mp3/libstm32stdlib/misc.d \
./helix-mp3/libstm32stdlib/stm32f4xx_adc.d \
./helix-mp3/libstm32stdlib/stm32f4xx_dma.d \
./helix-mp3/libstm32stdlib/stm32f4xx_flash.d \
./helix-mp3/libstm32stdlib/stm32f4xx_gpio.d \
./helix-mp3/libstm32stdlib/stm32f4xx_iwdg.d \
./helix-mp3/libstm32stdlib/stm32f4xx_pwr.d \
./helix-mp3/libstm32stdlib/stm32f4xx_rcc.d \
./helix-mp3/libstm32stdlib/stm32f4xx_sdio.d \
./helix-mp3/libstm32stdlib/stm32f4xx_spi.d \
./helix-mp3/libstm32stdlib/stm32f4xx_tim.d 


# Each subdirectory must supply rules for building sources it contributes
helix-mp3/libstm32stdlib/%.o helix-mp3/libstm32stdlib/%.su helix-mp3/libstm32stdlib/%.cyclo: ../helix-mp3/libstm32stdlib/%.c helix-mp3/libstm32stdlib/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F767xx -DUSE_FULL_LL_DRIVER -c -I../Core/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/CMSIS/Include -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/freertos/portable" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/freertos/include" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/Bsp" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/lwip-2.2.2/src/include" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/lwip-2.2.2/system" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/fatfs_r0.15b/source" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/fatfs_r0.15b/drivers" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/letter-shell-3.x/src" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/letter-shell-3.x/drivers" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/tinyusb/src" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/tinyusb/demo" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/helix-mp3/music_app" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/helix-mp3/libhelix" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/helix-mp3/libflac" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/helix-mp3/libstm32stdlib" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-helix-2d-mp3-2f-libstm32stdlib

clean-helix-2d-mp3-2f-libstm32stdlib:
	-$(RM) ./helix-mp3/libstm32stdlib/misc.cyclo ./helix-mp3/libstm32stdlib/misc.d ./helix-mp3/libstm32stdlib/misc.o ./helix-mp3/libstm32stdlib/misc.su ./helix-mp3/libstm32stdlib/stm32f4xx_adc.cyclo ./helix-mp3/libstm32stdlib/stm32f4xx_adc.d ./helix-mp3/libstm32stdlib/stm32f4xx_adc.o ./helix-mp3/libstm32stdlib/stm32f4xx_adc.su ./helix-mp3/libstm32stdlib/stm32f4xx_dma.cyclo ./helix-mp3/libstm32stdlib/stm32f4xx_dma.d ./helix-mp3/libstm32stdlib/stm32f4xx_dma.o ./helix-mp3/libstm32stdlib/stm32f4xx_dma.su ./helix-mp3/libstm32stdlib/stm32f4xx_flash.cyclo ./helix-mp3/libstm32stdlib/stm32f4xx_flash.d ./helix-mp3/libstm32stdlib/stm32f4xx_flash.o ./helix-mp3/libstm32stdlib/stm32f4xx_flash.su ./helix-mp3/libstm32stdlib/stm32f4xx_gpio.cyclo ./helix-mp3/libstm32stdlib/stm32f4xx_gpio.d ./helix-mp3/libstm32stdlib/stm32f4xx_gpio.o ./helix-mp3/libstm32stdlib/stm32f4xx_gpio.su ./helix-mp3/libstm32stdlib/stm32f4xx_iwdg.cyclo ./helix-mp3/libstm32stdlib/stm32f4xx_iwdg.d ./helix-mp3/libstm32stdlib/stm32f4xx_iwdg.o ./helix-mp3/libstm32stdlib/stm32f4xx_iwdg.su ./helix-mp3/libstm32stdlib/stm32f4xx_pwr.cyclo ./helix-mp3/libstm32stdlib/stm32f4xx_pwr.d ./helix-mp3/libstm32stdlib/stm32f4xx_pwr.o ./helix-mp3/libstm32stdlib/stm32f4xx_pwr.su ./helix-mp3/libstm32stdlib/stm32f4xx_rcc.cyclo ./helix-mp3/libstm32stdlib/stm32f4xx_rcc.d ./helix-mp3/libstm32stdlib/stm32f4xx_rcc.o ./helix-mp3/libstm32stdlib/stm32f4xx_rcc.su ./helix-mp3/libstm32stdlib/stm32f4xx_sdio.cyclo ./helix-mp3/libstm32stdlib/stm32f4xx_sdio.d ./helix-mp3/libstm32stdlib/stm32f4xx_sdio.o ./helix-mp3/libstm32stdlib/stm32f4xx_sdio.su ./helix-mp3/libstm32stdlib/stm32f4xx_spi.cyclo ./helix-mp3/libstm32stdlib/stm32f4xx_spi.d ./helix-mp3/libstm32stdlib/stm32f4xx_spi.o ./helix-mp3/libstm32stdlib/stm32f4xx_spi.su ./helix-mp3/libstm32stdlib/stm32f4xx_tim.cyclo ./helix-mp3/libstm32stdlib/stm32f4xx_tim.d ./helix-mp3/libstm32stdlib/stm32f4xx_tim.o ./helix-mp3/libstm32stdlib/stm32f4xx_tim.su

.PHONY: clean-helix-2d-mp3-2f-libstm32stdlib

