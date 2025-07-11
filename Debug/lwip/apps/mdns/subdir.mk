################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../lwip/apps/mdns/mdns.c \
../lwip/apps/mdns/mdns_domain.c \
../lwip/apps/mdns/mdns_out.c 

OBJS += \
./lwip/apps/mdns/mdns.o \
./lwip/apps/mdns/mdns_domain.o \
./lwip/apps/mdns/mdns_out.o 

C_DEPS += \
./lwip/apps/mdns/mdns.d \
./lwip/apps/mdns/mdns_domain.d \
./lwip/apps/mdns/mdns_out.d 


# Each subdirectory must supply rules for building sources it contributes
lwip/apps/mdns/%.o lwip/apps/mdns/%.su lwip/apps/mdns/%.cyclo: ../lwip/apps/mdns/%.c lwip/apps/mdns/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F767xx -DUSE_FULL_LL_DRIVER -c -I../Core/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/CMSIS/Include -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/freertos/portable" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/freertos/include" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/lwip/include" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/Bsp" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-lwip-2f-apps-2f-mdns

clean-lwip-2f-apps-2f-mdns:
	-$(RM) ./lwip/apps/mdns/mdns.cyclo ./lwip/apps/mdns/mdns.d ./lwip/apps/mdns/mdns.o ./lwip/apps/mdns/mdns.su ./lwip/apps/mdns/mdns_domain.cyclo ./lwip/apps/mdns/mdns_domain.d ./lwip/apps/mdns/mdns_domain.o ./lwip/apps/mdns/mdns_domain.su ./lwip/apps/mdns/mdns_out.cyclo ./lwip/apps/mdns/mdns_out.d ./lwip/apps/mdns/mdns_out.o ./lwip/apps/mdns/mdns_out.su

.PHONY: clean-lwip-2f-apps-2f-mdns

