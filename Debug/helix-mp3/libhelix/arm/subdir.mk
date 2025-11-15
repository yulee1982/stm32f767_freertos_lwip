################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
S_SRCS += \
../helix-mp3/libhelix/arm/asmmisc_gcc.s \
../helix-mp3/libhelix/arm/asmpoly_thumb2_gcc.s 

OBJS += \
./helix-mp3/libhelix/arm/asmmisc_gcc.o \
./helix-mp3/libhelix/arm/asmpoly_thumb2_gcc.o 

S_DEPS += \
./helix-mp3/libhelix/arm/asmmisc_gcc.d \
./helix-mp3/libhelix/arm/asmpoly_thumb2_gcc.d 


# Each subdirectory must supply rules for building sources it contributes
helix-mp3/libhelix/arm/%.o: ../helix-mp3/libhelix/arm/%.s helix-mp3/libhelix/arm/subdir.mk
	arm-none-eabi-gcc -mcpu=cortex-m7 -g3 -DDEBUG -c -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/freertos/include" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/freertos/portable" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/Bsp" -I"D:/stm32/workspace_1.12.1/stm32f767_freertos/helix-mp3/libhelix/arm" -x assembler-with-cpp -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@" "$<"

clean: clean-helix-2d-mp3-2f-libhelix-2f-arm

clean-helix-2d-mp3-2f-libhelix-2f-arm:
	-$(RM) ./helix-mp3/libhelix/arm/asmmisc_gcc.d ./helix-mp3/libhelix/arm/asmmisc_gcc.o ./helix-mp3/libhelix/arm/asmpoly_thumb2_gcc.d ./helix-mp3/libhelix/arm/asmpoly_thumb2_gcc.o

.PHONY: clean-helix-2d-mp3-2f-libhelix-2f-arm

