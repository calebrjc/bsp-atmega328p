#ifndef _CALEBRJC_BSP_USART_USART_H_
#define _CALEBRJC_BSP_USART_USART_H_

#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>

/// @brief USART peripherals.

typedef uint8_t usart;

#define BSP_USART0 0x00

/// @brief Baud rates for the USART.
typedef enum {
    USART_BAUD_INVALID = 0U,
    USART_BAUD_9600    = 9600U,
    USART_BAUD_19200   = 19200U,
    USART_BAUD_38400   = 38400U,
    USART_BAUD_57600   = 57600U,
    USART_BAUD_115200  = 115200U,
} usart_baud_rate;

/// @brief Configuration for the USART.
typedef struct {
    /// @brief The baud rate to use.
    usart_baud_rate baud_rate;

    /// @brief Whether or not to echo received characters. If true, all characters received will be
    ///        echoed back to the sender regardless of whether or not the character was read by
    ///        usart_read(), meaning that the user will see the characters sent to the USART device,
    ///        not the characters acknowledged by the application, and that characters will be
    ///        echoed until the RX buffer is full. If false, no characters will be echoed.
    bool echo_on_recv;
} usart_config;

/// @brief A callback to be called when a character is received.
typedef void (*usart_recv_callback)(void);

/// @brief Initialize the USART.
/// @param device The USART to initialize.
/// @param config The configuration to use.
void usart_init(usart device, usart_config config);

/// @brief Return true if the USART has a character to read.
/// @param device The USART to check.
/// @return True if the USART has a character to read.
bool usart_poll(usart device);

/// @brief Return a character read from the USART.
/// @param device The USART to read from.
/// @return A character read the from USART.
char usart_read(usart device);

/// @brief Write a character to the USART.
/// @param device The USART to write to.
/// @param c The character to write to the USART.
void usart_write(usart device, char c);

/// @brief Output a character to the USART.
/// @param c The character to output.
void _putchar(char c);

/// @brief Output a formatted string to the USART.
/// @param device The USART to output to.
/// @param format The format string to output.
/// @param ... The arguments to the format string.
void usart_printf(usart device, const char* format, ...);

/// @brief Output a formatted string to the USART.
/// @param device The USART to output to.
/// @param format The format string to output.
/// @param args The arguments to the format string.
void usart_vprintf(usart device, const char* format, va_list args);

/// @brief Register a callback to be called when a character is received. Note: The registered
/// callback will be called in an interrupt context.
/// @param device The USART to register the callback for.
/// @param callback The callback to register.
void usart_register_callback(usart device, usart_recv_callback on_character_recv);

#endif  // _CALEBRJC_BSP_USART_USART_H_
