################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Lora/Core/lora.c 

OBJS += \
./Core/Lora/Core/lora.o 

C_DEPS += \
./Core/Lora/Core/lora.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Lora/Core/lora.o: ../Core/Lora/Core/lora.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DUSE_BAND_868 -DUSE_MODEM_LORA -DSTM32F411xE -DDEBUG -DPCB_SPI_SA -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Core/Lora/Core/lora.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

