################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../usb-framework/cherryusb/osal/usb_osal_freertos.c \
../usb-framework/cherryusb/osal/usb_osal_liteos_m.c \
../usb-framework/cherryusb/osal/usb_osal_nuttx.c \
../usb-framework/cherryusb/osal/usb_osal_rtems.c \
../usb-framework/cherryusb/osal/usb_osal_rtthread.c \
../usb-framework/cherryusb/osal/usb_osal_threadx.c \
../usb-framework/cherryusb/osal/usb_osal_yoc.c \
../usb-framework/cherryusb/osal/usb_osal_zephyr.c 

OBJS += \
./usb-framework/cherryusb/osal/usb_osal_freertos.o \
./usb-framework/cherryusb/osal/usb_osal_liteos_m.o \
./usb-framework/cherryusb/osal/usb_osal_nuttx.o \
./usb-framework/cherryusb/osal/usb_osal_rtems.o \
./usb-framework/cherryusb/osal/usb_osal_rtthread.o \
./usb-framework/cherryusb/osal/usb_osal_threadx.o \
./usb-framework/cherryusb/osal/usb_osal_yoc.o \
./usb-framework/cherryusb/osal/usb_osal_zephyr.o 

C_DEPS += \
./usb-framework/cherryusb/osal/usb_osal_freertos.d \
./usb-framework/cherryusb/osal/usb_osal_liteos_m.d \
./usb-framework/cherryusb/osal/usb_osal_nuttx.d \
./usb-framework/cherryusb/osal/usb_osal_rtems.d \
./usb-framework/cherryusb/osal/usb_osal_rtthread.d \
./usb-framework/cherryusb/osal/usb_osal_threadx.d \
./usb-framework/cherryusb/osal/usb_osal_yoc.d \
./usb-framework/cherryusb/osal/usb_osal_zephyr.d 


# Each subdirectory must supply rules for building sources it contributes
usb-framework/cherryusb/osal/%.o usb-framework/cherryusb/osal/%.su usb-framework/cherryusb/osal/%.cyclo: ../usb-framework/cherryusb/osal/%.c usb-framework/cherryusb/osal/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F767xx -DUSE_FULL_LL_DRIVER -c -I../Core/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/CMSIS/Include -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/freertos/portable" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/freertos/include" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/Bsp" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/lwip-2.2.2/src/include" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/lwip-2.2.2/system" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/fatfs_r0.15b/source" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/fatfs_r0.15b/drivers" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/letter-shell-3.x/src" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/letter-shell-3.x/drivers" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/usb-framework" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/usb-framework/libusb_stm32/inc" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/usb-framework/simple_usb" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-usb-2d-framework-2f-cherryusb-2f-osal

clean-usb-2d-framework-2f-cherryusb-2f-osal:
	-$(RM) ./usb-framework/cherryusb/osal/usb_osal_freertos.cyclo ./usb-framework/cherryusb/osal/usb_osal_freertos.d ./usb-framework/cherryusb/osal/usb_osal_freertos.o ./usb-framework/cherryusb/osal/usb_osal_freertos.su ./usb-framework/cherryusb/osal/usb_osal_liteos_m.cyclo ./usb-framework/cherryusb/osal/usb_osal_liteos_m.d ./usb-framework/cherryusb/osal/usb_osal_liteos_m.o ./usb-framework/cherryusb/osal/usb_osal_liteos_m.su ./usb-framework/cherryusb/osal/usb_osal_nuttx.cyclo ./usb-framework/cherryusb/osal/usb_osal_nuttx.d ./usb-framework/cherryusb/osal/usb_osal_nuttx.o ./usb-framework/cherryusb/osal/usb_osal_nuttx.su ./usb-framework/cherryusb/osal/usb_osal_rtems.cyclo ./usb-framework/cherryusb/osal/usb_osal_rtems.d ./usb-framework/cherryusb/osal/usb_osal_rtems.o ./usb-framework/cherryusb/osal/usb_osal_rtems.su ./usb-framework/cherryusb/osal/usb_osal_rtthread.cyclo ./usb-framework/cherryusb/osal/usb_osal_rtthread.d ./usb-framework/cherryusb/osal/usb_osal_rtthread.o ./usb-framework/cherryusb/osal/usb_osal_rtthread.su ./usb-framework/cherryusb/osal/usb_osal_threadx.cyclo ./usb-framework/cherryusb/osal/usb_osal_threadx.d ./usb-framework/cherryusb/osal/usb_osal_threadx.o ./usb-framework/cherryusb/osal/usb_osal_threadx.su ./usb-framework/cherryusb/osal/usb_osal_yoc.cyclo ./usb-framework/cherryusb/osal/usb_osal_yoc.d ./usb-framework/cherryusb/osal/usb_osal_yoc.o ./usb-framework/cherryusb/osal/usb_osal_yoc.su ./usb-framework/cherryusb/osal/usb_osal_zephyr.cyclo ./usb-framework/cherryusb/osal/usb_osal_zephyr.d ./usb-framework/cherryusb/osal/usb_osal_zephyr.o ./usb-framework/cherryusb/osal/usb_osal_zephyr.su

.PHONY: clean-usb-2d-framework-2f-cherryusb-2f-osal

