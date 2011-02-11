################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/DHCPClient.cpp \
../src/DNSClient.cpp \
../src/SDFSClient.cpp \
../src/SDFSConnector.cpp \
../src/SDFSControllerUI.cpp \
../src/SDFSListener.cpp \
../src/SDFSMessage.cpp \
../src/SDFSMessageQueue.cpp \
../src/SDFSSocket.cpp \
../src/Utils.cpp \
../src/main.cpp 

OBJS += \
./src/DHCPClient.o \
./src/DNSClient.o \
./src/SDFSClient.o \
./src/SDFSConnector.o \
./src/SDFSControllerUI.o \
./src/SDFSListener.o \
./src/SDFSMessage.o \
./src/SDFSMessageQueue.o \
./src/SDFSSocket.o \
./src/Utils.o \
./src/main.o 

CPP_DEPS += \
./src/DHCPClient.d \
./src/DNSClient.d \
./src/SDFSClient.d \
./src/SDFSConnector.d \
./src/SDFSControllerUI.d \
./src/SDFSListener.d \
./src/SDFSMessage.d \
./src/SDFSMessageQueue.d \
./src/SDFSSocket.d \
./src/Utils.d \
./src/main.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


