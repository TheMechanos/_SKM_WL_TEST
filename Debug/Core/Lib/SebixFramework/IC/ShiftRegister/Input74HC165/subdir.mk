################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Core/Lib/SebixFramework/IC/ShiftRegister/Input74HC165/Shift74HC165.cpp 

OBJS += \
./Core/Lib/SebixFramework/IC/ShiftRegister/Input74HC165/Shift74HC165.o 

CPP_DEPS += \
./Core/Lib/SebixFramework/IC/ShiftRegister/Input74HC165/Shift74HC165.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Lib/SebixFramework/IC/ShiftRegister/Input74HC165/%.o: ../Core/Lib/SebixFramework/IC/ShiftRegister/Input74HC165/%.cpp Core/Lib/SebixFramework/IC/ShiftRegister/Input74HC165/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m4 -std=gnu++14 -g3 -DDEBUG -DCORE_CM4 -DUSE_HAL_DRIVER -DSTM32WLE5xx -c -I../Core/Inc -I../Drivers/STM32WLxx_HAL_Driver/Inc -I../Drivers/STM32WLxx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32WLxx/Include -I../Drivers/CMSIS/Include -I../Core/Src/HAL -I../Core/Lib/SebixFramework -I../Core/Lib/STM32WL_SebixFramework -O0 -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Core-2f-Lib-2f-SebixFramework-2f-IC-2f-ShiftRegister-2f-Input74HC165

clean-Core-2f-Lib-2f-SebixFramework-2f-IC-2f-ShiftRegister-2f-Input74HC165:
	-$(RM) ./Core/Lib/SebixFramework/IC/ShiftRegister/Input74HC165/Shift74HC165.d ./Core/Lib/SebixFramework/IC/ShiftRegister/Input74HC165/Shift74HC165.o

.PHONY: clean-Core-2f-Lib-2f-SebixFramework-2f-IC-2f-ShiftRegister-2f-Input74HC165

