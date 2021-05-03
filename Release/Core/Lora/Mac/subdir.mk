################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Lora/Mac/LoRaMac.c \
../Core/Lora/Mac/LoRaMacCrypto.c 

OBJS += \
./Core/Lora/Mac/LoRaMac.o \
./Core/Lora/Mac/LoRaMacCrypto.o 

C_DEPS += \
./Core/Lora/Mac/LoRaMac.d \
./Core/Lora/Mac/LoRaMacCrypto.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Lora/Mac/LoRaMac.o: ../Core/Lora/Mac/LoRaMac.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -DUSE_HAL_DRIVER -DSTM32F411xE -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Core/Lora/Mac/LoRaMac.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Core/Lora/Mac/LoRaMacCrypto.o: ../Core/Lora/Mac/LoRaMacCrypto.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -DUSE_HAL_DRIVER -DSTM32F411xE -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Core/Lora/Mac/LoRaMacCrypto.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

