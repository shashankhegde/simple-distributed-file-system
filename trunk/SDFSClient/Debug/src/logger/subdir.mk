################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/logger/Dictionary.cpp \
../src/logger/DictionaryEntry.cpp \
../src/logger/DictionaryEvent.cpp \
../src/logger/DictionaryManager.cpp \
../src/logger/Logger.cpp \
../src/logger/TimeTable.cpp 

OBJS += \
./src/logger/Dictionary.o \
./src/logger/DictionaryEntry.o \
./src/logger/DictionaryEvent.o \
./src/logger/DictionaryManager.o \
./src/logger/Logger.o \
./src/logger/TimeTable.o 

CPP_DEPS += \
./src/logger/Dictionary.d \
./src/logger/DictionaryEntry.d \
./src/logger/DictionaryEvent.d \
./src/logger/DictionaryManager.d \
./src/logger/Logger.d \
./src/logger/TimeTable.d 


# Each subdirectory must supply rules for building sources it contributes
src/logger/%.o: ../src/logger/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


