#include <inttypes.h>
#include <stdbool.h>
#include <stm32f4xx.h>

// This simple example reads the value of the "user" button on the board and outputs that value to the PC5 led

// Toggles on and off a mask
void set_mask(volatile uint32_t* target, uint32_t mask, bool state) {
    if (state) *target |= mask;
    else *target &= ~mask;
}

// Sets `bits` bits, offsetted `offset_low` to the left, to the value in `state` into `target`
void set_nbit(volatile uint32_t* target, uint8_t offset_low, uint8_t bits, uint32_t state) {
    uint32_t mask = ((1 << bits) - 1);
    *target = (*target & ~(mask << offset_low)) | (state & mask) << offset_low;
}

#define set_twobit(target, offset_low, state) set_nbit(target, offset_low, 2, state)
#define set_bit(target, offset_low, state) set_nbit(target, offset_low, 1, state)

// Sets the GPIO mode of a port
#define MODE_READ 0b00
#define MODE_WRITE 0b01
#define MODE_ALTERNATE 0b10
#define set_mode(gpio, number, state) set_twobit(&gpio->MODER, GPIO_MODER_MODER##number##_Pos, state)

bool read_bit(volatile uint32_t* target, uint8_t bit) {
    return (*target >> bit) & 1;
}

int main() {
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN | RCC_AHB1ENR_GPIOCEN; // Enable the clock for the GPIOA and GPIOC peripherals

    set_mode(GPIOA, 5, MODE_WRITE); // write

    set_mode(GPIOC, 13, MODE_READ); // read

    while (true) {
        bool state = !read_bit(&GPIOC->IDR, 13);

        set_mask(&GPIOA->ODR, GPIO_ODR_ODR_5, state);
    }
}
