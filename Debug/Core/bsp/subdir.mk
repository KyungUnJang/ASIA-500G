################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/bsp/at24_eep.c \
../Core/bsp/bsp_adc.c \
../Core/bsp/bsp_pwm.c \
../Core/bsp/bsp_uart.c \
../Core/bsp/cli.c \
../Core/bsp/debug.c \
../Core/bsp/hw_config.c \
../Core/bsp/indicate.c \
../Core/bsp/keyscan.c \
../Core/bsp/motor.c \
../Core/bsp/qbuffer.c \
../Core/bsp/task.c \
../Core/bsp/util.c 

OBJS += \
./Core/bsp/at24_eep.o \
./Core/bsp/bsp_adc.o \
./Core/bsp/bsp_pwm.o \
./Core/bsp/bsp_uart.o \
./Core/bsp/cli.o \
./Core/bsp/debug.o \
./Core/bsp/hw_config.o \
./Core/bsp/indicate.o \
./Core/bsp/keyscan.o \
./Core/bsp/motor.o \
./Core/bsp/qbuffer.o \
./Core/bsp/task.o \
./Core/bsp/util.o 

C_DEPS += \
./Core/bsp/at24_eep.d \
./Core/bsp/bsp_adc.d \
./Core/bsp/bsp_pwm.d \
./Core/bsp/bsp_uart.d \
./Core/bsp/cli.d \
./Core/bsp/debug.d \
./Core/bsp/hw_config.d \
./Core/bsp/indicate.d \
./Core/bsp/keyscan.d \
./Core/bsp/motor.d \
./Core/bsp/qbuffer.d \
./Core/bsp/task.d \
./Core/bsp/util.d 


# Each subdirectory must supply rules for building sources it contributes
Core/bsp/%.o Core/bsp/%.su Core/bsp/%.cyclo: ../Core/bsp/%.c Core/bsp/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xE -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Core-2f-bsp

clean-Core-2f-bsp:
	-$(RM) ./Core/bsp/at24_eep.cyclo ./Core/bsp/at24_eep.d ./Core/bsp/at24_eep.o ./Core/bsp/at24_eep.su ./Core/bsp/bsp_adc.cyclo ./Core/bsp/bsp_adc.d ./Core/bsp/bsp_adc.o ./Core/bsp/bsp_adc.su ./Core/bsp/bsp_pwm.cyclo ./Core/bsp/bsp_pwm.d ./Core/bsp/bsp_pwm.o ./Core/bsp/bsp_pwm.su ./Core/bsp/bsp_uart.cyclo ./Core/bsp/bsp_uart.d ./Core/bsp/bsp_uart.o ./Core/bsp/bsp_uart.su ./Core/bsp/cli.cyclo ./Core/bsp/cli.d ./Core/bsp/cli.o ./Core/bsp/cli.su ./Core/bsp/debug.cyclo ./Core/bsp/debug.d ./Core/bsp/debug.o ./Core/bsp/debug.su ./Core/bsp/hw_config.cyclo ./Core/bsp/hw_config.d ./Core/bsp/hw_config.o ./Core/bsp/hw_config.su ./Core/bsp/indicate.cyclo ./Core/bsp/indicate.d ./Core/bsp/indicate.o ./Core/bsp/indicate.su ./Core/bsp/keyscan.cyclo ./Core/bsp/keyscan.d ./Core/bsp/keyscan.o ./Core/bsp/keyscan.su ./Core/bsp/motor.cyclo ./Core/bsp/motor.d ./Core/bsp/motor.o ./Core/bsp/motor.su ./Core/bsp/qbuffer.cyclo ./Core/bsp/qbuffer.d ./Core/bsp/qbuffer.o ./Core/bsp/qbuffer.su ./Core/bsp/task.cyclo ./Core/bsp/task.d ./Core/bsp/task.o ./Core/bsp/task.su ./Core/bsp/util.cyclo ./Core/bsp/util.d ./Core/bsp/util.o ./Core/bsp/util.su

.PHONY: clean-Core-2f-bsp

