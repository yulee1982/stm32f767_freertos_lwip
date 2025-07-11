################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../lwip/apps/http/altcp_proxyconnect.c \
../lwip/apps/http/fs.c \
../lwip/apps/http/http_client.c \
../lwip/apps/http/httpd.c 

OBJS += \
./lwip/apps/http/altcp_proxyconnect.o \
./lwip/apps/http/fs.o \
./lwip/apps/http/http_client.o \
./lwip/apps/http/httpd.o 

C_DEPS += \
./lwip/apps/http/altcp_proxyconnect.d \
./lwip/apps/http/fs.d \
./lwip/apps/http/http_client.d \
./lwip/apps/http/httpd.d 


# Each subdirectory must supply rules for building sources it contributes
lwip/apps/http/%.o lwip/apps/http/%.su lwip/apps/http/%.cyclo: ../lwip/apps/http/%.c lwip/apps/http/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F767xx -DUSE_FULL_LL_DRIVER -c -I../Core/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/CMSIS/Include -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/freertos/portable" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/freertos/include" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/lwip/include" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/Bsp" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-lwip-2f-apps-2f-http

clean-lwip-2f-apps-2f-http:
	-$(RM) ./lwip/apps/http/altcp_proxyconnect.cyclo ./lwip/apps/http/altcp_proxyconnect.d ./lwip/apps/http/altcp_proxyconnect.o ./lwip/apps/http/altcp_proxyconnect.su ./lwip/apps/http/fs.cyclo ./lwip/apps/http/fs.d ./lwip/apps/http/fs.o ./lwip/apps/http/fs.su ./lwip/apps/http/http_client.cyclo ./lwip/apps/http/http_client.d ./lwip/apps/http/http_client.o ./lwip/apps/http/http_client.su ./lwip/apps/http/httpd.cyclo ./lwip/apps/http/httpd.d ./lwip/apps/http/httpd.o ./lwip/apps/http/httpd.su

.PHONY: clean-lwip-2f-apps-2f-http

