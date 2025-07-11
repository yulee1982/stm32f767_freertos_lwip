################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../lwip/netif/ppp/auth.c \
../lwip/netif/ppp/ccp.c \
../lwip/netif/ppp/chap-md5.c \
../lwip/netif/ppp/chap-new.c \
../lwip/netif/ppp/chap_ms.c \
../lwip/netif/ppp/demand.c \
../lwip/netif/ppp/eap.c \
../lwip/netif/ppp/ecp.c \
../lwip/netif/ppp/eui64.c \
../lwip/netif/ppp/fsm.c \
../lwip/netif/ppp/ipcp.c \
../lwip/netif/ppp/ipv6cp.c \
../lwip/netif/ppp/lcp.c \
../lwip/netif/ppp/magic.c \
../lwip/netif/ppp/mppe.c \
../lwip/netif/ppp/multilink.c \
../lwip/netif/ppp/ppp.c \
../lwip/netif/ppp/pppapi.c \
../lwip/netif/ppp/pppcrypt.c \
../lwip/netif/ppp/pppoe.c \
../lwip/netif/ppp/pppol2tp.c \
../lwip/netif/ppp/pppos.c \
../lwip/netif/ppp/upap.c \
../lwip/netif/ppp/utils.c \
../lwip/netif/ppp/vj.c 

OBJS += \
./lwip/netif/ppp/auth.o \
./lwip/netif/ppp/ccp.o \
./lwip/netif/ppp/chap-md5.o \
./lwip/netif/ppp/chap-new.o \
./lwip/netif/ppp/chap_ms.o \
./lwip/netif/ppp/demand.o \
./lwip/netif/ppp/eap.o \
./lwip/netif/ppp/ecp.o \
./lwip/netif/ppp/eui64.o \
./lwip/netif/ppp/fsm.o \
./lwip/netif/ppp/ipcp.o \
./lwip/netif/ppp/ipv6cp.o \
./lwip/netif/ppp/lcp.o \
./lwip/netif/ppp/magic.o \
./lwip/netif/ppp/mppe.o \
./lwip/netif/ppp/multilink.o \
./lwip/netif/ppp/ppp.o \
./lwip/netif/ppp/pppapi.o \
./lwip/netif/ppp/pppcrypt.o \
./lwip/netif/ppp/pppoe.o \
./lwip/netif/ppp/pppol2tp.o \
./lwip/netif/ppp/pppos.o \
./lwip/netif/ppp/upap.o \
./lwip/netif/ppp/utils.o \
./lwip/netif/ppp/vj.o 

C_DEPS += \
./lwip/netif/ppp/auth.d \
./lwip/netif/ppp/ccp.d \
./lwip/netif/ppp/chap-md5.d \
./lwip/netif/ppp/chap-new.d \
./lwip/netif/ppp/chap_ms.d \
./lwip/netif/ppp/demand.d \
./lwip/netif/ppp/eap.d \
./lwip/netif/ppp/ecp.d \
./lwip/netif/ppp/eui64.d \
./lwip/netif/ppp/fsm.d \
./lwip/netif/ppp/ipcp.d \
./lwip/netif/ppp/ipv6cp.d \
./lwip/netif/ppp/lcp.d \
./lwip/netif/ppp/magic.d \
./lwip/netif/ppp/mppe.d \
./lwip/netif/ppp/multilink.d \
./lwip/netif/ppp/ppp.d \
./lwip/netif/ppp/pppapi.d \
./lwip/netif/ppp/pppcrypt.d \
./lwip/netif/ppp/pppoe.d \
./lwip/netif/ppp/pppol2tp.d \
./lwip/netif/ppp/pppos.d \
./lwip/netif/ppp/upap.d \
./lwip/netif/ppp/utils.d \
./lwip/netif/ppp/vj.d 


# Each subdirectory must supply rules for building sources it contributes
lwip/netif/ppp/%.o lwip/netif/ppp/%.su lwip/netif/ppp/%.cyclo: ../lwip/netif/ppp/%.c lwip/netif/ppp/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -DUSE_HAL_DRIVER -DSTM32F767xx -DUSE_FULL_LL_DRIVER -c -I../Core/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/CMSIS/Include -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/freertos/include" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/lwip/include" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/freertos/portable" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/Bsp" -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-lwip-2f-netif-2f-ppp

