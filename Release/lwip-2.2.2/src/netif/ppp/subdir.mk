################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../lwip-2.2.2/src/netif/ppp/auth.c \
../lwip-2.2.2/src/netif/ppp/ccp.c \
../lwip-2.2.2/src/netif/ppp/chap-md5.c \
../lwip-2.2.2/src/netif/ppp/chap-new.c \
../lwip-2.2.2/src/netif/ppp/chap_ms.c \
../lwip-2.2.2/src/netif/ppp/demand.c \
../lwip-2.2.2/src/netif/ppp/eap.c \
../lwip-2.2.2/src/netif/ppp/ecp.c \
../lwip-2.2.2/src/netif/ppp/eui64.c \
../lwip-2.2.2/src/netif/ppp/fsm.c \
../lwip-2.2.2/src/netif/ppp/ipcp.c \
../lwip-2.2.2/src/netif/ppp/ipv6cp.c \
../lwip-2.2.2/src/netif/ppp/lcp.c \
../lwip-2.2.2/src/netif/ppp/magic.c \
../lwip-2.2.2/src/netif/ppp/mppe.c \
../lwip-2.2.2/src/netif/ppp/multilink.c \
../lwip-2.2.2/src/netif/ppp/ppp.c \
../lwip-2.2.2/src/netif/ppp/pppapi.c \
../lwip-2.2.2/src/netif/ppp/pppcrypt.c \
../lwip-2.2.2/src/netif/ppp/pppoe.c \
../lwip-2.2.2/src/netif/ppp/pppol2tp.c \
../lwip-2.2.2/src/netif/ppp/pppos.c \
../lwip-2.2.2/src/netif/ppp/upap.c \
../lwip-2.2.2/src/netif/ppp/utils.c \
../lwip-2.2.2/src/netif/ppp/vj.c 

OBJS += \
./lwip-2.2.2/src/netif/ppp/auth.o \
./lwip-2.2.2/src/netif/ppp/ccp.o \
./lwip-2.2.2/src/netif/ppp/chap-md5.o \
./lwip-2.2.2/src/netif/ppp/chap-new.o \
./lwip-2.2.2/src/netif/ppp/chap_ms.o \
./lwip-2.2.2/src/netif/ppp/demand.o \
./lwip-2.2.2/src/netif/ppp/eap.o \
./lwip-2.2.2/src/netif/ppp/ecp.o \
./lwip-2.2.2/src/netif/ppp/eui64.o \
./lwip-2.2.2/src/netif/ppp/fsm.o \
./lwip-2.2.2/src/netif/ppp/ipcp.o \
./lwip-2.2.2/src/netif/ppp/ipv6cp.o \
./lwip-2.2.2/src/netif/ppp/lcp.o \
./lwip-2.2.2/src/netif/ppp/magic.o \
./lwip-2.2.2/src/netif/ppp/mppe.o \
./lwip-2.2.2/src/netif/ppp/multilink.o \
./lwip-2.2.2/src/netif/ppp/ppp.o \
./lwip-2.2.2/src/netif/ppp/pppapi.o \
./lwip-2.2.2/src/netif/ppp/pppcrypt.o \
./lwip-2.2.2/src/netif/ppp/pppoe.o \
./lwip-2.2.2/src/netif/ppp/pppol2tp.o \
./lwip-2.2.2/src/netif/ppp/pppos.o \
./lwip-2.2.2/src/netif/ppp/upap.o \
./lwip-2.2.2/src/netif/ppp/utils.o \
./lwip-2.2.2/src/netif/ppp/vj.o 

