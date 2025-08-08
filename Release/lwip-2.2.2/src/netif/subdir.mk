################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../lwip-2.2.2/src/netif/bridgeif.c \
../lwip-2.2.2/src/netif/bridgeif_fdb.c \
../lwip-2.2.2/src/netif/ethernet.c \
../lwip-2.2.2/src/netif/lowpan6.c \
../lwip-2.2.2/src/netif/lowpan6_ble.c \
../lwip-2.2.2/src/netif/lowpan6_common.c \
../lwip-2.2.2/src/netif/slipif.c \
../lwip-2.2.2/src/netif/zepif.c 

OBJS += \
./lwip-2.2.2/src/netif/bridgeif.o \
./lwip-2.2.2/src/netif/bridgeif_fdb.o \
./lwip-2.2.2/src/netif/ethernet.o \
./lwip-2.2.2/src/netif/lowpan6.o \
./lwip-2.2.2/src/netif/lowpan6_ble.o \
./lwip-2.2.2/src/netif/lowpan6_common.o \
./lwip-2.2.2/src/netif/slipif.o \
./lwip-2.2.2/src/netif/zepif.o 

C_DEPS += \
./lwip-2.2.2/src/netif/bridgeif.d \
./lwip-2.2.2/src/netif/bridgeif_fdb.d \
./lwip-2.2.2/src/netif/ethernet.d \
./lwip-2.2.2/src/netif/lowpan6.d \
./lwip-2.2.2/src/netif/lowpan6_ble.d \
./lwip-2.2.2/src/netif/lowpan6_common.d \
./lwip-2.2.2/src/netif/slipif.d \
./lwip-2.2.2/src/netif/zepif.d 


# Each subdirectory must supply rules for building sources it contributes
lwip-2.2.2/src/netif/%.o lwip-2.2.2/src/netif/%.su lwip-2.2.2/src/netif/%.cyclo: ../lwip-2.2.2/src/netif/%.c lwip-2.2.2/src/netif/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -DUSE_HAL_DRIVER -DSTM32F767xx -DUSE_FULL_LL_DRIVER -c -I../Core/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/CMSIS/Include -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/freertos/include" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/freertos/portable" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/Bsp" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/lwip-2.2.2/src/include" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/lwip-2.2.2/system" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/fatfs_r0.15b/source" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/fatfs_r0.15b/drivers" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/letter-shell-3.x/src" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/letter-shell-3.x/drivers" -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-lwip-2d-2-2e-2-2e-2-2f-src-2f-netif

clean-lwip-2d-2-2e-2-2e-2-2f-src-2f-netif:
	-$(RM) ./lwip-2.2.2/src/netif/bridgeif.cyclo ./lwip-2.2.2/src/netif/bridgeif.d ./lwip-2.2.2/src/netif/bridgeif.o ./lwip-2.2.2/src/netif/bridgeif.su ./lwip-2.2.2/src/netif/bridgeif_fdb.cyclo ./lwip-2.2.2/src/netif/bridgeif_fdb.d ./lwip-2.2.2/src/netif/bridgeif_fdb.o ./lwip-2.2.2/src/netif/bridgeif_fdb.su ./lwip-2.2.2/src/netif/ethernet.cyclo ./lwip-2.2.2/src/netif/ethernet.d ./lwip-2.2.2/src/netif/ethernet.o ./lwip-2.2.2/src/netif/ethernet.su ./lwip-2.2.2/src/netif/lowpan6.cyclo ./lwip-2.2.2/src/netif/lowpan6.d ./lwip-2.2.2/src/netif/lowpan6.o ./lwip-2.2.2/src/netif/lowpan6.su ./lwip-2.2.2/src/netif/lowpan6_ble.cyclo ./lwip-2.2.2/src/netif/lowpan6_ble.d ./lwip-2.2.2/src/netif/lowpan6_ble.o ./lwip-2.2.2/src/netif/lowpan6_ble.su ./lwip-2.2.2/src/netif/lowpan6_common.cyclo ./lwip-2.2.2/src/netif/lowpan6_common.d ./lwip-2.2.2/src/netif/lowpan6_common.o ./lwip-2.2.2/src/netif/lowpan6_common.su ./lwip-2.2.2/src/netif/slipif.cyclo ./lwip-2.2.2/src/netif/slipif.d ./lwip-2.2.2/src/netif/slipif.o ./lwip-2.2.2/src/netif/slipif.su ./lwip-2.2.2/src/netif/zepif.cyclo ./lwip-2.2.2/src/netif/zepif.d ./lwip-2.2.2/src/netif/zepif.o ./lwip-2.2.2/src/netif/zepif.su

.PHONY: clean-lwip-2d-2-2e-2-2e-2-2f-src-2f-netif

