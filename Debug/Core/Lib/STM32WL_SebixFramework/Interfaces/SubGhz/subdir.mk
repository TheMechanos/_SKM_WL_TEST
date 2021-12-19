################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Core/Lib/STM32WL_SebixFramework/Interfaces/SubGhz/SubGhz.cpp 

OBJS += \
./Core/Lib/STM32WL_SebixFramework/Interfaces/SubGhz/SubGhz.o 

CPP_DEPS += \
./Core/Lib/STM32WL_SebixFramework/Interfaces/SubGhz/SubGhz.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Lib/STM32WL_SebixFramework/Interfaces/SubGhz/%.o: ../Core/Lib/STM32WL_SebixFramework/Interfaces/SubGhz/%.cpp Core/Lib/STM32WL_SebixFramework/Interfaces/SubGhz/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m4 -std=gnu++14 -g3 -DDEBUG -DCORE_CM4 -DUSE_HAL_DRIVER -DSTM32WLE5xx -c -I../Core/Inc -I../Drivers/STM32WLxx_HAL_Driver/Inc -I../Drivers/STM32WLxx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32WLxx/Include -I../Drivers/CMSIS/Include -I../Core/Src/HAL -I../Core/Lib/SebixFramework -I../Core/Lib/STM32WL_SebixFramework -I../Core/Src -Og -ffunction-sections -fdata-sections -fno-exceptions -fno-use-cxa-atexit -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Core-2f-Lib-2f-STM32WL_SebixFramework-2f-Interfaces-2f-SubGhz

clean-Core-2f-Lib-2f-STM32WL_SebixFramework-2f-Interfaces-2f-SubGhz:
	-$(RM) ./Core/Lib/STM32WL_SebixFramework/Interfaces/SubGhz/SubGhz.d ./Core/Lib/STM32WL_SebixFramework/Interfaces/SubGhz/SubGhz.o

.PHONY: clean-Core-2f-Lib-2f-STM32WL_SebixFramework-2f-Interfaces-2f-SubGhz

