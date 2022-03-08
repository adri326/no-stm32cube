#include <inttypes.h>
#include <stdbool.h>
#include <stm32f4xx.h>

// This simple example reads the value of the "user" button on the board and outputs that value to the PC5 led

void set_mask(volatile uint32_t* target, uint32_t mask, bool state) {
    if (state) *target |= mask;
    else *target &= ~mask;
}

void set_twobit(volatile uint32_t* target, uint8_t offset_low, uint8_t state) {
    *target = (*target & ~(0x11 << offset_low)) | state << offset_low;
}

#define MODE_READ 0b00
#define MODE_WRITE 0b01
#define MODE_ALTERNATE 0b10
#define set_mode(gpio, number, state) set_twobit(&gpio->MODER, GPIO_MODER_MODER##number##_Pos, state)

#define PUPD_NONE 0b00
#define PUPD_UP 0b01
#define PUPD_DOWN 0b10
#define set_pupd(gpio, number, state) set_twobit(&gpio->PUPDR, GPIO_PUPDR_PUPD##number##_Pos, state)

void set_bit(volatile uint32_t* target, uint8_t bit, bool state) {
    set_mask(target, 1 << bit, state);
}

bool read_bit(volatile uint32_t* target, uint8_t bit) {
    return (*target >> bit) & 1;
}

int main() {
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN | RCC_AHB1ENR_GPIOCEN; // Enable the clock for the GPIOA and GPIOC peripherals

    set_mode(GPIOA, 5, MODE_WRITE); // write

    set_mode(GPIOC, 13, MODE_READ); // read

    // set_mode(GPIOA, 0, MODE_READ); // read
    // set_mode(GPIOA, 1, MODE_WRITE); // write

    // set_pupd(GPIOA, 0, PUPD_UP); // pull-up
    // set_bit(&GPIOA->OTYPER, GPIO_OTYPER_OT_1, true); // open-drain

    while (true) {
        bool state = !read_bit(&GPIOC->IDR, 13);
        set_mask(&GPIOA->ODR, GPIO_ODR_ODR_5, state);

        // bool state2 = read_bit(&GPIOA->IDR, 0);
        // set_mask(&GPIOA->ODR, GPIO_ODR_ODR_1, state2);
    }
}
