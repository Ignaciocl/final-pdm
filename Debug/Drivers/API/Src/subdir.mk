################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/API/Src/API_I2c.c \
../Drivers/API/Src/API_LCD.c \
../Drivers/API/Src/API_LCD_port.c \
../Drivers/API/Src/API_cmdparser.c \
../Drivers/API/Src/API_debounce.c \
../Drivers/API/Src/API_delay.c \
../Drivers/API/Src/API_hardware.c \
../Drivers/API/Src/API_scanner.c \
../Drivers/API/Src/API_uart.c \
../Drivers/API/Src/common.c 

OBJS += \
./Drivers/API/Src/API_I2c.o \
./Drivers/API/Src/API_LCD.o \
./Drivers/API/Src/API_LCD_port.o \
./Drivers/API/Src/API_cmdparser.o \
./Drivers/API/Src/API_debounce.o \
./Drivers/API/Src/API_delay.o \
./Drivers/API/Src/API_hardware.o \
./Drivers/API/Src/API_scanner.o \
./Drivers/API/Src/API_uart.o \
./Drivers/API/Src/common.o 

C_DEPS += \
./Drivers/API/Src/API_I2c.d \
./Drivers/API/Src/API_LCD.d \
./Drivers/API/Src/API_LCD_port.d \
./Drivers/API/Src/API_cmdparser.d \
./Drivers/API/Src/API_debounce.d \
./Drivers/API/Src/API_delay.d \
./Drivers/API/Src/API_hardware.d \
./Drivers/API/Src/API_scanner.d \
./Drivers/API/Src/API_uart.d \
./Drivers/API/Src/common.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/API/Src/%.o Drivers/API/Src/%.su Drivers/API/Src/%.cyclo: ../Drivers/API/Src/%.c Drivers/API/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F446xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I"/home/ignaciocl/postgrado/PdM_workspace/practica_6/Drivers/API/Inc" -I"/home/ignaciocl/postgrado/PdM_workspace/practica_6/Drivers/API/Src" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Drivers-2f-API-2f-Src

clean-Drivers-2f-API-2f-Src:
	-$(RM) ./Drivers/API/Src/API_I2c.cyclo ./Drivers/API/Src/API_I2c.d ./Drivers/API/Src/API_I2c.o ./Drivers/API/Src/API_I2c.su ./Drivers/API/Src/API_LCD.cyclo ./Drivers/API/Src/API_LCD.d ./Drivers/API/Src/API_LCD.o ./Drivers/API/Src/API_LCD.su ./Drivers/API/Src/API_LCD_port.cyclo ./Drivers/API/Src/API_LCD_port.d ./Drivers/API/Src/API_LCD_port.o ./Drivers/API/Src/API_LCD_port.su ./Drivers/API/Src/API_cmdparser.cyclo ./Drivers/API/Src/API_cmdparser.d ./Drivers/API/Src/API_cmdparser.o ./Drivers/API/Src/API_cmdparser.su ./Drivers/API/Src/API_debounce.cyclo ./Drivers/API/Src/API_debounce.d ./Drivers/API/Src/API_debounce.o ./Drivers/API/Src/API_debounce.su ./Drivers/API/Src/API_delay.cyclo ./Drivers/API/Src/API_delay.d ./Drivers/API/Src/API_delay.o ./Drivers/API/Src/API_delay.su ./Drivers/API/Src/API_hardware.cyclo ./Drivers/API/Src/API_hardware.d ./Drivers/API/Src/API_hardware.o ./Drivers/API/Src/API_hardware.su ./Drivers/API/Src/API_scanner.cyclo ./Drivers/API/Src/API_scanner.d ./Drivers/API/Src/API_scanner.o ./Drivers/API/Src/API_scanner.su ./Drivers/API/Src/API_uart.cyclo ./Drivers/API/Src/API_uart.d ./Drivers/API/Src/API_uart.o ./Drivers/API/Src/API_uart.su ./Drivers/API/Src/common.cyclo ./Drivers/API/Src/common.d ./Drivers/API/Src/common.o ./Drivers/API/Src/common.su

.PHONY: clean-Drivers-2f-API-2f-Src

