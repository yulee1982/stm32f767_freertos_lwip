################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../lwip-2.2.2/src/netif/ppp/polarssl/arc4.c \
../lwip-2.2.2/src/netif/ppp/polarssl/des.c \
../lwip-2.2.2/src/netif/ppp/polarssl/md4.c \
../lwip-2.2.2/src/netif/ppp/polarssl/md5.c \
../lwip-2.2.2/src/netif/ppp/polarssl/sha1.c 

OBJS += \
./lwip-2.2.2/src/netif/ppp/polarssl/arc4.o \
./lwip-2.2.2/src/netif/ppp/polarssl/des.o \
./lwip-2.2.2/src/netif/ppp/polarssl/md4.o \
./lwip-2.2.2/src/netif/ppp/polarssl/md5.o \
./lwip-2.2.2/src/netif/ppp/polarssl/sha1.o 

C_DEPS += \
./lwip-2.2.2/src/netif/ppp/polarssl/arc4.d \
./lwip-2.2.2/src/netif/ppp/polarssl/des.d \
./lwip-2.2.2/src/netif/ppp/polarssl/md4.d \
./lwip-2.2.2/src/netif/ppp/polarssl/md5.d \
./lwip-2.2.2/src/netif/ppp/polarssl/sha1.d 


# Each subdirectory must supply rules for building sources it contributes
lwip-2.2.2/src/netif/ppp/polarssl/%.o lwip-2.2.2/src/netif/ppp/polarssl/%.su lwip-2.2.2/src/netif/ppp/polarssl/%.cyclo: ../lwip-2.2.2/src/netif/ppp/polarssl/%.c lwip-2.2.2/src/netif/ppp/polarssl/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F767xx -DUSE_FULL_LL_DRIVER -c -I../Core/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/CMSIS/Include -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/freertos/portable" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/freertos/include" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/Bsp" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/lwip-2.2.2/src/include" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/lwip-2.2.2/system" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/letter-shell-3.x/src" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/letter-shell-3.x/drivers" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/fatfs/source" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/fatfs/drivers" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/tinyusb/src" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/tinyusb/demo" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-lwip-2d-2-2e-2-2e-2-2f-src-2f-netif-2f-ppp-2f-polarssl

clean-lwip-2d-2-2e-2-2e-2-2f-src-2f-netif-2f-ppp-2f-polarssl:
	-$(RM) ./lwip-2.2.2/src/netif/ppp/polarssl/arc4.cyclo ./lwip-2.2.2/src/netif/ppp/polarssl/arc4.d ./lwip-2.2.2/src/netif/ppp/polarssl/arc4.o ./lwip-2.2.2/src/netif/ppp/polarssl/arc4.su ./lwip-2.2.2/src/netif/ppp/polarssl/des.cyclo ./lwip-2.2.2/src/netif/ppp/polarssl/des.d ./lwip-2.2.2/src/netif/ppp/polarssl/des.o ./lwip-2.2.2/src/netif/ppp/polarssl/des.su ./lwip-2.2.2/src/netif/ppp/polarssl/md4.cyclo ./lwip-2.2.2/src/netif/ppp/polarssl/md4.d ./lwip-2.2.2/src/netif/ppp/polarssl/md4.o ./lwip-2.2.2/src/netif/ppp/polarssl/md4.su ./lwip-2.2.2/src/netif/ppp/polarssl/md5.cyclo ./lwip-2.2.2/src/netif/ppp/polarssl/md5.d ./lwip-2.2.2/src/netif/ppp/polarssl/md5.o ./lwip-2.2.2/src/netif/ppp/polarssl/md5.su ./lwip-2.2.2/src/netif/ppp/polarssl/sha1.cyclo ./lwip-2.2.2/src/netif/ppp/polarssl/sha1.d ./lwip-2.2.2/src/netif/ppp/polarssl/sha1.o ./lwip-2.2.2/src/netif/ppp/polarssl/sha1.su

.PHONY: clean-lwip-2d-2-2e-2-2e-2-2f-src-2f-netif-2f-ppp-2f-polarssl

