################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../usb-framework/port/usb_dc_dwc2.c \
../usb-framework/port/usb_glue_st.c \
../usb-framework/port/usb_hc_dwc2.c 

OBJS += \
./usb-framework/port/usb_dc_dwc2.o \
./usb-framework/port/usb_glue_st.o \
./usb-framework/port/usb_hc_dwc2.o 

C_DEPS += \
./usb-framework/port/usb_dc_dwc2.d \
./usb-framework/port/usb_glue_st.d \
./usb-framework/port/usb_hc_dwc2.d 


# Each subdirectory must supply rules for building sources it contributes
usb-framework/port/%.o usb-framework/port/%.su usb-framework/port/%.cyclo: ../usb-framework/port/%.c usb-framework/port/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F767xx -DUSE_FULL_LL_DRIVER -c -I../Core/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/CMSIS/Include -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/freertos/include" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/freertos/portable" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/Bsp" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/lwip-2.2.2/src/include" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/lwip-2.2.2/system" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/fatfs_r0.15b/source" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/fatfs_r0.15b/drivers" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/letter-shell-3.x/src" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/letter-shell-3.x/drivers" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/usb-framework" -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-usb-2d-framework-2f-port

clean-usb-2d-framework-2f-port:
	-$(RM) ./usb-framework/port/usb_dc_dwc2.cyclo ./usb-framework/port/usb_dc_dwc2.d ./usb-framework/port/usb_dc_dwc2.o ./usb-framework/port/usb_dc_dwc2.su ./usb-framework/port/usb_glue_st.cyclo ./usb-framework/port/usb_glue_st.d ./usb-framework/port/usb_glue_st.o ./usb-framework/port/usb_glue_st.su ./usb-framework/port/usb_hc_dwc2.cyclo ./usb-framework/port/usb_hc_dwc2.d ./usb-framework/port/usb_hc_dwc2.o ./usb-framework/port/usb_hc_dwc2.su

.PHONY: clean-usb-2d-framework-2f-port

