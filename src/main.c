#include <stdbool.h>
#include <stm32f4xx.h>

// Example blinky code
int main(void) {
    GPIOD->MODER = 1 << 26; // set pin 13 to be general purpose output

    while (true) {
        GPIOD->ODR ^= 1 << 13; // invert the value of pin 13
    }
}
