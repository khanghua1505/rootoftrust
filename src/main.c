#include <stdint.h>
#include <stdbool.h>
#include "serial.h"
#include "cmd_handler.h"

void System_init(void)
{
    serial_init();
}

int main(void)
{
    System_init();
    
    while (1) {
        keycore_cmd_handler();
    }
}
