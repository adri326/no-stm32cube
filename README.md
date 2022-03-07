# no-stm32cube

This project aims to provide a way to create source code for the stm32f4 chips without the need for the stm32cube IDE.

## Requirements

On arch linux, you will need the following packages:

- `arm-none-eabi-gcc`, for the compiler itself
- `arm-none-eabi-newlib`, for the `nano.specs` specification file
- `stm32f4-headers-git` (AUR), for the header files
- `stlink`, for interacting with the chip
- `openocd`, for debugging the chip

## Installation

First, clone this repository:

```sh
PROJECT_NAME = "my_project"
git clone https://github.com/adri326/no-stm32cube "${PROJECT_NAME}"
cd "${PROJECT_NAME}"

# Edit Makefile and set PROJECT to your PROJECT_NAME
sed -i "s/PROJECT = project/PROJECT = ${PROJECT_NAME}/" Makefile
```

## Compilation

*You should include this section in the README of your project.*

To compile, simply run `make` (which will call `make all`) in the directory of the project.
A successful compilation should look like this:

```
arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -specs=nano.specs -DSTM32F401xE -O0 -g -DDEBUG -DUSE_FULL_ASSERT -I/opt/STM32/STM32F4/ -c stm32f4/stm32f4xx_it.c -o stm32f4/stm32f4xx_it.o
arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -specs=nano.specs -DSTM32F401xE -O0 -g -DDEBUG -DUSE_FULL_ASSERT -I/opt/STM32/STM32F4/ -c stm32f4/syscalls.c -o stm32f4/syscalls.o
arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -specs=nano.specs -DSTM32F401xE -O0 -g -DDEBUG -DUSE_FULL_ASSERT -I/opt/STM32/STM32F4/ -c stm32f4/sysmem.c -o stm32f4/sysmem.o
arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -specs=nano.specs -DSTM32F401xE -O0 -g -DDEBUG -DUSE_FULL_ASSERT -I/opt/STM32/STM32F4/ -c stm32f4/system_stm32f4xx.c -o stm32f4/system_stm32f4xx.o
arm-none-eabi-as -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -gdwarf-2 stm32f4/startup_stm32f401xe.s -o stm32f4/startup_stm32f401xe.o
arm-none-eabi-gcc -o projet.elf src/main.o stm32f4/stm32f4xx_it.o stm32f4/syscalls.o stm32f4/sysmem.o stm32f4/system_stm32f4xx.o stm32f4/startup_stm32f401xe.o -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -Wl,--gc-sections,-Map=projet.map,--cref -specs=nano.specs -lm -lc -lgcc -lnosys -lstdc++ -lsupc++ -T./stm32f4/stm32f401retx_flash.ld
arm-none-eabi-objcopy -O ihex projet.elf projet.hex
arm-none-eabi-objcopy -O binary projet.elf projet.bin
   text    data     bss     dec     hex  filename
   4288     112    1584    5984    1760  projet.elf
```

## Project structure

Pardon how messy this repository is, I'm making my best given the mess that STMicro made available to me.

The HAL files for the stm32f4 chip are in `stm32f4/`.
The makefile compiles all of the `.c` files in this folder by default, but you might want to pick and choose which you need.

The `Makefile` glues all of it together. It is split in several sections.
You should start by modifying the *User section*, then work your way down to the other sections as your needs evolve.

Alternatively, you should be able to easily tweak the Makefile of [jeremyherbert/stm32-templates](https://github.com/jeremyherbert/stm32-templates/tree/master/stm32f4-discovery) to match the structure of this project.
