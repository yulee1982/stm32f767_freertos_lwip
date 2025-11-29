################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../lwip-2.2.2/src/core/ipv6/dhcp6.c \
../lwip-2.2.2/src/core/ipv6/ethip6.c \
../lwip-2.2.2/src/core/ipv6/icmp6.c \
../lwip-2.2.2/src/core/ipv6/inet6.c \
../lwip-2.2.2/src/core/ipv6/ip6.c \
../lwip-2.2.2/src/core/ipv6/ip6_addr.c \
../lwip-2.2.2/src/core/ipv6/ip6_frag.c \
../lwip-2.2.2/src/core/ipv6/mld6.c \
../lwip-2.2.2/src/core/ipv6/nd6.c 

OBJS += \
./lwip-2.2.2/src/core/ipv6/dhcp6.o \
./lwip-2.2.2/src/core/ipv6/ethip6.o \
./lwip-2.2.2/src/core/ipv6/icmp6.o \
./lwip-2.2.2/src/core/ipv6/inet6.o \
./lwip-2.2.2/src/core/ipv6/ip6.o \
./lwip-2.2.2/src/core/ipv6/ip6_addr.o \
./lwip-2.2.2/src/core/ipv6/ip6_frag.o \
./lwip-2.2.2/src/core/ipv6/mld6.o \
./lwip-2.2.2/src/core/ipv6/nd6.o 

C_DEPS += \
./lwip-2.2.2/src/core/ipv6/dhcp6.d \
./lwip-2.2.2/src/core/ipv6/ethip6.d \
./lwip-2.2.2/src/core/ipv6/icmp6.d \
./lwip-2.2.2/src/core/ipv6/inet6.d \
./lwip-2.2.2/src/core/ipv6/ip6.d \
./lwip-2.2.2/src/core/ipv6/ip6_addr.d \
./lwip-2.2.2/src/core/ipv6/ip6_frag.d \
./lwip-2.2.2/src/core/ipv6/mld6.d \
./lwip-2.2.2/src/core/ipv6/nd6.d 


# Each subdirectory must supply rules for building sources it contributes
lwip-2.2.2/src/core/ipv6/%.o lwip-2.2.2/src/core/ipv6/%.su lwip-2.2.2/src/core/ipv6/%.cyclo: ../lwip-2.2.2/src/core/ipv6/%.c lwip-2.2.2/src/core/ipv6/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F767xx -DUSE_FULL_LL_DRIVER -c -I../Core/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/CMSIS/Include -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/freertos/portable" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/freertos/include" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/Bsp" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/lwip-2.2.2/src/include" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/lwip-2.2.2/system" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/letter-shell-3.x/src" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/letter-shell-3.x/drivers" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/fatfs/source" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/fatfs/drivers" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/tinyusb/src" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/tinyusb/demo" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-lwip-2d-2-2e-2-2e-2-2f-src-2f-core-2f-ipv6

clean-lwip-2d-2-2e-2-2e-2-2f-src-2f-core-2f-ipv6:
	-$(RM) ./lwip-2.2.2/src/core/ipv6/dhcp6.cyclo ./lwip-2.2.2/src/core/ipv6/dhcp6.d ./lwip-2.2.2/src/core/ipv6/dhcp6.o ./lwip-2.2.2/src/core/ipv6/dhcp6.su ./lwip-2.2.2/src/core/ipv6/ethip6.cyclo ./lwip-2.2.2/src/core/ipv6/ethip6.d ./lwip-2.2.2/src/core/ipv6/ethip6.o ./lwip-2.2.2/src/core/ipv6/ethip6.su ./lwip-2.2.2/src/core/ipv6/icmp6.cyclo ./lwip-2.2.2/src/core/ipv6/icmp6.d ./lwip-2.2.2/src/core/ipv6/icmp6.o ./lwip-2.2.2/src/core/ipv6/icmp6.su ./lwip-2.2.2/src/core/ipv6/inet6.cyclo ./lwip-2.2.2/src/core/ipv6/inet6.d ./lwip-2.2.2/src/core/ipv6/inet6.o ./lwip-2.2.2/src/core/ipv6/inet6.su ./lwip-2.2.2/src/core/ipv6/ip6.cyclo ./lwip-2.2.2/src/core/ipv6/ip6.d ./lwip-2.2.2/src/core/ipv6/ip6.o ./lwip-2.2.2/src/core/ipv6/ip6.su ./lwip-2.2.2/src/core/ipv6/ip6_addr.cyclo ./lwip-2.2.2/src/core/ipv6/ip6_addr.d ./lwip-2.2.2/src/core/ipv6/ip6_addr.o ./lwip-2.2.2/src/core/ipv6/ip6_addr.su ./lwip-2.2.2/src/core/ipv6/ip6_frag.cyclo ./lwip-2.2.2/src/core/ipv6/ip6_frag.d ./lwip-2.2.2/src/core/ipv6/ip6_frag.o ./lwip-2.2.2/src/core/ipv6/ip6_frag.su ./lwip-2.2.2/src/core/ipv6/mld6.cyclo ./lwip-2.2.2/src/core/ipv6/mld6.d ./lwip-2.2.2/src/core/ipv6/mld6.o ./lwip-2.2.2/src/core/ipv6/mld6.su ./lwip-2.2.2/src/core/ipv6/nd6.cyclo ./lwip-2.2.2/src/core/ipv6/nd6.d ./lwip-2.2.2/src/core/ipv6/nd6.o ./lwip-2.2.2/src/core/ipv6/nd6.su

.PHONY: clean-lwip-2d-2-2e-2-2e-2-2f-src-2f-core-2f-ipv6

