################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include Startup/subdir.mk
-include Src/subdir.mk
-include Lora/Utilities/subdir.mk
-include Lora/Phy/sx1272/subdir.mk
-include Lora/Mac/region/subdir.mk
-include Lora/Mac/subdir.mk
-include Lora/Crypto/subdir.mk
-include Lora/Core/subdir.mk
-include Drivers/STM32F4xx_HAL_Driver/Src/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
EXECUTABLES += \
F411.elf \

SIZE_OUTPUT += \
default.size.stdout \

OBJDUMP_LIST += \
F411.list \


# All Target
all: F411.elf secondary-outputs

# Tool invocations
F411.elf: $(OBJS) $(USER_OBJS) C:\Users\Propietario\STM32CubeIDE\workspace_1.0.2\F411\STM32F401RETX_FLASH.ld
	arm-none-eabi-gcc -o "F411.elf" @"objects.list" $(USER_OBJS) $(LIBS) -mcpu=cortex-m4 -T"C:\Users\Propietario\STM32CubeIDE\workspace_1.0.2\F411\STM32F401RETX_FLASH.ld" --specs=nosys.specs -Wl,-Map="F411.map" -Wl,--gc-sections -static --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -Wl,--start-group -lc -lm -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

default.size.stdout: $(EXECUTABLES)
	arm-none-eabi-size  $(EXECUTABLES)
	@echo 'Finished building: $@'
	@echo ' '

F411.list: $(EXECUTABLES)
	arm-none-eabi-objdump -h -S $(EXECUTABLES) > "F411.list"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) *
	-@echo ' '

secondary-outputs: $(SIZE_OUTPUT) $(OBJDUMP_LIST)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
