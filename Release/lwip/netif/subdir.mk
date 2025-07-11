################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../lwip/netif/bridgeif.c \
../lwip/netif/bridgeif_fdb.c \
../lwip/netif/ethernet.c \
../lwip/netif/lowpan6.c \
../lwip/netif/lowpan6_ble.c \
../lwip/netif/lowpan6_common.c \
../lwip/netif/slipif.c \
../lwip/netif/zepif.c 

OBJS += \
./lwip/netif/bridgeif.o \
./lwip/netif/bridgeif_fdb.o \
./lwip/netif/ethernet.o \
./lwip/netif/lowpan6.o \
./lwip/netif/lowpan6_ble.o \
./lwip/netif/lowpan6_common.o \
./lwip/netif/slipif.o \
./lwip/netif/zepif.o 

C_DEPS += \
./lwip/netif/bridgeif.d \
./lwip/netif/bridgeif_fdb.d \
./lwip/netif/ethernet.d \
./lwip/netif/lowpan6.d \
./lwip/netif/lowpan6_ble.d \
./lwip/netif/lowpan6_common.d \
./lwip/netif/slipif.d \
./lwip/netif/zepif.d 


# Each subdirectory must supply rules for building sources it contributes
lwip/netif/%.o lwip/netif/%.su lwip/netif/%.cyclo: ../lwip/netif/%.c lwip/netif/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -DUSE_HAL_DRIVER -DSTM32F767xx -DUSE_FULL_LL_DRIVER -c -I../Core/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/CMSIS/Include -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/freertos/include" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/lwip/include" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/freertos/portable" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/Bsp" -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-lwip-2f-netif

clean-lwip-2f-netif:
	-$(RM) ./lwip/netif/bridgeif.cyclo ./lwip/netif/bridgeif.d ./lwip/netif/bridgeif.o ./lwip/netif/bridgeif.su ./lwip/netif/bridgeif_fdb.cyclo ./lwip/netif/bridgeif_fdb.d ./lwip/netif/bridgeif_fdb.o ./lwip/netif/bridgeif_fdb.su ./lwip/netif/ethernet.cyclo ./lwip/netif/ethernet.d ./lwip/netif/ethernet.o ./lwip/netif/ethernet.su ./lwip/netif/lowpan6.cyclo ./lwip/netif/lowpan6.d ./lwip/netif/lowpan6.o ./lwip/netif/lowpan6.su ./lwip/netif/lowpan6_ble.cyclo ./lwip/netif/lowpan6_ble.d ./lwip/netif/lowpan6_ble.o ./lwip/netif/lowpan6_ble.su ./lwip/netif/lowpan6_common.cyclo ./lwip/netif/lowpan6_common.d ./lwip/netif/lowpan6_common.o ./lwip/netif/lowpan6_common.su ./lwip/netif/slipif.cyclo ./lwip/netif/slipif.d ./lwip/netif/slipif.o ./lwip/netif/slipif.su ./lwip/netif/zepif.cyclo ./lwip/netif/zepif.d ./lwip/netif/zepif.o ./lwip/netif/zepif.su

.PHONY: clean-lwip-2f-netif