clean-lwip-2f-netif-2f-ppp:
	-$(RM) ./lwip/netif/ppp/auth.cyclo ./lwip/netif/ppp/auth.d ./lwip/netif/ppp/auth.o ./lwip/netif/ppp/auth.su ./lwip/netif/ppp/ccp.cyclo ./lwip/netif/ppp/ccp.d ./lwip/netif/ppp/ccp.o ./lwip/netif/ppp/ccp.su ./lwip/netif/ppp/chap-md5.cyclo ./lwip/netif/ppp/chap-md5.d ./lwip/netif/ppp/chap-md5.o ./lwip/netif/ppp/chap-md5.su ./lwip/netif/ppp/chap-new.cyclo ./lwip/netif/ppp/chap-new.d ./lwip/netif/ppp/chap-new.o ./lwip/netif/ppp/chap-new.su ./lwip/netif/ppp/chap_ms.cyclo ./lwip/netif/ppp/chap_ms.d ./lwip/netif/ppp/chap_ms.o ./lwip/netif/ppp/chap_ms.su ./lwip/netif/ppp/demand.cyclo ./lwip/netif/ppp/demand.d ./lwip/netif/ppp/demand.o ./lwip/netif/ppp/demand.su ./lwip/netif/ppp/eap.cyclo ./lwip/netif/ppp/eap.d ./lwip/netif/ppp/eap.o ./lwip/netif/ppp/eap.su ./lwip/netif/ppp/ecp.cyclo ./lwip/netif/ppp/ecp.d ./lwip/netif/ppp/ecp.o ./lwip/netif/ppp/ecp.su ./lwip/netif/ppp/eui64.cyclo ./lwip/netif/ppp/eui64.d ./lwip/netif/ppp/eui64.o ./lwip/netif/ppp/eui64.su ./lwip/netif/ppp/fsm.cyclo ./lwip/netif/ppp/fsm.d ./lwip/netif/ppp/fsm.o ./lwip/netif/ppp/fsm.su ./lwip/netif/ppp/ipcp.cyclo ./lwip/netif/ppp/ipcp.d ./lwip/netif/ppp/ipcp.o ./lwip/netif/ppp/ipcp.su ./lwip/netif/ppp/ipv6cp.cyclo ./lwip/netif/ppp/ipv6cp.d ./lwip/netif/ppp/ipv6cp.o ./lwip/netif/ppp/ipv6cp.su ./lwip/netif/ppp/lcp.cyclo ./lwip/netif/ppp/lcp.d ./lwip/netif/ppp/lcp.o ./lwip/netif/ppp/lcp.su ./lwip/netif/ppp/magic.cyclo ./lwip/netif/ppp/magic.d ./lwip/netif/ppp/magic.o ./lwip/netif/ppp/magic.su ./lwip/netif/ppp/mppe.cyclo ./lwip/netif/ppp/mppe.d ./lwip/netif/ppp/mppe.o ./lwip/netif/ppp/mppe.su ./lwip/netif/ppp/multilink.cyclo ./lwip/netif/ppp/multilink.d ./lwip/netif/ppp/multilink.o ./lwip/netif/ppp/multilink.su ./lwip/netif/ppp/ppp.cyclo ./lwip/netif/ppp/ppp.d ./lwip/netif/ppp/ppp.o ./lwip/netif/ppp/ppp.su ./lwip/netif/ppp/pppapi.cyclo ./lwip/netif/ppp/pppapi.d ./lwip/netif/ppp/pppapi.o ./lwip/netif/ppp/pppapi.su ./lwip/netif/ppp/pppcrypt.cyclo ./lwip/netif/ppp/pppcrypt.d ./lwip/netif/ppp/pppcrypt.o ./lwip/netif/ppp/pppcrypt.su ./lwip/netif/ppp/pppoe.cyclo ./lwip/netif/ppp/pppoe.d ./lwip/netif/ppp/pppoe.o ./lwip/netif/ppp/pppoe.su ./lwip/netif/ppp/pppol2tp.cyclo ./lwip/netif/ppp/pppol2tp.d ./lwip/netif/ppp/pppol2tp.o ./lwip/netif/ppp/pppol2tp.su ./lwip/netif/ppp/pppos.cyclo ./lwip/netif/ppp/pppos.d ./lwip/netif/ppp/pppos.o ./lwip/netif/ppp/pppos.su ./lwip/netif/ppp/upap.cyclo ./lwip/netif/ppp/upap.d ./lwip/netif/ppp/upap.o ./lwip/netif/ppp/upap.su ./lwip/netif/ppp/utils.cyclo ./lwip/netif/ppp/utils.d ./lwip/netif/ppp/utils.o ./lwip/netif/ppp/utils.su ./lwip/netif/ppp/vj.cyclo ./lwip/netif/ppp/vj.d ./lwip/netif/ppp/vj.o ./lwip/netif/ppp/vj.su

.PHONY: clean-lwip-2f-netif-2f-ppp

