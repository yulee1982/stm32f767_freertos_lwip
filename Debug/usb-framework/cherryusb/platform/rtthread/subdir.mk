################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../usb-framework/cherryusb/platform/rtthread/usb_check.c \
../usb-framework/cherryusb/platform/rtthread/usb_msh.c \
../usb-framework/cherryusb/platform/rtthread/usbd_adb_shell.c \
../usb-framework/cherryusb/platform/rtthread/usbd_serial.c \
../usb-framework/cherryusb/platform/rtthread/usbh_dfs.c \
../usb-framework/cherryusb/platform/rtthread/usbh_lwip.c \
../usb-framework/cherryusb/platform/rtthread/usbh_serial.c 

OBJS += \
./usb-framework/cherryusb/platform/rtthread/usb_check.o \
./usb-framework/cherryusb/platform/rtthread/usb_msh.o \
./usb-framework/cherryusb/platform/rtthread/usbd_adb_shell.o \
./usb-framework/cherryusb/platform/rtthread/usbd_serial.o \
./usb-framework/cherryusb/platform/rtthread/usbh_dfs.o \
./usb-framework/cherryusb/platform/rtthread/usbh_lwip.o \
./usb-framework/cherryusb/platform/rtthread/usbh_serial.o 

C_DEPS += \
./usb-framework/cherryusb/platform/rtthread/usb_check.d \
./usb-framework/cherryusb/platform/rtthread/usb_msh.d \
./usb-framework/cherryusb/platform/rtthread/usbd_adb_shell.d \
./usb-framework/cherryusb/platform/rtthread/usbd_serial.d \
./usb-framework/cherryusb/platform/rtthread/usbh_dfs.d \
./usb-framework/cherryusb/platform/rtthread/usbh_lwip.d \
./usb-framework/cherryusb/platform/rtthread/usbh_serial.d 


# Each subdirectory must supply rules for building sources it contributes
usb-framework/cherryusb/platform/rtthread/%.o usb-framework/cherryusb/platform/rtthread/%.su usb-framework/cherryusb/platform/rtthread/%.cyclo: ../usb-framework/cherryusb/platform/rtthread/%.c usb-framework/cherryusb/platform/rtthread/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F767xx -DUSE_FULL_LL_DRIVER -c -I../Core/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/CMSIS/Include -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/freertos/portable" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/freertos/include" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/Bsp" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/lwip-2.2.2/src/include" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/lwip-2.2.2/system" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/fatfs_r0.15b/source" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/fatfs_r0.15b/drivers" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/letter-shell-3.x/src" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/letter-shell-3.x/drivers" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/usb-framework" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/usb-framework/libusb_stm32/inc" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/usb-framework/simple_usb" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-usb-2d-framework-2f-cherryusb-2f-platform-2f-rtthread

clean-usb-2d-framework-2f-cherryusb-2f-platform-2f-rtthread:
	-$(RM) ./usb-framework/cherryusb/platform/rtthread/usb_check.cyclo ./usb-framework/cherryusb/platform/rtthread/usb_check.d ./usb-framework/cherryusb/platform/rtthread/usb_check.o ./usb-framework/cherryusb/platform/rtthread/usb_check.su ./usb-framework/cherryusb/platform/rtthread/usb_msh.cyclo ./usb-framework/cherryusb/platform/rtthread/usb_msh.d ./usb-framework/cherryusb/platform/rtthread/usb_msh.o ./usb-framework/cherryusb/platform/rtthread/usb_msh.su ./usb-framework/cherryusb/platform/rtthread/usbd_adb_shell.cyclo ./usb-framework/cherryusb/platform/rtthread/usbd_adb_shell.d ./usb-framework/cherryusb/platform/rtthread/usbd_adb_shell.o ./usb-framework/cherryusb/platform/rtthread/usbd_adb_shell.su ./usb-framework/cherryusb/platform/rtthread/usbd_serial.cyclo ./usb-framework/cherryusb/platform/rtthread/usbd_serial.d ./usb-framework/cherryusb/platform/rtthread/usbd_serial.o ./usb-framework/cherryusb/platform/rtthread/usbd_serial.su ./usb-framework/cherryusb/platform/rtthread/usbh_dfs.cyclo ./usb-framework/cherryusb/platform/rtthread/usbh_dfs.d ./usb-framework/cherryusb/platform/rtthread/usbh_dfs.o ./usb-framework/cherryusb/platform/rtthread/usbh_dfs.su ./usb-framework/cherryusb/platform/rtthread/usbh_lwip.cyclo ./usb-framework/cherryusb/platform/rtthread/usbh_lwip.d ./usb-framework/cherryusb/platform/rtthread/usbh_lwip.o ./usb-framework/cherryusb/platform/rtthread/usbh_lwip.su ./usb-framework/cherryusb/platform/rtthread/usbh_serial.cyclo ./usb-framework/cherryusb/platform/rtthread/usbh_serial.d ./usb-framework/cherryusb/platform/rtthread/usbh_serial.o ./usb-framework/cherryusb/platform/rtthread/usbh_serial.su

.PHONY: clean-usb-2d-framework-2f-cherryusb-2f-platform-2f-rtthread

