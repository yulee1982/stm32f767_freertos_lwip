################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../lwip/core/ipv4/acd.c \
../lwip/core/ipv4/autoip.c \
../lwip/core/ipv4/dhcp.c \
../lwip/core/ipv4/etharp.c \
../lwip/core/ipv4/icmp.c \
../lwip/core/ipv4/igmp.c \
../lwip/core/ipv4/ip4.c \
../lwip/core/ipv4/ip4_addr.c \
../lwip/core/ipv4/ip4_frag.c 

OBJS += \
./lwip/core/ipv4/acd.o \
./lwip/core/ipv4/autoip.o \
./lwip/core/ipv4/dhcp.o \
./lwip/core/ipv4/etharp.o \
./lwip/core/ipv4/icmp.o \
./lwip/core/ipv4/igmp.o \
./lwip/core/ipv4/ip4.o \
./lwip/core/ipv4/ip4_addr.o \
./lwip/core/ipv4/ip4_frag.o 

C_DEPS += \
./lwip/core/ipv4/acd.d \
./lwip/core/ipv4/autoip.d \
./lwip/core/ipv4/dhcp.d \
./lwip/core/ipv4/etharp.d \
./lwip/core/ipv4/icmp.d \
./lwip/core/ipv4/igmp.d \
./lwip/core/ipv4/ip4.d \
./lwip/core/ipv4/ip4_addr.d \
./lwip/core/ipv4/ip4_frag.d 


# Each subdirectory must supply rules for building sources it contributes
lwip/core/ipv4/%.o lwip/core/ipv4/%.su lwip/core/ipv4/%.cyclo: ../lwip/core/ipv4/%.c lwip/core/ipv4/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F767xx -DUSE_FULL_LL_DRIVER -c -I../Core/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/CMSIS/Include -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/freertos/portable" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/freertos/include" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/lwip/include" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/Bsp" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-lwip-2f-core-2f-ipv4

clean-lwip-2f-core-2f-ipv4:
	-$(RM) ./lwip/core/ipv4/acd.cyclo ./lwip/core/ipv4/acd.d ./lwip/core/ipv4/acd.o ./lwip/core/ipv4/acd.su ./lwip/core/ipv4/autoip.cyclo ./lwip/core/ipv4/autoip.d ./lwip/core/ipv4/autoip.o ./lwip/core/ipv4/autoip.su ./lwip/core/ipv4/dhcp.cyclo ./lwip/core/ipv4/dhcp.d ./lwip/core/ipv4/dhcp.o ./lwip/core/ipv4/dhcp.su ./lwip/core/ipv4/etharp.cyclo ./lwip/core/ipv4/etharp.d ./lwip/core/ipv4/etharp.o ./lwip/core/ipv4/etharp.su ./lwip/core/ipv4/icmp.cyclo ./lwip/core/ipv4/icmp.d ./lwip/core/ipv4/icmp.o ./lwip/core/ipv4/icmp.su ./lwip/core/ipv4/igmp.cyclo ./lwip/core/ipv4/igmp.d ./lwip/core/ipv4/igmp.o ./lwip/core/ipv4/igmp.su ./lwip/core/ipv4/ip4.cyclo ./lwip/core/ipv4/ip4.d ./lwip/core/ipv4/ip4.o ./lwip/core/ipv4/ip4.su ./lwip/core/ipv4/ip4_addr.cyclo ./lwip/core/ipv4/ip4_addr.d ./lwip/core/ipv4/ip4_addr.o ./lwip/core/ipv4/ip4_addr.su ./lwip/core/ipv4/ip4_frag.cyclo ./lwip/core/ipv4/ip4_frag.d ./lwip/core/ipv4/ip4_frag.o ./lwip/core/ipv4/ip4_frag.su

.PHONY: clean-lwip-2f-core-2f-ipv4

