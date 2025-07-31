################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../lwip-2.2.2/src/api/api_lib.c \
../lwip-2.2.2/src/api/api_msg.c \
../lwip-2.2.2/src/api/err.c \
../lwip-2.2.2/src/api/if_api.c \
../lwip-2.2.2/src/api/netbuf.c \
../lwip-2.2.2/src/api/netdb.c \
../lwip-2.2.2/src/api/netifapi.c \
../lwip-2.2.2/src/api/sockets.c \
../lwip-2.2.2/src/api/tcpip.c 

OBJS += \
./lwip-2.2.2/src/api/api_lib.o \
./lwip-2.2.2/src/api/api_msg.o \
./lwip-2.2.2/src/api/err.o \
./lwip-2.2.2/src/api/if_api.o \
./lwip-2.2.2/src/api/netbuf.o \
./lwip-2.2.2/src/api/netdb.o \
./lwip-2.2.2/src/api/netifapi.o \
./lwip-2.2.2/src/api/sockets.o \
./lwip-2.2.2/src/api/tcpip.o 

C_DEPS += \
./lwip-2.2.2/src/api/api_lib.d \
./lwip-2.2.2/src/api/api_msg.d \
./lwip-2.2.2/src/api/err.d \
./lwip-2.2.2/src/api/if_api.d \
./lwip-2.2.2/src/api/netbuf.d \
./lwip-2.2.2/src/api/netdb.d \
./lwip-2.2.2/src/api/netifapi.d \
./lwip-2.2.2/src/api/sockets.d \
./lwip-2.2.2/src/api/tcpip.d 


# Each subdirectory must supply rules for building sources it contributes
lwip-2.2.2/src/api/%.o lwip-2.2.2/src/api/%.su lwip-2.2.2/src/api/%.cyclo: ../lwip-2.2.2/src/api/%.c lwip-2.2.2/src/api/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -DUSE_HAL_DRIVER -DSTM32F767xx -DUSE_FULL_LL_DRIVER -c -I../Core/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/CMSIS/Include -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/freertos/include" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/freertos/portable" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/Bsp" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/lwip-2.2.2/src/include" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/lwip-2.2.2/system" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/fatfs_r0.15b/source" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/fatfs_r0.15b/drivers" -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-lwip-2d-2-2e-2-2e-2-2f-src-2f-api

clean-lwip-2d-2-2e-2-2e-2-2f-src-2f-api:
	-$(RM) ./lwip-2.2.2/src/api/api_lib.cyclo ./lwip-2.2.2/src/api/api_lib.d ./lwip-2.2.2/src/api/api_lib.o ./lwip-2.2.2/src/api/api_lib.su ./lwip-2.2.2/src/api/api_msg.cyclo ./lwip-2.2.2/src/api/api_msg.d ./lwip-2.2.2/src/api/api_msg.o ./lwip-2.2.2/src/api/api_msg.su ./lwip-2.2.2/src/api/err.cyclo ./lwip-2.2.2/src/api/err.d ./lwip-2.2.2/src/api/err.o ./lwip-2.2.2/src/api/err.su ./lwip-2.2.2/src/api/if_api.cyclo ./lwip-2.2.2/src/api/if_api.d ./lwip-2.2.2/src/api/if_api.o ./lwip-2.2.2/src/api/if_api.su ./lwip-2.2.2/src/api/netbuf.cyclo ./lwip-2.2.2/src/api/netbuf.d ./lwip-2.2.2/src/api/netbuf.o ./lwip-2.2.2/src/api/netbuf.su ./lwip-2.2.2/src/api/netdb.cyclo ./lwip-2.2.2/src/api/netdb.d ./lwip-2.2.2/src/api/netdb.o ./lwip-2.2.2/src/api/netdb.su ./lwip-2.2.2/src/api/netifapi.cyclo ./lwip-2.2.2/src/api/netifapi.d ./lwip-2.2.2/src/api/netifapi.o ./lwip-2.2.2/src/api/netifapi.su ./lwip-2.2.2/src/api/sockets.cyclo ./lwip-2.2.2/src/api/sockets.d ./lwip-2.2.2/src/api/sockets.o ./lwip-2.2.2/src/api/sockets.su ./lwip-2.2.2/src/api/tcpip.cyclo ./lwip-2.2.2/src/api/tcpip.d ./lwip-2.2.2/src/api/tcpip.o ./lwip-2.2.2/src/api/tcpip.su

.PHONY: clean-lwip-2d-2-2e-2-2e-2-2f-src-2f-api

