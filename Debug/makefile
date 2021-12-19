################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include Drivers/STM32WLxx_HAL_Driver/Src/subdir.mk
-include Core/Startup/subdir.mk
-include Core/Src/HAL/subdir.mk
-include Core/Src/subdir.mk
-include Core/Lib/SebixFramework/Radio/Modules/SX126x/subdir.mk
-include Core/Lib/SebixFramework/Parameters/subdir.mk
-include Core/Lib/SebixFramework/Music/subdir.mk
-include Core/Lib/SebixFramework/IC/WS2812B/subdir.mk
-include Core/Lib/SebixFramework/IC/LithiumBatteryChargers/LithiumBattery/subdir.mk
-include Core/Lib/SebixFramework/IC/LithiumBatteryChargers/subdir.mk
-include Core/Lib/SebixFramework/IC/EEPROM/24C512/subdir.mk
-include Core/Lib/SebixFramework/IC/DAC/MCP4728/subdir.mk
-include Core/Lib/SebixFramework/Graphics/SebixUI_MONO_128x64/ScreenTypes/TimeDataSet/subdir.mk
-include Core/Lib/SebixFramework/Graphics/SebixUI_MONO_128x64/ScreenTypes/Parameter/subdir.mk
-include Core/Lib/SebixFramework/Graphics/SebixUI_MONO_128x64/ScreenTypes/Message/subdir.mk
-include Core/Lib/SebixFramework/Graphics/SebixUI_MONO_128x64/ScreenTypes/Menu/subdir.mk
-include Core/Lib/SebixFramework/Graphics/SebixUI_MONO_128x64/ScreenTypes/Dialog/subdir.mk
-include Core/Lib/SebixFramework/Graphics/SebixUI_MONO_128x64/ScreenTypes/CodeAccess/subdir.mk
-include Core/Lib/SebixFramework/Graphics/SebixUI_MONO_128x64/ScreenTypes/ActionEntry/subdir.mk
-include Core/Lib/SebixFramework/Graphics/SebixUI_MONO_128x64/subdir.mk
-include Core/Lib/SebixFramework/Graphics/subdir.mk
-include Core/Lib/SebixFramework/Encoder/subdir.mk
-include Core/Lib/SebixFramework/Displays/MONO/WDO0066/subdir.mk
-include Core/Lib/SebixFramework/BasicTypes/RTC/subdir.mk
-include Core/Lib/SebixFramework/BasicTypes/Key/subdir.mk
-include Core/Lib/SebixFramework/BasicTypes/ImpulseInput/subdir.mk
-include Core/Lib/SebixFramework/BasicTypes/IO/subdir.mk
-include Core/Lib/SebixFramework/BasicTypes/AnalogInput/subdir.mk
-include Core/Lib/STM32WL_SebixFramework/Peripheral/Timers/TimeBase/subdir.mk
-include Core/Lib/STM32WL_SebixFramework/Peripheral/Timers/PWM/subdir.mk
-include Core/Lib/STM32WL_SebixFramework/Peripheral/Timers/Encoder/subdir.mk
-include Core/Lib/STM32WL_SebixFramework/Peripheral/RTC/subdir.mk
-include Core/Lib/STM32WL_SebixFramework/Peripheral/GPIO/subdir.mk
-include Core/Lib/STM32WL_SebixFramework/Peripheral/DMA/subdir.mk
-include Core/Lib/STM32WL_SebixFramework/Peripheral/ADC/subdir.mk
-include Core/Lib/STM32WL_SebixFramework/Interfaces/SubGhz/subdir.mk
-include Core/Lib/STM32WL_SebixFramework/Interfaces/SPI/subdir.mk
-include Core/Lib/STM32WL_SebixFramework/Interfaces/I2C/subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
endif

-include ../makefile.defs

OPTIONAL_TOOL_DEPS := \
$(wildcard ../makefile.defs) \
$(wildcard ../makefile.init) \
$(wildcard ../makefile.targets) \


BUILD_ARTIFACT_NAME := _SKM_WL_TEST
BUILD_ARTIFACT_EXTENSION := elf
BUILD_ARTIFACT_PREFIX :=
BUILD_ARTIFACT := $(BUILD_ARTIFACT_PREFIX)$(BUILD_ARTIFACT_NAME)$(if $(BUILD_ARTIFACT_EXTENSION),.$(BUILD_ARTIFACT_EXTENSION),)

# Add inputs and outputs from these tool invocations to the build variables 
EXECUTABLES += \
_SKM_WL_TEST.elf \

SIZE_OUTPUT += \
default.size.stdout \

OBJDUMP_LIST += \
_SKM_WL_TEST.list \

OBJCOPY_BIN += \
_SKM_WL_TEST.bin \


# All Target
all: main-build

# Main-build Target
main-build: _SKM_WL_TEST.elf secondary-outputs

# Tool invocations
_SKM_WL_TEST.elf: $(OBJS) $(USER_OBJS) C:\OneDrive\PROJEKTY\PROGRAMY_STM_CUBEIDE\_SKM_WL_TEST\STM32WLE5JCIX_FLASH.ld makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-g++ -o "_SKM_WL_TEST.elf" @"objects.list" $(USER_OBJS) $(LIBS) -mcpu=cortex-m4 -T"C:\OneDrive\PROJEKTY\PROGRAMY_STM_CUBEIDE\_SKM_WL_TEST\STM32WLE5JCIX_FLASH.ld" --specs=nosys.specs -Wl,-Map="_SKM_WL_TEST.map" -Wl,--gc-sections -static --specs=nano.specs -mfloat-abi=soft -mthumb -Wl,--start-group -lc -lm -lstdc++ -lsupc++ -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

default.size.stdout: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-size  $(EXECUTABLES)
	@echo 'Finished building: $@'
	@echo ' '

_SKM_WL_TEST.list: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-objdump -h -S $(EXECUTABLES) > "_SKM_WL_TEST.list"
	@echo 'Finished building: $@'
	@echo ' '

_SKM_WL_TEST.bin: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-objcopy  -O binary $(EXECUTABLES) "_SKM_WL_TEST.bin"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) _SKM_WL_TEST.bin _SKM_WL_TEST.elf _SKM_WL_TEST.list default.size.stdout
	-@echo ' '

secondary-outputs: $(SIZE_OUTPUT) $(OBJDUMP_LIST) $(OBJCOPY_BIN)

fail-specified-linker-script-missing:
	@echo 'Error: Cannot find the specified linker script. Check the linker settings in the build configuration.'
	@exit 2

warn-no-linker-script-specified:
	@echo 'Warning: No linker script specified. Check the linker settings in the build configuration.'

.PHONY: all clean dependents main-build fail-specified-linker-script-missing warn-no-linker-script-specified

-include ../makefile.targets
