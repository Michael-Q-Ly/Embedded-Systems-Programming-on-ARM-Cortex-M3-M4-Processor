################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
<<<<<<< HEAD
../Src/main.c \
../Src/sysmem.c 

OBJS += \
./Src/main.o \
./Src/sysmem.o 

C_DEPS += \
./Src/main.d \
=======
../Src/main_1.c \
../Src/sysmem.c 

OBJS += \
./Src/main_1.o \
./Src/sysmem.o 

C_DEPS += \
./Src/main_1.d \
<<<<<<< HEAD
>>>>>>> a7e49dd (Add Example 12 (two versions):)
=======
>>>>>>> 013_svc_number
>>>>>>> 07fc6d4bb3630419988721ba31b856ce3b86637e
./Src/sysmem.d 


# Each subdirectory must supply rules for building sources it contributes
Src/%.o: ../Src/%.c Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DSTM32F429I_DISC1 -DSTM32 -DSTM32F429ZITx -DSTM32F4 -c -I../Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Src

clean-Src:
<<<<<<< HEAD
	-$(RM) ./Src/main.d ./Src/main.o ./Src/sysmem.d ./Src/sysmem.o
=======
	-$(RM) ./Src/main_1.d ./Src/main_1.o ./Src/sysmem.d ./Src/sysmem.o
<<<<<<< HEAD
>>>>>>> a7e49dd (Add Example 12 (two versions):)
=======
>>>>>>> 013_svc_number
>>>>>>> 07fc6d4bb3630419988721ba31b856ce3b86637e

.PHONY: clean-Src

