################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../lwip/api/api_lib.c \
../lwip/api/api_msg.c \
../lwip/api/err.c \
../lwip/api/if_api.c \
../lwip/api/netbuf.c \
../lwip/api/netdb.c \
../lwip/api/netifapi.c \
../lwip/api/sockets.c \
../lwip/api/tcpip.c 

OBJS += \
./lwip/api/api_lib.o \
./lwip/api/api_msg.o \
./lwip/api/err.o \
./lwip/api/if_api.o \
./lwip/api/netbuf.o \
./lwip/api/netdb.o \
./lwip/api/netifapi.o \
./lwip/api/sockets.o \
./lwip/api/tcpip.o 

C_DEPS += \
./lwip/api/api_lib.d \
./lwip/api/api_msg.d \
./lwip/api/err.d \
./lwip/api/if_api.d \
./lwip/api/netbuf.d \
./lwip/api/netdb.d \
./lwip/api/netifapi.d \
./lwip/api/sockets.d \
./lwip/api/tcpip.d 


# Each subdirectory must supply rules for building sources it contributes
lwip/api/%.o lwip/api/%.su lwip/api/%.cyclo: ../lwip/api/%.c lwip/api/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F767xx -DUSE_FULL_LL_DRIVER -c -I../Core/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/CMSIS/Include -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/freertos/portable" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/freertos/include" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/lwip/include" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/Bsp" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-lwip-2f-api

clean-lwip-2f-api:
	-$(RM) ./lwip/api/api_lib.cyclo ./lwip/api/api_lib.d ./lwip/api/api_lib.o ./lwip/api/api_lib.su ./lwip/api/api_msg.cyclo ./lwip/api/api_msg.d ./lwip/api/api_msg.o ./lwip/api/api_msg.su ./lwip/api/err.cyclo ./lwip/api/err.d ./lwip/api/err.o ./lwip/api/err.su ./lwip/api/if_api.cyclo ./lwip/api/if_api.d ./lwip/api/if_api.o ./lwip/api/if_api.su ./lwip/api/netbuf.cyclo ./lwip/api/netbuf.d ./lwip/api/netbuf.o ./lwip/api/netbuf.su ./lwip/api/netdb.cyclo ./lwip/api/netdb.d ./lwip/api/netdb.o ./lwip/api/netdb.su ./lwip/api/netifapi.cyclo ./lwip/api/netifapi.d ./lwip/api/netifapi.o ./lwip/api/netifapi.su ./lwip/api/sockets.cyclo ./lwip/api/sockets.d ./lwip/api/sockets.o ./lwip/api/sockets.su ./lwip/api/tcpip.cyclo ./lwip/api/tcpip.d ./lwip/api/tcpip.o ./lwip/api/tcpip.su

.PHONY: clean-lwip-2f-api

