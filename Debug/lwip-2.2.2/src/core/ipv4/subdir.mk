################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../lwip-2.2.2/src/core/ipv4/acd.c \
../lwip-2.2.2/src/core/ipv4/autoip.c \
../lwip-2.2.2/src/core/ipv4/dhcp.c \
../lwip-2.2.2/src/core/ipv4/etharp.c \
../lwip-2.2.2/src/core/ipv4/icmp.c \
../lwip-2.2.2/src/core/ipv4/igmp.c \
../lwip-2.2.2/src/core/ipv4/ip4.c \
../lwip-2.2.2/src/core/ipv4/ip4_addr.c \
../lwip-2.2.2/src/core/ipv4/ip4_frag.c 

OBJS += \
./lwip-2.2.2/src/core/ipv4/acd.o \
./lwip-2.2.2/src/core/ipv4/autoip.o \
./lwip-2.2.2/src/core/ipv4/dhcp.o \
./lwip-2.2.2/src/core/ipv4/etharp.o \
./lwip-2.2.2/src/core/ipv4/icmp.o \
./lwip-2.2.2/src/core/ipv4/igmp.o \
./lwip-2.2.2/src/core/ipv4/ip4.o \
./lwip-2.2.2/src/core/ipv4/ip4_addr.o \
./lwip-2.2.2/src/core/ipv4/ip4_frag.o 

C_DEPS += \
./lwip-2.2.2/src/core/ipv4/acd.d \
./lwip-2.2.2/src/core/ipv4/autoip.d \
./lwip-2.2.2/src/core/ipv4/dhcp.d \
./lwip-2.2.2/src/core/ipv4/etharp.d \
./lwip-2.2.2/src/core/ipv4/icmp.d \
./lwip-2.2.2/src/core/ipv4/igmp.d \
./lwip-2.2.2/src/core/ipv4/ip4.d \
./lwip-2.2.2/src/core/ipv4/ip4_addr.d \
./lwip-2.2.2/src/core/ipv4/ip4_frag.d 


# Each subdirectory must supply rules for building sources it contributes
lwip-2.2.2/src/core/ipv4/%.o lwip-2.2.2/src/core/ipv4/%.su lwip-2.2.2/src/core/ipv4/%.cyclo: ../lwip-2.2.2/src/core/ipv4/%.c lwip-2.2.2/src/core/ipv4/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F767xx -DUSE_FULL_LL_DRIVER -c -I../Core/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/CMSIS/Include -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/freertos/portable" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/freertos/include" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/Bsp" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/lwip-2.2.2/src/include" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/lwip-2.2.2/system" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/fatfs_r0.15b/source" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/fatfs_r0.15b/drivers" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-lwip-2d-2-2e-2-2e-2-2f-src-2f-core-2f-ipv4

clean-lwip-2d-2-2e-2-2e-2-2f-src-2f-core-2f-ipv4:
	-$(RM) ./lwip-2.2.2/src/core/ipv4/acd.cyclo ./lwip-2.2.2/src/core/ipv4/acd.d ./lwip-2.2.2/src/core/ipv4/acd.o ./lwip-2.2.2/src/core/ipv4/acd.su ./lwip-2.2.2/src/core/ipv4/autoip.cyclo ./lwip-2.2.2/src/core/ipv4/autoip.d ./lwip-2.2.2/src/core/ipv4/autoip.o ./lwip-2.2.2/src/core/ipv4/autoip.su ./lwip-2.2.2/src/core/ipv4/dhcp.cyclo ./lwip-2.2.2/src/core/ipv4/dhcp.d ./lwip-2.2.2/src/core/ipv4/dhcp.o ./lwip-2.2.2/src/core/ipv4/dhcp.su ./lwip-2.2.2/src/core/ipv4/etharp.cyclo ./lwip-2.2.2/src/core/ipv4/etharp.d ./lwip-2.2.2/src/core/ipv4/etharp.o ./lwip-2.2.2/src/core/ipv4/etharp.su ./lwip-2.2.2/src/core/ipv4/icmp.cyclo ./lwip-2.2.2/src/core/ipv4/icmp.d ./lwip-2.2.2/src/core/ipv4/icmp.o ./lwip-2.2.2/src/core/ipv4/icmp.su ./lwip-2.2.2/src/core/ipv4/igmp.cyclo ./lwip-2.2.2/src/core/ipv4/igmp.d ./lwip-2.2.2/src/core/ipv4/igmp.o ./lwip-2.2.2/src/core/ipv4/igmp.su ./lwip-2.2.2/src/core/ipv4/ip4.cyclo ./lwip-2.2.2/src/core/ipv4/ip4.d ./lwip-2.2.2/src/core/ipv4/ip4.o ./lwip-2.2.2/src/core/ipv4/ip4.su ./lwip-2.2.2/src/core/ipv4/ip4_addr.cyclo ./lwip-2.2.2/src/core/ipv4/ip4_addr.d ./lwip-2.2.2/src/core/ipv4/ip4_addr.o ./lwip-2.2.2/src/core/ipv4/ip4_addr.su ./lwip-2.2.2/src/core/ipv4/ip4_frag.cyclo ./lwip-2.2.2/src/core/ipv4/ip4_frag.d ./lwip-2.2.2/src/core/ipv4/ip4_frag.o ./lwip-2.2.2/src/core/ipv4/ip4_frag.su

.PHONY: clean-lwip-2d-2-2e-2-2e-2-2f-src-2f-core-2f-ipv4

