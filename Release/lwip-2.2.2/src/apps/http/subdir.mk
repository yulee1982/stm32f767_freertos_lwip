################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../lwip-2.2.2/src/apps/http/altcp_proxyconnect.c \
../lwip-2.2.2/src/apps/http/fs.c \
../lwip-2.2.2/src/apps/http/http_client.c \
../lwip-2.2.2/src/apps/http/httpd.c 

OBJS += \
./lwip-2.2.2/src/apps/http/altcp_proxyconnect.o \
./lwip-2.2.2/src/apps/http/fs.o \
./lwip-2.2.2/src/apps/http/http_client.o \
./lwip-2.2.2/src/apps/http/httpd.o 

C_DEPS += \
./lwip-2.2.2/src/apps/http/altcp_proxyconnect.d \
./lwip-2.2.2/src/apps/http/fs.d \
./lwip-2.2.2/src/apps/http/http_client.d \
./lwip-2.2.2/src/apps/http/httpd.d 


# Each subdirectory must supply rules for building sources it contributes
lwip-2.2.2/src/apps/http/%.o lwip-2.2.2/src/apps/http/%.su lwip-2.2.2/src/apps/http/%.cyclo: ../lwip-2.2.2/src/apps/http/%.c lwip-2.2.2/src/apps/http/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -DUSE_HAL_DRIVER -DSTM32F767xx -DUSE_FULL_LL_DRIVER -c -I../Core/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/CMSIS/Include -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/freertos/include" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/freertos/portable" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/Bsp" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/lwip-2.2.2/src/include" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/lwip-2.2.2/system" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/fatfs_r0.15b/source" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/fatfs_r0.15b/drivers" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/letter-shell-3.x/src" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/letter-shell-3.x/drivers" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/usb-framework" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/usb-framework/libusb_stm32/inc" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/usb-framework/simple_usb" -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-lwip-2d-2-2e-2-2e-2-2f-src-2f-apps-2f-http

clean-lwip-2d-2-2e-2-2e-2-2f-src-2f-apps-2f-http:
	-$(RM) ./lwip-2.2.2/src/apps/http/altcp_proxyconnect.cyclo ./lwip-2.2.2/src/apps/http/altcp_proxyconnect.d ./lwip-2.2.2/src/apps/http/altcp_proxyconnect.o ./lwip-2.2.2/src/apps/http/altcp_proxyconnect.su ./lwip-2.2.2/src/apps/http/fs.cyclo ./lwip-2.2.2/src/apps/http/fs.d ./lwip-2.2.2/src/apps/http/fs.o ./lwip-2.2.2/src/apps/http/fs.su ./lwip-2.2.2/src/apps/http/http_client.cyclo ./lwip-2.2.2/src/apps/http/http_client.d ./lwip-2.2.2/src/apps/http/http_client.o ./lwip-2.2.2/src/apps/http/http_client.su ./lwip-2.2.2/src/apps/http/httpd.cyclo ./lwip-2.2.2/src/apps/http/httpd.d ./lwip-2.2.2/src/apps/http/httpd.o ./lwip-2.2.2/src/apps/http/httpd.su

.PHONY: clean-lwip-2d-2-2e-2-2e-2-2f-src-2f-apps-2f-http

