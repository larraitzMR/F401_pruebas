################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Lora/Phy/sx1272/sx1272.c 

OBJS += \
./Lora/Phy/sx1272/sx1272.o 

C_DEPS += \
./Lora/Phy/sx1272/sx1272.d 


# Each subdirectory must supply rules for building sources it contributes
Lora/Phy/sx1272/sx1272.o: ../Lora/Phy/sx1272/sx1272.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DREGION_EU868 -DUSE_BAND_868 -DUSE_MODEM_LORA -DSTM32F411xE -c -I../Inc -I../Drivers/CMSIS/Include -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Lora/Core -I../Lora/Crypto -I../Lora/Mac -I../Lora/Phy -I../Lora/Utilities -I../Lora/Mac/region -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Lora/Phy/sx1272/sx1272.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

