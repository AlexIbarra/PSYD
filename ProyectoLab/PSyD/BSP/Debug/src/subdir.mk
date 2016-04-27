################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/keypad.c \
../src/leds.c \
../src/pbs.c \
../src/rtc.c \
../src/segs.c \
../src/system.c \
../src/timers.c \
../src/uart.c 

OBJS += \
./src/keypad.o \
./src/leds.o \
./src/pbs.o \
./src/rtc.o \
./src/segs.o \
./src/system.o \
./src/timers.o \
./src/uart.o 

C_DEPS += \
./src/keypad.d \
./src/leds.d \
./src/pbs.d \
./src/rtc.d \
./src/segs.d \
./src/system.d \
./src/timers.d \
./src/uart.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Sourcery Windows GCC C Compiler'
	arm-none-eabi-gcc -I../../BSP/include -O0 -Wall -Wa,-adhlns="$@.lst" -c -fmessage-length=0 -mapcs-frame -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -mcpu=arm7tdmi -g3 -gdwarf-2 -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


