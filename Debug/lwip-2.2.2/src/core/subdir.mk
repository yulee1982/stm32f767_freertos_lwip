################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../lwip-2.2.2/src/core/altcp.c \
../lwip-2.2.2/src/core/altcp_alloc.c \
../lwip-2.2.2/src/core/altcp_tcp.c \
../lwip-2.2.2/src/core/def.c \
../lwip-2.2.2/src/core/dns.c \
../lwip-2.2.2/src/core/inet_chksum.c \
../lwip-2.2.2/src/core/init.c \
../lwip-2.2.2/src/core/ip.c \
../lwip-2.2.2/src/core/mem.c \
../lwip-2.2.2/src/core/memp.c \
../lwip-2.2.2/src/core/netif.c \
../lwip-2.2.2/src/core/pbuf.c \
../lwip-2.2.2/src/core/raw.c \
../lwip-2.2.2/src/core/stats.c \
../lwip-2.2.2/src/core/sys.c \
../lwip-2.2.2/src/core/tcp.c \
../lwip-2.2.2/src/core/tcp_in.c \
../lwip-2.2.2/src/core/tcp_out.c \
../lwip-2.2.2/src/core/timeouts.c \
../lwip-2.2.2/src/core/udp.c 

OBJS += \
./lwip-2.2.2/src/core/altcp.o \
./lwip-2.2.2/src/core/altcp_alloc.o \
./lwip-2.2.2/src/core/altcp_tcp.o \
./lwip-2.2.2/src/core/def.o \
./lwip-2.2.2/src/core/dns.o \
./lwip-2.2.2/src/core/inet_chksum.o \
./lwip-2.2.2/src/core/init.o \
./lwip-2.2.2/src/core/ip.o \
./lwip-2.2.2/src/core/mem.o \
./lwip-2.2.2/src/core/memp.o \
./lwip-2.2.2/src/core/netif.o \
./lwip-2.2.2/src/core/pbuf.o \
./lwip-2.2.2/src/core/raw.o \
./lwip-2.2.2/src/core/stats.o \
./lwip-2.2.2/src/core/sys.o \
./lwip-2.2.2/src/core/tcp.o \
./lwip-2.2.2/src/core/tcp_in.o \
./lwip-2.2.2/src/core/tcp_out.o \
./lwip-2.2.2/src/core/timeouts.o \
./lwip-2.2.2/src/core/udp.o 

C_DEPS += \
./lwip-2.2.2/src/core/altcp.d \
./lwip-2.2.2/src/core/altcp_alloc.d \
./lwip-2.2.2/src/core/altcp_tcp.d \
./lwip-2.2.2/src/core/def.d \
./lwip-2.2.2/src/core/dns.d \
./lwip-2.2.2/src/core/inet_chksum.d \
./lwip-2.2.2/src/core/init.d \
./lwip-2.2.2/src/core/ip.d \
./lwip-2.2.2/src/core/mem.d \
./lwip-2.2.2/src/core/memp.d \
./lwip-2.2.2/src/core/netif.d \
./lwip-2.2.2/src/core/pbuf.d \
./lwip-2.2.2/src/core/raw.d \
./lwip-2.2.2/src/core/stats.d \
./lwip-2.2.2/src/core/sys.d \
./lwip-2.2.2/src/core/tcp.d \
./lwip-2.2.2/src/core/tcp_in.d \
./lwip-2.2.2/src/core/tcp_out.d \
./lwip-2.2.2/src/core/timeouts.d \
./lwip-2.2.2/src/core/udp.d 


# Each subdirectory must supply rules for building sources it contributes
lwip-2.2.2/src/core/%.o lwip-2.2.2/src/core/%.su lwip-2.2.2/src/core/%.cyclo: ../lwip-2.2.2/src/core/%.c lwip-2.2.2/src/core/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F767xx -DUSE_FULL_LL_DRIVER -c -I../Core/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/CMSIS/Include -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/freertos/portable" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/freertos/include" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/Bsp" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/lwip-2.2.2/src/include" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/lwip-2.2.2/system" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/fatfs_r0.15b/source" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/fatfs_r0.15b/drivers" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/letter-shell-3.x/src" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/letter-shell-3.x/drivers" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/tinyusb/src" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/tinyusb/demo" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-lwip-2d-2-2e-2-2e-2-2f-src-2f-core

