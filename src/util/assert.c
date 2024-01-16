#include "bsp/util/assert.h"

#include <stdarg.h>
#include <util/delay.h>

#include "bsp/io.h"
#include "bsp/usart.h"

void assert_handler(const char* file, int line, const char* msg_fmt, ...) {
    // Print the file and line number.
    usart_printf(BSP_USART0, "Assertion failed in %s on line %d: ", file, line);

    // Print the message.
    va_list args;
    va_start(args, msg_fmt);
    usart_vprintf(BSP_USART0, msg_fmt, args);
    va_end(args);

    // Print a newline.
    usart_printf(BSP_USART0, "\r\n");

    // Flash the debug LED forever.
    io_configure(BSP_PB5, (io_config){.direction = IO_DIRECTION_OUTPUT});
    while (1) {
        io_toggle(BSP_PB5);
        _delay_ms(500);
    }
}
