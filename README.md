# no-stm32cube

This project aims to provide a way to create source code for the stm32f4 chips without the need for the stm32cube IDE.

## Requirements

On arch linux, you will need the following packages:

- `arm-none-eabi-gcc`, for the compiler itself
- `arm-none-eabi-newlib`, for the `nano.specs` specification file, used by the compiler
- `arm-none-eabi-gdb`, for debugging
- `stm32f4-headers-git` (AUR), for the header files
- `stlink`, for interacting with the chip
- `openocd`, for debugging the chip

## Installation

The recommended installation procedure is as follows:
- clone the repository
- switch to a new branch (this repository uses the branch name `no-stm32cube`, so it shouldn't interfere with yours)
- edit the makefile to fit your project

```sh
PROJECT_NAME = "my_project"
git clone https://github.com/adri326/no-stm32cube "${PROJECT_NAME}"
cd "${PROJECT_NAME}"

git switch -c main

# Edit Makefile and set PROJECT to your PROJECT_NAME
sed -i "s/PROJECT = project/PROJECT = ${PROJECT_NAME}/" Makefile
```

### As a user

If, in the future, you plan on releasing your project, then users of your project will need to do the following:
- get the required dependencies (for compiling and/or flashing the chip)
- download the code (through `git clone` or by downloading an archive)
- compile the code with `make`
- flash the code with `make flash`

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

Then, run `make flash` to flash the board.

### Debugging

To debug, you will first need to run `make openocd`. This will call `openocd`, which will act as a bridge between `gdb` and the chip.
Next, run `make gdb` while `make openocd` is running. You should then be able to `continue` the code and see it run.

The `DEBUG` flag in the makefile toggles between a debug build and a release build. It is set to 1 by default.
If you do change it, then you should re-run the compilation with `make --always-make`, so that it correctly applies the new setting to every file.

When building in debug mode, the macros will be exported in the `.elf` file.
These will be available in gdb as soon as you have `continue`d the code.

## Project structure

Please pardon how messy this repository is, I'm making my best given the mess that STMicro made available to me.

The HAL files for the stm32f4 chip are in `stm32f4/`.
The makefile compiles all of the `.c` files in this folder by default, but you might want to pick and choose which you need.

The `Makefile` glues everything together; it is split in several sections.
You should start by modifying the *User section*, then work your way down to the other sections as your needs evolve.

Alternatively, you should be able to easily tweak the Makefile of [jeremyherbert/stm32-templates](https://github.com/jeremyherbert/stm32-templates/tree/master/stm32f4-discovery) to match the structure of this project.