clean-lwip-2d-2-2e-2-2e-2-2f-src-2f-core:
	-$(RM) ./lwip-2.2.2/src/core/altcp.cyclo ./lwip-2.2.2/src/core/altcp.d ./lwip-2.2.2/src/core/altcp.o ./lwip-2.2.2/src/core/altcp.su ./lwip-2.2.2/src/core/altcp_alloc.cyclo ./lwip-2.2.2/src/core/altcp_alloc.d ./lwip-2.2.2/src/core/altcp_alloc.o ./lwip-2.2.2/src/core/altcp_alloc.su ./lwip-2.2.2/src/core/altcp_tcp.cyclo ./lwip-2.2.2/src/core/altcp_tcp.d ./lwip-2.2.2/src/core/altcp_tcp.o ./lwip-2.2.2/src/core/altcp_tcp.su ./lwip-2.2.2/src/core/def.cyclo ./lwip-2.2.2/src/core/def.d ./lwip-2.2.2/src/core/def.o ./lwip-2.2.2/src/core/def.su ./lwip-2.2.2/src/core/dns.cyclo ./lwip-2.2.2/src/core/dns.d ./lwip-2.2.2/src/core/dns.o ./lwip-2.2.2/src/core/dns.su ./lwip-2.2.2/src/core/inet_chksum.cyclo ./lwip-2.2.2/src/core/inet_chksum.d ./lwip-2.2.2/src/core/inet_chksum.o ./lwip-2.2.2/src/core/inet_chksum.su ./lwip-2.2.2/src/core/init.cyclo ./lwip-2.2.2/src/core/init.d ./lwip-2.2.2/src/core/init.o ./lwip-2.2.2/src/core/init.su ./lwip-2.2.2/src/core/ip.cyclo ./lwip-2.2.2/src/core/ip.d ./lwip-2.2.2/src/core/ip.o ./lwip-2.2.2/src/core/ip.su ./lwip-2.2.2/src/core/mem.cyclo ./lwip-2.2.2/src/core/mem.d ./lwip-2.2.2/src/core/mem.o ./lwip-2.2.2/src/core/mem.su ./lwip-2.2.2/src/core/memp.cyclo ./lwip-2.2.2/src/core/memp.d ./lwip-2.2.2/src/core/memp.o ./lwip-2.2.2/src/core/memp.su ./lwip-2.2.2/src/core/netif.cyclo ./lwip-2.2.2/src/core/netif.d ./lwip-2.2.2/src/core/netif.o ./lwip-2.2.2/src/core/netif.su ./lwip-2.2.2/src/core/pbuf.cyclo ./lwip-2.2.2/src/core/pbuf.d ./lwip-2.2.2/src/core/pbuf.o ./lwip-2.2.2/src/core/pbuf.su ./lwip-2.2.2/src/core/raw.cyclo ./lwip-2.2.2/src/core/raw.d ./lwip-2.2.2/src/core/raw.o ./lwip-2.2.2/src/core/raw.su ./lwip-2.2.2/src/core/stats.cyclo ./lwip-2.2.2/src/core/stats.d ./lwip-2.2.2/src/core/stats.o ./lwip-2.2.2/src/core/stats.su ./lwip-2.2.2/src/core/sys.cyclo ./lwip-2.2.2/src/core/sys.d ./lwip-2.2.2/src/core/sys.o ./lwip-2.2.2/src/core/sys.su ./lwip-2.2.2/src/core/tcp.cyclo ./lwip-2.2.2/src/core/tcp.d ./lwip-2.2.2/src/core/tcp.o ./lwip-2.2.2/src/core/tcp.su ./lwip-2.2.2/src/core/tcp_in.cyclo ./lwip-2.2.2/src/core/tcp_in.d ./lwip-2.2.2/src/core/tcp_in.o ./lwip-2.2.2/src/core/tcp_in.su ./lwip-2.2.2/src/core/tcp_out.cyclo ./lwip-2.2.2/src/core/tcp_out.d ./lwip-2.2.2/src/core/tcp_out.o ./lwip-2.2.2/src/core/tcp_out.su ./lwip-2.2.2/src/core/timeouts.cyclo ./lwip-2.2.2/src/core/timeouts.d ./lwip-2.2.2/src/core/timeouts.o ./lwip-2.2.2/src/core/timeouts.su ./lwip-2.2.2/src/core/udp.cyclo ./lwip-2.2.2/src/core/udp.d ./lwip-2.2.2/src/core/udp.o ./lwip-2.2.2/src/core/udp.su

.PHONY: clean-lwip-2d-2-2e-2-2e-2-2f-src-2f-core

