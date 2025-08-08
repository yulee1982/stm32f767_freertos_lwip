################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../freertos/croutine.c \
../freertos/event_groups.c \
../freertos/list.c \
../freertos/queue.c \
../freertos/stream_buffer.c \
../freertos/tasks.c \
../freertos/timers.c 

OBJS += \
./freertos/croutine.o \
./freertos/event_groups.o \
./freertos/list.o \
./freertos/queue.o \
./freertos/stream_buffer.o \
./freertos/tasks.o \
./freertos/timers.o 

C_DEPS += \
./freertos/croutine.d \
./freertos/event_groups.d \
./freertos/list.d \
./freertos/queue.d \
./freertos/stream_buffer.d \
./freertos/tasks.d \
./freertos/timers.d 


# Each subdirectory must supply rules for building sources it contributes
freertos/%.o freertos/%.su freertos/%.cyclo: ../freertos/%.c freertos/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F767xx -DUSE_FULL_LL_DRIVER -c -I../Core/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/CMSIS/Include -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/freertos/portable" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/freertos/include" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/Bsp" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/lwip-2.2.2/src/include" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/lwip-2.2.2/system" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/fatfs_r0.15b/source" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/fatfs_r0.15b/drivers" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/letter-shell-3.x/src" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/letter-shell-3.x/drivers" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-freertos

clean-freertos:
	-$(RM) ./freertos/croutine.cyclo ./freertos/croutine.d ./freertos/croutine.o ./freertos/croutine.su ./freertos/event_groups.cyclo ./freertos/event_groups.d ./freertos/event_groups.o ./freertos/event_groups.su ./freertos/list.cyclo ./freertos/list.d ./freertos/list.o ./freertos/list.su ./freertos/queue.cyclo ./freertos/queue.d ./freertos/queue.o ./freertos/queue.su ./freertos/stream_buffer.cyclo ./freertos/stream_buffer.d ./freertos/stream_buffer.o ./freertos/stream_buffer.su ./freertos/tasks.cyclo ./freertos/tasks.d ./freertos/tasks.o ./freertos/tasks.su ./freertos/timers.cyclo ./freertos/timers.d ./freertos/timers.o ./freertos/timers.su

.PHONY: clean-freertos