C_DEPS += \
./lwip-2.2.2/src/netif/ppp/auth.d \
./lwip-2.2.2/src/netif/ppp/ccp.d \
./lwip-2.2.2/src/netif/ppp/chap-md5.d \
./lwip-2.2.2/src/netif/ppp/chap-new.d \
./lwip-2.2.2/src/netif/ppp/chap_ms.d \
./lwip-2.2.2/src/netif/ppp/demand.d \
./lwip-2.2.2/src/netif/ppp/eap.d \
./lwip-2.2.2/src/netif/ppp/ecp.d \
./lwip-2.2.2/src/netif/ppp/eui64.d \
./lwip-2.2.2/src/netif/ppp/fsm.d \
./lwip-2.2.2/src/netif/ppp/ipcp.d \
./lwip-2.2.2/src/netif/ppp/ipv6cp.d \
./lwip-2.2.2/src/netif/ppp/lcp.d \
./lwip-2.2.2/src/netif/ppp/magic.d \
./lwip-2.2.2/src/netif/ppp/mppe.d \
./lwip-2.2.2/src/netif/ppp/multilink.d \
./lwip-2.2.2/src/netif/ppp/ppp.d \
./lwip-2.2.2/src/netif/ppp/pppapi.d \
./lwip-2.2.2/src/netif/ppp/pppcrypt.d \
./lwip-2.2.2/src/netif/ppp/pppoe.d \
./lwip-2.2.2/src/netif/ppp/pppol2tp.d \
./lwip-2.2.2/src/netif/ppp/pppos.d \
./lwip-2.2.2/src/netif/ppp/upap.d \
./lwip-2.2.2/src/netif/ppp/utils.d \
./lwip-2.2.2/src/netif/ppp/vj.d 


# Each subdirectory must supply rules for building sources it contributes
lwip-2.2.2/src/netif/ppp/%.o lwip-2.2.2/src/netif/ppp/%.su lwip-2.2.2/src/netif/ppp/%.cyclo: ../lwip-2.2.2/src/netif/ppp/%.c lwip-2.2.2/src/netif/ppp/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F767xx -DUSE_FULL_LL_DRIVER -c -I../Core/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/CMSIS/Include -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/freertos/include" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/freertos/portable" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/Bsp" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/lwip-2.2.2/src/include" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/lwip-2.2.2/system" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/fatfs_r0.15b/source" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/fatfs_r0.15b/drivers" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/letter-shell-3.x/src" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/letter-shell-3.x/drivers" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/tinyusb/src" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/tinyusb/demo" -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-lwip-2d-2-2e-2-2e-2-2f-src-2f-netif-2f-ppp

