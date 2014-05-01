################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.local

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS_QUOTED += \
"../DDS/DDS.c" \

C_SRCS += \
../DDS/DDS.c \

OBJS += \
./DDS/DDS.o \

C_DEPS += \
./DDS/DDS.d \

OBJS_QUOTED += \
"./DDS/DDS.o" \

C_DEPS_QUOTED += \
"./DDS/DDS.d" \

OBJS_OS_FORMAT += \
./DDS/DDS.o \


# Each subdirectory must supply rules for building sources it contributes
DDS/DDS.o: ../DDS/DDS.c
	@echo 'Building file: $<'
	@echo 'Executing target #5 $<'
	@echo 'Invoking: ARM Ltd Windows GCC C Compiler'
	"$(ARMSourceryDirEnv)/arm-none-eabi-gcc" "$<" @"DDS/DDS.args" -MMD -MP -MF"$(@:%.o=%.d)" -o"DDS/DDS.o"
	@echo 'Finished building: $<'
	@echo ' '


