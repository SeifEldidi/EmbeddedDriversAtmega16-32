################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Gpiodriver.c \
../I2C.c \
../Internal_Interrupts.c \
../Interrupts.c \
../OLEDDisplay.c \
../RTCDS1307.c \
../main.c 

OBJS += \
./Gpiodriver.o \
./I2C.o \
./Internal_Interrupts.o \
./Interrupts.o \
./OLEDDisplay.o \
./RTCDS1307.o \
./main.o 

C_DEPS += \
./Gpiodriver.d \
./I2C.d \
./Internal_Interrupts.d \
./Interrupts.d \
./OLEDDisplay.d \
./RTCDS1307.d \
./main.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -Wall -g3 -gdwarf-2 -Os -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega32 -DF_CPU=8000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


