################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../ECAL/Ecual_DHT11.c \
../ECAL/Ecual_Ultrasonic.c \
../ECAL/Ecual_lcd.c \
../ECAL/Ecual_max7219.c \
../ECAL/Ecual_rtc.c 

OBJS += \
./ECAL/Ecual_DHT11.o \
./ECAL/Ecual_Ultrasonic.o \
./ECAL/Ecual_lcd.o \
./ECAL/Ecual_max7219.o \
./ECAL/Ecual_rtc.o 

C_DEPS += \
./ECAL/Ecual_DHT11.d \
./ECAL/Ecual_Ultrasonic.d \
./ECAL/Ecual_lcd.d \
./ECAL/Ecual_max7219.d \
./ECAL/Ecual_rtc.d 


# Each subdirectory must supply rules for building sources it contributes
ECAL/%.o: ../ECAL/%.c ECAL/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -Wall -g3 -gdwarf-2 -Os -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega32 -DF_CPU=16000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


