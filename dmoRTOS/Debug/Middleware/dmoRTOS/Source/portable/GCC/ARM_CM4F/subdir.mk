################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.Rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Middleware/dmoRTOS/Source/portable/GCC/ARM_CM4F/port.c 

OBJS += \
./Middleware/dmoRTOS/Source/portable/GCC/ARM_CM4F/port.o 

C_DEPS += \
./Middleware/dmoRTOS/Source/portable/GCC/ARM_CM4F/port.d 


# Each subdirectory must supply rules for building sources it contributes
Middleware/dmoRTOS/Source/portable/GCC/ARM_CM4F/%.o Middleware/dmoRTOS/Source/portable/GCC/ARM_CM4F/%.su: ../Middleware/dmoRTOS/Source/portable/GCC/ARM_CM4F/%.c Middleware/dmoRTOS/Source/portable/GCC/ARM_CM4F/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DSTM32 -DSTM32F407G_DISC1 -DSTM32F4 -DSTM32F407VGTx -c -I../Inc -I"C:/workspace/sau_ce_tasarim_calismasi/dmoRTOS/dmoRTOS/../chip_headers/Device" -I"C:/workspace/sau_ce_tasarim_calismasi/dmoRTOS/dmoRTOS/../chip_headers/Include" -I"C:/workspace/sau_ce_tasarim_calismasi/dmoRTOS/dmoRTOS/Middleware/dmoRTOS/Source/include" -I"C:/workspace/sau_ce_tasarim_calismasi/dmoRTOS/dmoRTOS/Middleware/dmoRTOS/Source/portable/GCC/ARM_CM4F" -I"C:/workspace/sau_ce_tasarim_calismasi/dmoRTOS/dmoRTOS/Middleware/dmoRTOS/Source" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Middleware-2f-dmoRTOS-2f-Source-2f-portable-2f-GCC-2f-ARM_CM4F

clean-Middleware-2f-dmoRTOS-2f-Source-2f-portable-2f-GCC-2f-ARM_CM4F:
	-$(RM) ./Middleware/dmoRTOS/Source/portable/GCC/ARM_CM4F/port.d ./Middleware/dmoRTOS/Source/portable/GCC/ARM_CM4F/port.o ./Middleware/dmoRTOS/Source/portable/GCC/ARM_CM4F/port.su

.PHONY: clean-Middleware-2f-dmoRTOS-2f-Source-2f-portable-2f-GCC-2f-ARM_CM4F

