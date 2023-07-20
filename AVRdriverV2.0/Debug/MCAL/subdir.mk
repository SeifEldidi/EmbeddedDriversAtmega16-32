################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../MCAL/BitBangeUart.c \
../MCAL/Mcal_External_Interrupts.c \
../MCAL/Mcal_I2c.c \
../MCAL/Mcal_gpio.c \
../MCAL/mcal_adc.c \
../MCAL/mcal_spi.c \
../MCAL/mcal_timer0.c \
../MCAL/mcal_uart.c 

OBJS += \
./MCAL/BitBangeUart.o \
./MCAL/Mcal_External_Interrupts.o \
./MCAL/Mcal_I2c.o \
./MCAL/Mcal_gpio.o \
./MCAL/mcal_adc.o \
./MCAL/mcal_spi.o \
./MCAL/mcal_timer0.o \
./MCAL/mcal_uart.o 

C_DEPS += \
./MCAL/BitBangeUart.d \
./MCAL/Mcal_External_Interrupts.d \
./MCAL/Mcal_I2c.d \
./MCAL/Mcal_gpio.d \
./MCAL/mcal_adc.d \
./MCAL/mcal_spi.d \
./MCAL/mcal_timer0.d \
./MCAL/mcal_uart.d 


# Each subdirectory must supply rules for building sources it contributes
MCAL/%.o: ../MCAL/%.c MCAL/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -Wall -g3 -gdwarf-2 -Os -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega32 -DF_CPU=16000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


