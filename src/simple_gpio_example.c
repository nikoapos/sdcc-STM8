#include <stdint.h>
#include <stm8.h>
#include <gpio.h>

int main(void)
{
    GPIO_SET_ALL_PORTS_INPUT_PULL_UP_NO_INT();
    GPIO_SET_AS_OUTPUT(D, 4);
    GPIO_SET_AS_PUSH_PULL(D, 4);
    
    GPIO_SET_AS_INPUT(C, 4);

    while(1) {
        if (GPIO_READ_INPUT(C, 4)) {
            GPIO_WRITE_HIGH(D, 4);
        } else {
            GPIO_WRITE_LOW(D, 4);
        }
    }
}
