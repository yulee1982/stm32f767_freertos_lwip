################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
S_SRCS += \
../mp3/helix/arm/asmmisc_gcc.s \
../mp3/helix/arm/asmpoly_thumb2_gcc.s 

OBJS += \
./mp3/helix/arm/asmmisc_gcc.o \
./mp3/helix/arm/asmpoly_thumb2_gcc.o 

S_DEPS += \
./mp3/helix/arm/asmmisc_gcc.d \
./mp3/helix/arm/asmpoly_thumb2_gcc.d 


# Each subdirectory must supply rules for building sources it contributes
mp3/helix/arm/%.o: ../mp3/helix/arm/%.s mp3/helix/arm/subdir.mk
	arm-none-eabi-gcc -mcpu=cortex-m7 -c -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/freertos/include" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/freertos/portable" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/Bsp" -x assembler-with-cpp -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@" "$<"

clean: clean-mp3-2f-helix-2f-arm

clean-mp3-2f-helix-2f-arm:
	-$(RM) ./mp3/helix/arm/asmmisc_gcc.d ./mp3/helix/arm/asmmisc_gcc.o ./mp3/helix/arm/asmpoly_thumb2_gcc.d ./mp3/helix/arm/asmpoly_thumb2_gcc.o

.PHONY: clean-mp3-2f-helix-2f-arm

