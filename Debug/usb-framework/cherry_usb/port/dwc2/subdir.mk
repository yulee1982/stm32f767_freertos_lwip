################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../usb-framework/cherry_usb/port/dwc2/usb_dc_dwc2.c \
../usb-framework/cherry_usb/port/dwc2/usb_glue_hc.c \
../usb-framework/cherry_usb/port/dwc2/usb_glue_kendryte.c \
../usb-framework/cherry_usb/port/dwc2/usb_glue_st.c \
../usb-framework/cherry_usb/port/dwc2/usb_hc_dwc2.c 

OBJS += \
./usb-framework/cherry_usb/port/dwc2/usb_dc_dwc2.o \
./usb-framework/cherry_usb/port/dwc2/usb_glue_hc.o \
./usb-framework/cherry_usb/port/dwc2/usb_glue_kendryte.o \
./usb-framework/cherry_usb/port/dwc2/usb_glue_st.o \
./usb-framework/cherry_usb/port/dwc2/usb_hc_dwc2.o 

C_DEPS += \
./usb-framework/cherry_usb/port/dwc2/usb_dc_dwc2.d \
./usb-framework/cherry_usb/port/dwc2/usb_glue_hc.d \
./usb-framework/cherry_usb/port/dwc2/usb_glue_kendryte.d \
./usb-framework/cherry_usb/port/dwc2/usb_glue_st.d \
./usb-framework/cherry_usb/port/dwc2/usb_hc_dwc2.d 


# Each subdirectory must supply rules for building sources it contributes
usb-framework/cherry_usb/port/dwc2/%.o usb-framework/cherry_usb/port/dwc2/%.su usb-framework/cherry_usb/port/dwc2/%.cyclo: ../usb-framework/cherry_usb/port/dwc2/%.c usb-framework/cherry_usb/port/dwc2/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F767xx -DUSE_FULL_LL_DRIVER -c -I../Core/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/CMSIS/Include -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/freertos/portable" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/freertos/include" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/Bsp" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/lwip-2.2.2/src/include" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/lwip-2.2.2/system" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/fatfs_r0.15b/source" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/fatfs_r0.15b/drivers" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/letter-shell-3.x/src" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/letter-shell-3.x/drivers" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/usb-framework" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/usb-framework/libusb_stm32/inc" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/usb-framework/simple_usb" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/usb-framework/cherry_usb/core" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/usb-framework/cherry_usb/common" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/usb-framework/cherry_usb/port/dwc2" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/usb-framework/cherry_usb/port/fsdev" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/usb-framework/cherry_usb/class/adb" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/usb-framework/cherry_usb/class/aoa" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/usb-framework/cherry_usb/class/audio" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/usb-framework/cherry_usb/class/cdc" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/usb-framework/cherry_usb/class/dfu" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/usb-framework/cherry_usb/class/hid" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/usb-framework/cherry_usb/class/midi" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/usb-framework/cherry_usb/class/hub" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/usb-framework/cherry_usb/class/msc" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/usb-framework/cherry_usb/class/mtp" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/usb-framework/cherry_usb/class/vendor" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/usb-framework/cherry_usb/class/video" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/usb-framework/cherry_usb/class/wireless" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-usb-2d-framework-2f-cherry_usb-2f-port-2f-dwc2

clean-usb-2d-framework-2f-cherry_usb-2f-port-2f-dwc2:
	-$(RM) ./usb-framework/cherry_usb/port/dwc2/usb_dc_dwc2.cyclo ./usb-framework/cherry_usb/port/dwc2/usb_dc_dwc2.d ./usb-framework/cherry_usb/port/dwc2/usb_dc_dwc2.o ./usb-framework/cherry_usb/port/dwc2/usb_dc_dwc2.su ./usb-framework/cherry_usb/port/dwc2/usb_glue_hc.cyclo ./usb-framework/cherry_usb/port/dwc2/usb_glue_hc.d ./usb-framework/cherry_usb/port/dwc2/usb_glue_hc.o ./usb-framework/cherry_usb/port/dwc2/usb_glue_hc.su ./usb-framework/cherry_usb/port/dwc2/usb_glue_kendryte.cyclo ./usb-framework/cherry_usb/port/dwc2/usb_glue_kendryte.d ./usb-framework/cherry_usb/port/dwc2/usb_glue_kendryte.o ./usb-framework/cherry_usb/port/dwc2/usb_glue_kendryte.su ./usb-framework/cherry_usb/port/dwc2/usb_glue_st.cyclo ./usb-framework/cherry_usb/port/dwc2/usb_glue_st.d ./usb-framework/cherry_usb/port/dwc2/usb_glue_st.o ./usb-framework/cherry_usb/port/dwc2/usb_glue_st.su ./usb-framework/cherry_usb/port/dwc2/usb_hc_dwc2.cyclo ./usb-framework/cherry_usb/port/dwc2/usb_hc_dwc2.d ./usb-framework/cherry_usb/port/dwc2/usb_hc_dwc2.o ./usb-framework/cherry_usb/port/dwc2/usb_hc_dwc2.su

.PHONY: clean-usb-2d-framework-2f-cherry_usb-2f-port-2f-dwc2

