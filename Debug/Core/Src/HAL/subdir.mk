################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/HAL/stm32wlxx_hal_msp.c \
../Core/Src/HAL/stm32wlxx_it.c \
../Core/Src/HAL/syscalls.c \
../Core/Src/HAL/sysmem.c \
../Core/Src/HAL/system_stm32wlxx.c 

C_DEPS += \
./Core/Src/HAL/stm32wlxx_hal_msp.d \
./Core/Src/HAL/stm32wlxx_it.d \
./Core/Src/HAL/syscalls.d \
./Core/Src/HAL/sysmem.d \
./Core/Src/HAL/system_stm32wlxx.d 

OBJS += \
./Core/Src/HAL/stm32wlxx_hal_msp.o \
./Core/Src/HAL/stm32wlxx_it.o \
./Core/Src/HAL/syscalls.o \
./Core/Src/HAL/sysmem.o \
./Core/Src/HAL/system_stm32wlxx.o 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/HAL/%.o: ../Core/Src/HAL/%.c Core/Src/HAL/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DCORE_CM4 -DUSE_HAL_DRIVER -DSTM32WLE5xx -c -I../Core/Inc -I../Drivers/STM32WLxx_HAL_Driver/Inc -I../Drivers/STM32WLxx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32WLxx/Include -I../Drivers/CMSIS/Include -I../Core/Src/HAL -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-HAL

clean-Core-2f-Src-2f-HAL:
	-$(RM) ./Core/Src/HAL/stm32wlxx_hal_msp.d ./Core/Src/HAL/stm32wlxx_hal_msp.o ./Core/Src/HAL/stm32wlxx_it.d ./Core/Src/HAL/stm32wlxx_it.o ./Core/Src/HAL/syscalls.d ./Core/Src/HAL/syscalls.o ./Core/Src/HAL/sysmem.d ./Core/Src/HAL/sysmem.o ./Core/Src/HAL/system_stm32wlxx.d ./Core/Src/HAL/system_stm32wlxx.o

.PHONY: clean-Core-2f-Src-2f-HAL

