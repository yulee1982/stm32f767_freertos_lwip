################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../lwip/netif/ppp/polarssl/arc4.c \
../lwip/netif/ppp/polarssl/des.c \
../lwip/netif/ppp/polarssl/md4.c \
../lwip/netif/ppp/polarssl/md5.c \
../lwip/netif/ppp/polarssl/sha1.c 

OBJS += \
./lwip/netif/ppp/polarssl/arc4.o \
./lwip/netif/ppp/polarssl/des.o \
./lwip/netif/ppp/polarssl/md4.o \
./lwip/netif/ppp/polarssl/md5.o \
./lwip/netif/ppp/polarssl/sha1.o 

C_DEPS += \
./lwip/netif/ppp/polarssl/arc4.d \
./lwip/netif/ppp/polarssl/des.d \
./lwip/netif/ppp/polarssl/md4.d \
./lwip/netif/ppp/polarssl/md5.d \
./lwip/netif/ppp/polarssl/sha1.d 


# Each subdirectory must supply rules for building sources it contributes
lwip/netif/ppp/polarssl/%.o lwip/netif/ppp/polarssl/%.su lwip/netif/ppp/polarssl/%.cyclo: ../lwip/netif/ppp/polarssl/%.c lwip/netif/ppp/polarssl/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F767xx -DUSE_FULL_LL_DRIVER -c -I../Core/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/CMSIS/Include -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/freertos/portable" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/freertos/include" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/lwip/include" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/Bsp" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-lwip-2f-netif-2f-ppp-2f-polarssl

clean-lwip-2f-netif-2f-ppp-2f-polarssl:
	-$(RM) ./lwip/netif/ppp/polarssl/arc4.cyclo ./lwip/netif/ppp/polarssl/arc4.d ./lwip/netif/ppp/polarssl/arc4.o ./lwip/netif/ppp/polarssl/arc4.su ./lwip/netif/ppp/polarssl/des.cyclo ./lwip/netif/ppp/polarssl/des.d ./lwip/netif/ppp/polarssl/des.o ./lwip/netif/ppp/polarssl/des.su ./lwip/netif/ppp/polarssl/md4.cyclo ./lwip/netif/ppp/polarssl/md4.d ./lwip/netif/ppp/polarssl/md4.o ./lwip/netif/ppp/polarssl/md4.su ./lwip/netif/ppp/polarssl/md5.cyclo ./lwip/netif/ppp/polarssl/md5.d ./lwip/netif/ppp/polarssl/md5.o ./lwip/netif/ppp/polarssl/md5.su ./lwip/netif/ppp/polarssl/sha1.cyclo ./lwip/netif/ppp/polarssl/sha1.d ./lwip/netif/ppp/polarssl/sha1.o ./lwip/netif/ppp/polarssl/sha1.su

.PHONY: clean-lwip-2f-netif-2f-ppp-2f-polarssl

