################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../lwip/core/altcp.c \
../lwip/core/altcp_alloc.c \
../lwip/core/altcp_tcp.c \
../lwip/core/def.c \
../lwip/core/dns.c \
../lwip/core/inet_chksum.c \
../lwip/core/init.c \
../lwip/core/ip.c \
../lwip/core/mem.c \
../lwip/core/memp.c \
../lwip/core/netif.c \
../lwip/core/pbuf.c \
../lwip/core/raw.c \
../lwip/core/stats.c \
../lwip/core/sys.c \
../lwip/core/tcp.c \
../lwip/core/tcp_in.c \
../lwip/core/tcp_out.c \
../lwip/core/timeouts.c \
../lwip/core/udp.c 

OBJS += \
./lwip/core/altcp.o \
./lwip/core/altcp_alloc.o \
./lwip/core/altcp_tcp.o \
./lwip/core/def.o \
./lwip/core/dns.o \
./lwip/core/inet_chksum.o \
./lwip/core/init.o \
./lwip/core/ip.o \
./lwip/core/mem.o \
./lwip/core/memp.o \
./lwip/core/netif.o \
./lwip/core/pbuf.o \
./lwip/core/raw.o \
./lwip/core/stats.o \
./lwip/core/sys.o \
./lwip/core/tcp.o \
./lwip/core/tcp_in.o \
./lwip/core/tcp_out.o \
./lwip/core/timeouts.o \
./lwip/core/udp.o 

C_DEPS += \
./lwip/core/altcp.d \
./lwip/core/altcp_alloc.d \
./lwip/core/altcp_tcp.d \
./lwip/core/def.d \
./lwip/core/dns.d \
./lwip/core/inet_chksum.d \
./lwip/core/init.d \
./lwip/core/ip.d \
./lwip/core/mem.d \
./lwip/core/memp.d \
./lwip/core/netif.d \
./lwip/core/pbuf.d \
./lwip/core/raw.d \
./lwip/core/stats.d \
./lwip/core/sys.d \
./lwip/core/tcp.d \
./lwip/core/tcp_in.d \
./lwip/core/tcp_out.d \
./lwip/core/timeouts.d \
./lwip/core/udp.d 


# Each subdirectory must supply rules for building sources it contributes
lwip/core/%.o lwip/core/%.su lwip/core/%.cyclo: ../lwip/core/%.c lwip/core/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F767xx -DUSE_FULL_LL_DRIVER -c -I../Core/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/CMSIS/Include -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/freertos/portable" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/freertos/include" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/lwip/include" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/Bsp" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-lwip-2f-core

clean-lwip-2f-core:
	-$(RM) ./lwip/core/altcp.cyclo ./lwip/core/altcp.d ./lwip/core/altcp.o ./lwip/core/altcp.su ./lwip/core/altcp_alloc.cyclo ./lwip/core/altcp_alloc.d ./lwip/core/altcp_alloc.o ./lwip/core/altcp_alloc.su ./lwip/core/altcp_tcp.cyclo ./lwip/core/altcp_tcp.d ./lwip/core/altcp_tcp.o ./lwip/core/altcp_tcp.su ./lwip/core/def.cyclo ./lwip/core/def.d ./lwip/core/def.o ./lwip/core/def.su ./lwip/core/dns.cyclo ./lwip/core/dns.d ./lwip/core/dns.o ./lwip/core/dns.su ./lwip/core/inet_chksum.cyclo ./lwip/core/inet_chksum.d ./lwip/core/inet_chksum.o ./lwip/core/inet_chksum.su ./lwip/core/init.cyclo ./lwip/core/init.d ./lwip/core/init.o ./lwip/core/init.su ./lwip/core/ip.cyclo ./lwip/core/ip.d ./lwip/core/ip.o ./lwip/core/ip.su ./lwip/core/mem.cyclo ./lwip/core/mem.d ./lwip/core/mem.o ./lwip/core/mem.su ./lwip/core/memp.cyclo ./lwip/core/memp.d ./lwip/core/memp.o ./lwip/core/memp.su ./lwip/core/netif.cyclo ./lwip/core/netif.d ./lwip/core/netif.o ./lwip/core/netif.su ./lwip/core/pbuf.cyclo ./lwip/core/pbuf.d ./lwip/core/pbuf.o ./lwip/core/pbuf.su ./lwip/core/raw.cyclo ./lwip/core/raw.d ./lwip/core/raw.o ./lwip/core/raw.su ./lwip/core/stats.cyclo ./lwip/core/stats.d ./lwip/core/stats.o ./lwip/core/stats.su ./lwip/core/sys.cyclo ./lwip/core/sys.d ./lwip/core/sys.o ./lwip/core/sys.su ./lwip/core/tcp.cyclo ./lwip/core/tcp.d ./lwip/core/tcp.o ./lwip/core/tcp.su ./lwip/core/tcp_in.cyclo ./lwip/core/tcp_in.d ./lwip/core/tcp_in.o ./lwip/core/tcp_in.su ./lwip/core/tcp_out.cyclo ./lwip/core/tcp_out.d ./lwip/core/tcp_out.o ./lwip/core/tcp_out.su ./lwip/core/timeouts.cyclo ./lwip/core/timeouts.d ./lwip/core/timeouts.o ./lwip/core/timeouts.su ./lwip/core/udp.cyclo ./lwip/core/udp.d ./lwip/core/udp.o ./lwip/core/udp.su

.PHONY: clean-lwip-2f-core

