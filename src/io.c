#include "bsp/io.h"

#include <avr/io.h>

/// The output data register for each IO port
static volatile uint8_t *const PORT_REGISTERS[] = {&PORTB, &PORTC, &PORTD};

/// The data direction register for each IO port
static volatile uint8_t *const DDR_REGISTERS[] = {&DDRB, &DDRC, &DDRD};

/// The input data register for each IO port
static volatile uint8_t *const PIN_REGISTERS[] = {&PINB, &PINC, &PIND};

// Convenience macro for getting the port index from a pin number
#define PORT_MASK     0x18
#define PORT_OFFSET   3
#define PORT_IDX(pin) (((pin)&PORT_MASK) >> PORT_OFFSET)

// Convenience macro for getting the pin index from a pin number
#define PIN_MASK     0x07
#define PIN_IDX(pin) ((pin)&PIN_MASK)

// Convenience macros for getting a register from a pin number
#define PORT_REGISTER(pin) *PORT_REGISTERS[PORT_IDX(pin)]
#define DDR_REGISTER(pin)  *DDR_REGISTERS[PORT_IDX(pin)]
#define PIN_REGISTER(pin)  *PIN_REGISTERS[PORT_IDX(pin)]

void io_configure(io_pin pin, io_config config) {
    switch (config.direction) {
        case IO_DIRECTION_INPUT:
            // Set the pin up as an input
            DDR_REGISTER(pin) &= ~_BV(PIN_IDX(pin));

            // Set the pin's resistor
            switch (config.resistor) {
                case IO_RESISTOR_FLOATING:
                    PORT_REGISTER(pin) &= ~_BV(PIN_IDX(pin));
                    break;
                case IO_RESISTOR_PULLUP:
                    PORT_REGISTER(pin) |= _BV(PIN_IDX(pin));
                    break;
            }
            break;
        case IO_DIRECTION_OUTPUT:
            // Set the pin up as an output
            DDR_REGISTER(pin) |= _BV(PIN_IDX(pin));

            // Set the pin's initial level
            switch (config.initial_level) {
                case IO_LOW:
                    PORT_REGISTER(pin) &= ~_BV(PIN_IDX(pin));
                    break;
                case IO_HIGH:
                    PORT_REGISTER(pin) |= _BV(PIN_IDX(pin));
                    break;
            }
            break;
    }
}

io_logic_level io_read(io_pin pin) {
    //? Note(Caleb): Should we assert that this is an input pin?

    return (PIN_REGISTER(pin) & _BV(PIN_IDX(pin))) ? IO_HIGH : IO_LOW;
}

void io_write(io_pin pin, io_logic_level level) {
    //? Note(Caleb): Should we assert that this is an output pin?

    switch (level) {
        case IO_LOW:
            PORT_REGISTER(pin) &= ~_BV(PIN_IDX(pin));
            break;
        case IO_HIGH:
            PORT_REGISTER(pin) |= _BV(PIN_IDX(pin));
            break;
    }
}

void io_toggle(io_pin pin) {
    //? Note(Caleb): Should we assert that this is an output pin?

    PORT_REGISTER(pin) ^= _BV(PIN_IDX(pin));
}
