################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Lora/Mac/LoRaMac.c \
../Lora/Mac/LoRaMacCrypto.c 

OBJS += \
./Lora/Mac/LoRaMac.o \
./Lora/Mac/LoRaMacCrypto.o 

C_DEPS += \
./Lora/Mac/LoRaMac.d \
./Lora/Mac/LoRaMacCrypto.d 


# Each subdirectory must supply rules for building sources it contributes
Lora/Mac/LoRaMac.o: ../Lora/Mac/LoRaMac.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DREGION_EU868 -DUSE_BAND_868 -DUSE_MODEM_LORA -DSTM32F411xE -c -I../Inc -I../Drivers/CMSIS/Include -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Lora/Core -I../Lora/Crypto -I../Lora/Mac -I../Lora/Phy -I../Lora/Utilities -I../Lora/Mac/region -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Lora/Mac/LoRaMac.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Lora/Mac/LoRaMacCrypto.o: ../Lora/Mac/LoRaMacCrypto.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DREGION_EU868 -DUSE_BAND_868 -DUSE_MODEM_LORA -DSTM32F411xE -c -I../Inc -I../Drivers/CMSIS/Include -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Lora/Core -I../Lora/Crypto -I../Lora/Mac -I../Lora/Phy -I../Lora/Utilities -I../Lora/Mac/region -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Lora/Mac/LoRaMacCrypto.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