clean-lwip-2d-2-2e-2-2e-2-2f-src-2f-netif-2f-ppp:
	-$(RM) ./lwip-2.2.2/src/netif/ppp/auth.cyclo ./lwip-2.2.2/src/netif/ppp/auth.d ./lwip-2.2.2/src/netif/ppp/auth.o ./lwip-2.2.2/src/netif/ppp/auth.su ./lwip-2.2.2/src/netif/ppp/ccp.cyclo ./lwip-2.2.2/src/netif/ppp/ccp.d ./lwip-2.2.2/src/netif/ppp/ccp.o ./lwip-2.2.2/src/netif/ppp/ccp.su ./lwip-2.2.2/src/netif/ppp/chap-md5.cyclo ./lwip-2.2.2/src/netif/ppp/chap-md5.d ./lwip-2.2.2/src/netif/ppp/chap-md5.o ./lwip-2.2.2/src/netif/ppp/chap-md5.su ./lwip-2.2.2/src/netif/ppp/chap-new.cyclo ./lwip-2.2.2/src/netif/ppp/chap-new.d ./lwip-2.2.2/src/netif/ppp/chap-new.o ./lwip-2.2.2/src/netif/ppp/chap-new.su ./lwip-2.2.2/src/netif/ppp/chap_ms.cyclo ./lwip-2.2.2/src/netif/ppp/chap_ms.d ./lwip-2.2.2/src/netif/ppp/chap_ms.o ./lwip-2.2.2/src/netif/ppp/chap_ms.su ./lwip-2.2.2/src/netif/ppp/demand.cyclo ./lwip-2.2.2/src/netif/ppp/demand.d ./lwip-2.2.2/src/netif/ppp/demand.o ./lwip-2.2.2/src/netif/ppp/demand.su ./lwip-2.2.2/src/netif/ppp/eap.cyclo ./lwip-2.2.2/src/netif/ppp/eap.d ./lwip-2.2.2/src/netif/ppp/eap.o ./lwip-2.2.2/src/netif/ppp/eap.su ./lwip-2.2.2/src/netif/ppp/ecp.cyclo ./lwip-2.2.2/src/netif/ppp/ecp.d ./lwip-2.2.2/src/netif/ppp/ecp.o ./lwip-2.2.2/src/netif/ppp/ecp.su ./lwip-2.2.2/src/netif/ppp/eui64.cyclo ./lwip-2.2.2/src/netif/ppp/eui64.d ./lwip-2.2.2/src/netif/ppp/eui64.o ./lwip-2.2.2/src/netif/ppp/eui64.su ./lwip-2.2.2/src/netif/ppp/fsm.cyclo ./lwip-2.2.2/src/netif/ppp/fsm.d ./lwip-2.2.2/src/netif/ppp/fsm.o ./lwip-2.2.2/src/netif/ppp/fsm.su ./lwip-2.2.2/src/netif/ppp/ipcp.cyclo ./lwip-2.2.2/src/netif/ppp/ipcp.d ./lwip-2.2.2/src/netif/ppp/ipcp.o ./lwip-2.2.2/src/netif/ppp/ipcp.su ./lwip-2.2.2/src/netif/ppp/ipv6cp.cyclo ./lwip-2.2.2/src/netif/ppp/ipv6cp.d ./lwip-2.2.2/src/netif/ppp/ipv6cp.o ./lwip-2.2.2/src/netif/ppp/ipv6cp.su ./lwip-2.2.2/src/netif/ppp/lcp.cyclo ./lwip-2.2.2/src/netif/ppp/lcp.d ./lwip-2.2.2/src/netif/ppp/lcp.o ./lwip-2.2.2/src/netif/ppp/lcp.su ./lwip-2.2.2/src/netif/ppp/magic.cyclo ./lwip-2.2.2/src/netif/ppp/magic.d ./lwip-2.2.2/src/netif/ppp/magic.o ./lwip-2.2.2/src/netif/ppp/magic.su ./lwip-2.2.2/src/netif/ppp/mppe.cyclo ./lwip-2.2.2/src/netif/ppp/mppe.d ./lwip-2.2.2/src/netif/ppp/mppe.o ./lwip-2.2.2/src/netif/ppp/mppe.su ./lwip-2.2.2/src/netif/ppp/multilink.cyclo ./lwip-2.2.2/src/netif/ppp/multilink.d ./lwip-2.2.2/src/netif/ppp/multilink.o ./lwip-2.2.2/src/netif/ppp/multilink.su ./lwip-2.2.2/src/netif/ppp/ppp.cyclo ./lwip-2.2.2/src/netif/ppp/ppp.d ./lwip-2.2.2/src/netif/ppp/ppp.o ./lwip-2.2.2/src/netif/ppp/ppp.su ./lwip-2.2.2/src/netif/ppp/pppapi.cyclo ./lwip-2.2.2/src/netif/ppp/pppapi.d ./lwip-2.2.2/src/netif/ppp/pppapi.o ./lwip-2.2.2/src/netif/ppp/pppapi.su ./lwip-2.2.2/src/netif/ppp/pppcrypt.cyclo ./lwip-2.2.2/src/netif/ppp/pppcrypt.d ./lwip-2.2.2/src/netif/ppp/pppcrypt.o ./lwip-2.2.2/src/netif/ppp/pppcrypt.su ./lwip-2.2.2/src/netif/ppp/pppoe.cyclo ./lwip-2.2.2/src/netif/ppp/pppoe.d ./lwip-2.2.2/src/netif/ppp/pppoe.o ./lwip-2.2.2/src/netif/ppp/pppoe.su ./lwip-2.2.2/src/netif/ppp/pppol2tp.cyclo ./lwip-2.2.2/src/netif/ppp/pppol2tp.d ./lwip-2.2.2/src/netif/ppp/pppol2tp.o ./lwip-2.2.2/src/netif/ppp/pppol2tp.su ./lwip-2.2.2/src/netif/ppp/pppos.cyclo ./lwip-2.2.2/src/netif/ppp/pppos.d ./lwip-2.2.2/src/netif/ppp/pppos.o ./lwip-2.2.2/src/netif/ppp/pppos.su ./lwip-2.2.2/src/netif/ppp/upap.cyclo ./lwip-2.2.2/src/netif/ppp/upap.d ./lwip-2.2.2/src/netif/ppp/upap.o ./lwip-2.2.2/src/netif/ppp/upap.su ./lwip-2.2.2/src/netif/ppp/utils.cyclo ./lwip-2.2.2/src/netif/ppp/utils.d ./lwip-2.2.2/src/netif/ppp/utils.o ./lwip-2.2.2/src/netif/ppp/utils.su ./lwip-2.2.2/src/netif/ppp/vj.cyclo ./lwip-2.2.2/src/netif/ppp/vj.d ./lwip-2.2.2/src/netif/ppp/vj.o ./lwip-2.2.2/src/netif/ppp/vj.su

.PHONY: clean-lwip-2d-2-2e-2-2e-2-2f-src-2f-netif-2f-ppp

