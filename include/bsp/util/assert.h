#ifndef _CALEBRJC_BSP_UTIL_ASSERT_H_
#define _CALEBRJC_BSP_UTIL_ASSERT_H_

#define bsp_assert(condition, msg_fmt, ...) \
    ((condition) ? (void)0 : assert_handler(__FILE__, __LINE__, msg_fmt, ##__VA_ARGS__))

/// @brief Asserts that the given condition is true.
/// @param condition The condition to assert.
/// @param file The file in which the assertion is being made.
/// @param line The line number at which the assertion is being made.
/// @param msg_fmt The format of the message to print if the condition is false.
void assert_handler(const char* file, int line, const char* msg_fmt, ...);

#endif  // _CALEBRJC_BSP_UTIL_ASSERT_H_
