################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Core/Lib/SebixFramework/Graphics/SebixUI_MONO_128x64/ScreenTypes/Menu/MenuElements.cpp \
../Core/Lib/SebixFramework/Graphics/SebixUI_MONO_128x64/ScreenTypes/Menu/MenuGeneric.cpp 

OBJS += \
./Core/Lib/SebixFramework/Graphics/SebixUI_MONO_128x64/ScreenTypes/Menu/MenuElements.o \
./Core/Lib/SebixFramework/Graphics/SebixUI_MONO_128x64/ScreenTypes/Menu/MenuGeneric.o 

CPP_DEPS += \
./Core/Lib/SebixFramework/Graphics/SebixUI_MONO_128x64/ScreenTypes/Menu/MenuElements.d \
./Core/Lib/SebixFramework/Graphics/SebixUI_MONO_128x64/ScreenTypes/Menu/MenuGeneric.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Lib/SebixFramework/Graphics/SebixUI_MONO_128x64/ScreenTypes/Menu/%.o: ../Core/Lib/SebixFramework/Graphics/SebixUI_MONO_128x64/ScreenTypes/Menu/%.cpp Core/Lib/SebixFramework/Graphics/SebixUI_MONO_128x64/ScreenTypes/Menu/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m4 -std=gnu++14 -g3 -DDEBUG -DCORE_CM4 -DUSE_HAL_DRIVER -DSTM32WLE5xx -c -I../Core/Inc -I../Drivers/STM32WLxx_HAL_Driver/Inc -I../Drivers/STM32WLxx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32WLxx/Include -I../Drivers/CMSIS/Include -I../Core/Src/HAL -I../Core/Lib/SebixFramework -I../Core/Lib/STM32WL_SebixFramework -I../Core/Src -Og -ffunction-sections -fdata-sections -fno-exceptions -fno-use-cxa-atexit -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Core-2f-Lib-2f-SebixFramework-2f-Graphics-2f-SebixUI_MONO_128x64-2f-ScreenTypes-2f-Menu

clean-Core-2f-Lib-2f-SebixFramework-2f-Graphics-2f-SebixUI_MONO_128x64-2f-ScreenTypes-2f-Menu:
	-$(RM) ./Core/Lib/SebixFramework/Graphics/SebixUI_MONO_128x64/ScreenTypes/Menu/MenuElements.d ./Core/Lib/SebixFramework/Graphics/SebixUI_MONO_128x64/ScreenTypes/Menu/MenuElements.o ./Core/Lib/SebixFramework/Graphics/SebixUI_MONO_128x64/ScreenTypes/Menu/MenuGeneric.d ./Core/Lib/SebixFramework/Graphics/SebixUI_MONO_128x64/ScreenTypes/Menu/MenuGeneric.o

.PHONY: clean-Core-2f-Lib-2f-SebixFramework-2f-Graphics-2f-SebixUI_MONO_128x64-2f-ScreenTypes-2f-Menu

