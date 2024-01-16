#include "bsp/usart.h"

#include <avr/interrupt.h>
#include <printf.h>

#include "bsp/dsa/queue.h"
#include "bsp/util/assert.h"

#ifndef F_CPU
#error "F_CPU must be defined to use the USART driver."
#endif

// Note:
// Although the interface is designed in a generic way to support multiple USART peripherals, this
// implementation only supports one USART peripheral (USART0) on the ATmega328P, because that is the
// only USART peripheral on the microcontroller. Because of this, variables and registers are
// referenced directly instead of some other mechanism (e.g. an array of registers, switch
// statements, and/or macros) to avoid unnecessary overhead and complexity.

// Baud rate calculation macros
#define UBRR(baud)  ((F_CPU / (16UL * baud)) - 1)
#define UBRRH(baud) ((uint8_t)(UBRR(baud) >> 8))
#define UBRRL(baud) ((uint8_t)UBRR(baud))

// Configuration(s) --------------------------------------------------------------------------------

// Initialization flag
static bool usart0_initialized = false;

#define assert_usart0_initialized() \
    bsp_assert(usart0_initialized, "USART0 has not been initialized.")

// USART0 configuration (initialized in usart_init())
static usart_config usart0_config = {0};

// USART0 callback function (initialized in usart_register_callback())
static usart_recv_callback usart0_callback = NULL;

// RX and TX Buffers -------------------------------------------------------------------------------

#define USART0_BUFFER_SIZE 16

QUEUE_DECLARE_STATIC(usart0_rx_queue, char, USART0_BUFFER_SIZE);
QUEUE_DECLARE_STATIC(usart0_tx_queue, char, USART0_BUFFER_SIZE);

// Interrupt handlers ------------------------------------------------------------------------------

/// @brief Data register empty interrupt handler for USART0. Triggered when the USART0 data register
///        is empty and ready to receive more data.
ISR(USART_UDRE_vect) {
    if (!queue_is_empty(&usart0_tx_queue)) {
        // Send the next byte in the TX buffer
        char data;
        queue_dequeue(&usart0_tx_queue, &data);

        UDR0 = data;
    } else {
        // Nothing to send, disable data register empty interrupts
        UCSR0B &= ~_BV(UDRIE0);
    }
}

/// @brief Receive complete interrupt handler for USART0.
ISR(USART_RX_vect) {
    // Read the byte from the data register
    char data = UDR0;

    // Ignore the byte if the RX buffer is full
    if (queue_is_full(&usart0_rx_queue)) return;

    // Echo the byte back to the sender if necessary
    // (Convert carriage returns to newlines)
    if (usart0_config.echo_on_recv) usart_write(BSP_USART0, (data == '\r') ? '\n' : data);

    // Enqueue the byte into the RX buffer
    queue_enqueue(&usart0_rx_queue, &data);

    // Call the callback function if there is one registered
    if (usart0_callback) usart0_callback();
}

// Implementation ----------------------------------------------------------------------------------

void usart_init(usart device, usart_config config) {
    (void)device;

    bsp_assert(!usart0_initialized, "USART0 has already been initialized.");

    // Save the configuration
    usart0_config = config;

    // Set the baud rate
    UBRR0H = (uint8_t)UBRRH(config.baud_rate);
    UBRR0L = (uint8_t)UBRRL(config.baud_rate);

    // Enable the receiver and transmitter blocks
    UCSR0B = _BV(RXEN0) | _BV(TXEN0);

    // Set the frame format: 8data, 1stop bit
    // TODO(Caleb): Support other data lengths and parity modes
    UCSR0C = _BV(UCSZ01) | _BV(UCSZ00);

    // Enable RX and TX interrupts
    UCSR0B |= _BV(RXCIE0);

    // Set the initialization flag
    usart0_initialized = true;
}

bool usart_poll(usart device) {
    (void)device;

    assert_usart0_initialized();

    // Return true if there is data in the RX buffer
    return !queue_is_empty(&usart0_rx_queue);
}

char usart_read(usart device) {
    (void)device;

    assert_usart0_initialized();

    // Spin until there is data in the RX buffer
    while (queue_is_empty(&usart0_rx_queue)) {}

    // Dequeue the byte from the RX buffer
    char data;
    queue_dequeue(&usart0_rx_queue, &data);

    return data;
}

void usart_write(usart device, char c) {
    (void)device;

    assert_usart0_initialized();

    if (c == '\n') {
        // Send a carriage return before the newline (for terminals that require it)
        usart_write(BSP_USART0, '\r');
    }

    // Spin until there is space in the TX buffer
    while (queue_is_full(&usart0_tx_queue)) {}

    // Enqueue the byte in the TX buffer
    queue_enqueue(&usart0_tx_queue, &c);

    // Enable TX interrupts
    UCSR0B |= _BV(UDRIE0);
}

void _putchar(char c) {
    usart_write(BSP_USART0, c);
}

void usart_printf(usart device, const char* format, ...) {
    (void)device;

    assert_usart0_initialized();

    va_list args;
    va_start(args, format);
    vprintf(format, args);
    va_end(args);
}

void usart_vprintf(usart device, const char* format, va_list args) {
    (void)device;

    assert_usart0_initialized();

    vprintf(format, args);
}

void usart_register_callback(usart device, usart_recv_callback on_character_recv) {
    (void)device;

    assert_usart0_initialized();

    usart0_callback = on_character_recv;
}
