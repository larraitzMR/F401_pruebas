################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Lora/Mac/region/Region.c \
../Core/Lora/Mac/region/RegionCommon.c \
../Core/Lora/Mac/region/RegionEU868.c 

OBJS += \
./Core/Lora/Mac/region/Region.o \
./Core/Lora/Mac/region/RegionCommon.o \
./Core/Lora/Mac/region/RegionEU868.o 

C_DEPS += \
./Core/Lora/Mac/region/Region.d \
./Core/Lora/Mac/region/RegionCommon.d \
./Core/Lora/Mac/region/RegionEU868.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Lora/Mac/region/Region.o: ../Core/Lora/Mac/region/Region.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DUSE_BAND_868 -DUSE_MODEM_LORA -DSTM32F411xE -DDEBUG -DPCB_SPI_SA -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Core/Lora/Mac/region/Region.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Core/Lora/Mac/region/RegionCommon.o: ../Core/Lora/Mac/region/RegionCommon.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DUSE_BAND_868 -DUSE_MODEM_LORA -DSTM32F411xE -DDEBUG -DPCB_SPI_SA -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Core/Lora/Mac/region/RegionCommon.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Core/Lora/Mac/region/RegionEU868.o: ../Core/Lora/Mac/region/RegionEU868.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DUSE_BAND_868 -DUSE_MODEM_LORA -DSTM32F411xE -DDEBUG -DPCB_SPI_SA -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Core/Lora/Mac/region/RegionEU868.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

