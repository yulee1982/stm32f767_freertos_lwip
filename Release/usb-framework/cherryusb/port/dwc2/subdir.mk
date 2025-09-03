################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../usb-framework/cherryusb/port/dwc2/usb_dc_dwc2.c \
../usb-framework/cherryusb/port/dwc2/usb_glue_at.c \
../usb-framework/cherryusb/port/dwc2/usb_glue_esp.c \
../usb-framework/cherryusb/port/dwc2/usb_glue_gd.c \
../usb-framework/cherryusb/port/dwc2/usb_glue_hc.c \
../usb-framework/cherryusb/port/dwc2/usb_glue_kendryte.c \
../usb-framework/cherryusb/port/dwc2/usb_glue_nation.c \
../usb-framework/cherryusb/port/dwc2/usb_glue_st.c \
../usb-framework/cherryusb/port/dwc2/usb_hc_dwc2.c 

OBJS += \
./usb-framework/cherryusb/port/dwc2/usb_dc_dwc2.o \
./usb-framework/cherryusb/port/dwc2/usb_glue_at.o \
./usb-framework/cherryusb/port/dwc2/usb_glue_esp.o \
./usb-framework/cherryusb/port/dwc2/usb_glue_gd.o \
./usb-framework/cherryusb/port/dwc2/usb_glue_hc.o \
./usb-framework/cherryusb/port/dwc2/usb_glue_kendryte.o \
./usb-framework/cherryusb/port/dwc2/usb_glue_nation.o \
./usb-framework/cherryusb/port/dwc2/usb_glue_st.o \
./usb-framework/cherryusb/port/dwc2/usb_hc_dwc2.o 

C_DEPS += \
./usb-framework/cherryusb/port/dwc2/usb_dc_dwc2.d \
./usb-framework/cherryusb/port/dwc2/usb_glue_at.d \
./usb-framework/cherryusb/port/dwc2/usb_glue_esp.d \
./usb-framework/cherryusb/port/dwc2/usb_glue_gd.d \
./usb-framework/cherryusb/port/dwc2/usb_glue_hc.d \
./usb-framework/cherryusb/port/dwc2/usb_glue_kendryte.d \
./usb-framework/cherryusb/port/dwc2/usb_glue_nation.d \
./usb-framework/cherryusb/port/dwc2/usb_glue_st.d \
./usb-framework/cherryusb/port/dwc2/usb_hc_dwc2.d 


# Each subdirectory must supply rules for building sources it contributes
usb-framework/cherryusb/port/dwc2/%.o usb-framework/cherryusb/port/dwc2/%.su usb-framework/cherryusb/port/dwc2/%.cyclo: ../usb-framework/cherryusb/port/dwc2/%.c usb-framework/cherryusb/port/dwc2/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -DUSE_HAL_DRIVER -DSTM32F767xx -DUSE_FULL_LL_DRIVER -c -I../Core/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/CMSIS/Include -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/freertos/include" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/freertos/portable" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/Bsp" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/lwip-2.2.2/src/include" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/lwip-2.2.2/system" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/fatfs_r0.15b/source" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/fatfs_r0.15b/drivers" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/letter-shell-3.x/src" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/letter-shell-3.x/drivers" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/usb-framework" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/usb-framework/libusb_stm32/inc" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/usb-framework/simple_usb" -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-usb-2d-framework-2f-cherryusb-2f-port-2f-dwc2

clean-usb-2d-framework-2f-cherryusb-2f-port-2f-dwc2:
	-$(RM) ./usb-framework/cherryusb/port/dwc2/usb_dc_dwc2.cyclo ./usb-framework/cherryusb/port/dwc2/usb_dc_dwc2.d ./usb-framework/cherryusb/port/dwc2/usb_dc_dwc2.o ./usb-framework/cherryusb/port/dwc2/usb_dc_dwc2.su ./usb-framework/cherryusb/port/dwc2/usb_glue_at.cyclo ./usb-framework/cherryusb/port/dwc2/usb_glue_at.d ./usb-framework/cherryusb/port/dwc2/usb_glue_at.o ./usb-framework/cherryusb/port/dwc2/usb_glue_at.su ./usb-framework/cherryusb/port/dwc2/usb_glue_esp.cyclo ./usb-framework/cherryusb/port/dwc2/usb_glue_esp.d ./usb-framework/cherryusb/port/dwc2/usb_glue_esp.o ./usb-framework/cherryusb/port/dwc2/usb_glue_esp.su ./usb-framework/cherryusb/port/dwc2/usb_glue_gd.cyclo ./usb-framework/cherryusb/port/dwc2/usb_glue_gd.d ./usb-framework/cherryusb/port/dwc2/usb_glue_gd.o ./usb-framework/cherryusb/port/dwc2/usb_glue_gd.su ./usb-framework/cherryusb/port/dwc2/usb_glue_hc.cyclo ./usb-framework/cherryusb/port/dwc2/usb_glue_hc.d ./usb-framework/cherryusb/port/dwc2/usb_glue_hc.o ./usb-framework/cherryusb/port/dwc2/usb_glue_hc.su ./usb-framework/cherryusb/port/dwc2/usb_glue_kendryte.cyclo ./usb-framework/cherryusb/port/dwc2/usb_glue_kendryte.d ./usb-framework/cherryusb/port/dwc2/usb_glue_kendryte.o ./usb-framework/cherryusb/port/dwc2/usb_glue_kendryte.su ./usb-framework/cherryusb/port/dwc2/usb_glue_nation.cyclo ./usb-framework/cherryusb/port/dwc2/usb_glue_nation.d ./usb-framework/cherryusb/port/dwc2/usb_glue_nation.o ./usb-framework/cherryusb/port/dwc2/usb_glue_nation.su ./usb-framework/cherryusb/port/dwc2/usb_glue_st.cyclo ./usb-framework/cherryusb/port/dwc2/usb_glue_st.d ./usb-framework/cherryusb/port/dwc2/usb_glue_st.o ./usb-framework/cherryusb/port/dwc2/usb_glue_st.su ./usb-framework/cherryusb/port/dwc2/usb_hc_dwc2.cyclo ./usb-framework/cherryusb/port/dwc2/usb_hc_dwc2.d ./usb-framework/cherryusb/port/dwc2/usb_hc_dwc2.o ./usb-framework/cherryusb/port/dwc2/usb_hc_dwc2.su

.PHONY: clean-usb-2d-framework-2f-cherryusb-2f-port-2f-dwc2

