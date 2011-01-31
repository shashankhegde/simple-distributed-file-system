################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../DHCP.cpp \
../DNS.cpp \
../Decider.cpp \
../Listener.cpp \
../Router.cpp \
../SDFSSocket.cpp \
../Utils.cpp \
../main.cpp 

OBJS += \
./DHCP.o \
./DNS.o \
./Decider.o \
./Listener.o \
./Router.o \
./SDFSSocket.o \
./Utils.o \
./main.o 

CPP_DEPS += \
./DHCP.d \
./DNS.d \
./Decider.d \
./Listener.d \
./Router.d \
./SDFSSocket.d \
./Utils.d \
./main.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


