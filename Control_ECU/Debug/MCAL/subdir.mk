################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../MCAL/gpio.c \
../MCAL/pwm.c \
../MCAL/timer.c \
../MCAL/twi.c \
../MCAL/uart.c 

OBJS += \
./MCAL/gpio.o \
./MCAL/pwm.o \
./MCAL/timer.o \
./MCAL/twi.o \
./MCAL/uart.o 

C_DEPS += \
./MCAL/gpio.d \
./MCAL/pwm.d \
./MCAL/timer.d \
./MCAL/twi.d \
./MCAL/uart.d 


# Each subdirectory must supply rules for building sources it contributes
MCAL/%.o: ../MCAL/%.c MCAL/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -Wall -g2 -gstabs -O0 -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega32 -DF_CPU=8000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


