#ifndef _CALEBRJC_BSP_IO_IO_H_
#define _CALEBRJC_BSP_IO_IO_H_

#include <stdint.h>

/// @brief IO pins.

// Each pin number is an 8-bit integer represented as shown below:
// [ zeroes (7:5) | port index (4:3) | pin index (2:0) ]

typedef uint8_t io_pin;

#define BSP_PB0 0x00  // Port B, Pin 0
#define BSP_PB1 0x01  // Port B, Pin 1
#define BSP_PB2 0x02  // Port B, Pin 2
#define BSP_PB3 0x03  // Port B, Pin 3
#define BSP_PB4 0x04  // Port B, Pin 4
#define BSP_PB5 0x05  // Port B, Pin 5
#define BSP_PB6 0x06  // Port B, Pin 6
#define BSP_PB7 0x07  // Port B, Pin 7

#define BSP_PC0 0x08  // Port C, Pin 0
#define BSP_PC1 0x09  // Port C, Pin 1
#define BSP_PC2 0x0A  // Port C, Pin 2
#define BSP_PC3 0x0B  // Port C, Pin 3
#define BSP_PC4 0x0C  // Port C, Pin 4
#define BSP_PC5 0x0D  // Port C, Pin 5
#define BSP_PC6 0x0E  // Port C, Pin 6
#define BSP_PC7 0x0F  // Port C, Pin 7

#define BSP_PD0 0x10  // Port D, Pin 0
#define BSP_PD1 0x11  // Port D, Pin 1
#define BSP_PD2 0x12  // Port D, Pin 2
#define BSP_PD3 0x13  // Port D, Pin 3
#define BSP_PD4 0x14  // Port D, Pin 4
#define BSP_PD5 0x15  // Port D, Pin 5
#define BSP_PD6 0x16  // Port D, Pin 6
#define BSP_PD7 0x17  // Port D, Pin 7

/// @brief The logic level of an IO pin.
typedef enum {
    /// @brief Represents a logic low.
    IO_LOW,

    /// @brief Represents a logic high.
    IO_HIGH,
} io_logic_level;

/// @brief The direction of an IO pin.
typedef enum {
    /// @brief Represents an input pin.
    IO_DIRECTION_INPUT,

    /// @brief Represents an output pin.
    IO_DIRECTION_OUTPUT,
} io_direction;

/// @brief The resistor configuration of an IO pin.
typedef enum {
    /// @brief Represents a floating pin.
    IO_RESISTOR_FLOATING,

    /// @brief Represents a pulled up pin.
    IO_RESISTOR_PULLUP,
} io_resistor;

/// @brief The configuration of an IO pin.
typedef struct {
    /// @brief The direction of the pin.
    io_direction direction;

    /// @brief Whether the pin is pulled up or not.
    io_resistor resistor;

    /// @brief The initial logic level of the pin.
    io_logic_level initial_level;
} io_config;

/// @brief Configure an IO pin.
/// @param pin The pin to configure.
/// @param config The configuration to apply to the pin.
void io_configure(io_pin pin, io_config config);

/// @brief Return the logic level of an IO pin.
/// @param pin The pin to read.
/// @return The logic level of an IO pin.
io_logic_level io_read(io_pin pin);

/// @brief Write a logic level to an IO pin.
/// @param pin The pin to write to.
/// @param level The logic level to write.
void io_write(io_pin pin, io_logic_level level);

/// @brief Toggle the logic level of an IO pin.
/// @param pin The pin to toggle.
void io_toggle(io_pin pin);

#endif  // _CALEBRJC_BSP_IO_IO_H_
