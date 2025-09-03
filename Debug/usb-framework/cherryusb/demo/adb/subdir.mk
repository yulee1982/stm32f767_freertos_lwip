################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../usb-framework/cherryusb/demo/adb/cherrysh_port.c \
../usb-framework/cherryusb/demo/adb/usbd_adb_template.c 

OBJS += \
./usb-framework/cherryusb/demo/adb/cherrysh_port.o \
./usb-framework/cherryusb/demo/adb/usbd_adb_template.o 

C_DEPS += \
./usb-framework/cherryusb/demo/adb/cherrysh_port.d \
./usb-framework/cherryusb/demo/adb/usbd_adb_template.d 


# Each subdirectory must supply rules for building sources it contributes
usb-framework/cherryusb/demo/adb/%.o usb-framework/cherryusb/demo/adb/%.su usb-framework/cherryusb/demo/adb/%.cyclo: ../usb-framework/cherryusb/demo/adb/%.c usb-framework/cherryusb/demo/adb/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F767xx -DUSE_FULL_LL_DRIVER -c -I../Core/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/CMSIS/Include -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/freertos/portable" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/freertos/include" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/Bsp" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/lwip-2.2.2/src/include" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/lwip-2.2.2/system" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/fatfs_r0.15b/source" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/fatfs_r0.15b/drivers" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/letter-shell-3.x/src" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/letter-shell-3.x/drivers" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/usb-framework" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/usb-framework/libusb_stm32/inc" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/usb-framework/simple_usb" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-usb-2d-framework-2f-cherryusb-2f-demo-2f-adb

clean-usb-2d-framework-2f-cherryusb-2f-demo-2f-adb:
	-$(RM) ./usb-framework/cherryusb/demo/adb/cherrysh_port.cyclo ./usb-framework/cherryusb/demo/adb/cherrysh_port.d ./usb-framework/cherryusb/demo/adb/cherrysh_port.o ./usb-framework/cherryusb/demo/adb/cherrysh_port.su ./usb-framework/cherryusb/demo/adb/usbd_adb_template.cyclo ./usb-framework/cherryusb/demo/adb/usbd_adb_template.d ./usb-framework/cherryusb/demo/adb/usbd_adb_template.o ./usb-framework/cherryusb/demo/adb/usbd_adb_template.su

.PHONY: clean-usb-2d-framework-2f-cherryusb-2f-demo-2f-adb

