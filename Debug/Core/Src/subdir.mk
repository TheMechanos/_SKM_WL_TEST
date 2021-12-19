################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Core/Src/App.cpp \
../Core/Src/SKWL.cpp \
../Core/Src/System.cpp \
../Core/Src/main.cpp 

OBJS += \
./Core/Src/App.o \
./Core/Src/SKWL.o \
./Core/Src/System.o \
./Core/Src/main.o 

CPP_DEPS += \
./Core/Src/App.d \
./Core/Src/SKWL.d \
./Core/Src/System.d \
./Core/Src/main.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/%.o: ../Core/Src/%.cpp Core/Src/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m4 -std=gnu++14 -g3 -DDEBUG -DCORE_CM4 -DUSE_HAL_DRIVER -DSTM32WLE5xx -c -I../Core/Inc -I../Drivers/STM32WLxx_HAL_Driver/Inc -I../Drivers/STM32WLxx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32WLxx/Include -I../Drivers/CMSIS/Include -I../Core/Src/HAL -I../Core/Lib/SebixFramework -I../Core/Lib/STM32WL_SebixFramework -I../Core/Src -Og -ffunction-sections -fdata-sections -fno-exceptions -fno-use-cxa-atexit -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Core-2f-Src

clean-Core-2f-Src:
	-$(RM) ./Core/Src/App.d ./Core/Src/App.o ./Core/Src/SKWL.d ./Core/Src/SKWL.o ./Core/Src/System.d ./Core/Src/System.o ./Core/Src/main.d ./Core/Src/main.o

.PHONY: clean-Core-2f-Src

