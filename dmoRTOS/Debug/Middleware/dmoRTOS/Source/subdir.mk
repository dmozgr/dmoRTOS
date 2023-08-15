################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.Rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Middleware/dmoRTOS/Source/task.c 

OBJS += \
./Middleware/dmoRTOS/Source/task.o 

C_DEPS += \
./Middleware/dmoRTOS/Source/task.d 


# Each subdirectory must supply rules for building sources it contributes
Middleware/dmoRTOS/Source/%.o Middleware/dmoRTOS/Source/%.su: ../Middleware/dmoRTOS/Source/%.c Middleware/dmoRTOS/Source/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DSTM32 -DSTM32F407G_DISC1 -DSTM32F4 -DSTM32F407VGTx -c -I../Inc -I"C:/workspace/sau_ce_tasarim_calismasi/dmoRTOS/dmoRTOS/../chip_headers/Device" -I"C:/workspace/sau_ce_tasarim_calismasi/dmoRTOS/dmoRTOS/../chip_headers/Include" -I"C:/workspace/sau_ce_tasarim_calismasi/dmoRTOS/dmoRTOS/Middleware/dmoRTOS/Source/include" -I"C:/workspace/sau_ce_tasarim_calismasi/dmoRTOS/dmoRTOS/Middleware/dmoRTOS/Source/portable/GCC/ARM_CM4F" -I"C:/workspace/sau_ce_tasarim_calismasi/dmoRTOS/dmoRTOS/Middleware/dmoRTOS/Source" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Middleware-2f-dmoRTOS-2f-Source

clean-Middleware-2f-dmoRTOS-2f-Source:
	-$(RM) ./Middleware/dmoRTOS/Source/task.d ./Middleware/dmoRTOS/Source/task.o ./Middleware/dmoRTOS/Source/task.su

.PHONY: clean-Middleware-2f-dmoRTOS-2f-Source

