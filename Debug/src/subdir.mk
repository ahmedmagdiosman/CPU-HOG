################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/HOG.cpp \
../src/Image3.cpp \
../src/Vec2.cpp \
../src/main.cpp 

OBJS += \
./src/HOG.o \
./src/Image3.o \
./src/Vec2.o \
./src/main.o 

CPP_DEPS += \
./src/HOG.d \
./src/Image3.d \
./src/Vec2.d \
./src/main.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


