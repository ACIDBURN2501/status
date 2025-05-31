/*
 * @copyright MIT
 *
 * @file: status.h
 *
 * @brief
 *    Provides runtime logic for setting, clearing, and querying fault and
 *    warning status bits defined by the application.
 */

#ifndef STATUS_H
#define STATUS_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/**
 * @brief
 *    Internal primitive type aliases used throughout the status module.
 *
 * @details
 *    These typedefs provide more concise and readable type names
 *    suitable for embedded applications. They are defined here to
 *    avoid dependencies on project-wide typedef headers and are
 *    protected by include guards to prevent redefinition.
 */
#ifndef STATUS_PRIMITIVES_DEFINED
#define STATUS_PRIMITIVES_DEFINED

typedef uint16_t u16;
typedef uint32_t u32;
typedef int16_t s16;
typedef int32_t s32;
typedef size_t usize;

/**
 * @brief
 *    Static assertions to ensure type sizes are consistent on target platform.
 */
_Static_assert(sizeof(u16) == 2, "u16 must be 16 bits");
_Static_assert(sizeof(s16) == 2, "s16 must be 16 bits");
_Static_assert(sizeof(u32) == 4, "u32 must be 32 bits");
_Static_assert(sizeof(s32) == 4, "s32 must be 32 bits");

#endif /* STATUS_PRIMITIVES_DEFINED */

/**
 * @def STATUS_ENCODE
 * @brief Encodes a status bank and bit index into a single `u16` ID.
 *
 * @param bank      Logical bank index (0-based). Typically used to group
 *                  faults/warnings.
 *
 * @param bit       Bit position within the bank (0–31). Each bank can store up
 *                  to 32 bits.
 *
 *
 * @return          A compact 16-bit status ID, suitable for use in the
 *                  `status` module.
 *
 * @note
 *    The result must be less than `STATUS_ID_MAX`, as defined by the
 *    application.
 */
#define STATUS_ENCODE(bank, bit) (((bank) << 8) | (bit))

/**
 * @def STATUS_BANK
 * @brief Extracts the bank number from an encoded status ID.
 *
 * @param id        A status ID encoded using `STATUS_ENCODE()`.
 *
 * @return          The bank index (0-based).
 */
#define STATUS_BANK(id)          (((id) >> 8) & 0xFFu)

/**
 * @def STATUS_BIT
 * @brief Extracts the bit index from an encoded status ID.
 *
 * @param id        A status ID encoded using `STATUS_ENCODE()`.
 *
 * @return          The bit index (0–31) within the bank.
 */
#define STATUS_BIT(id)           ((id) & 0xFFu)

/**
 * @brief Status class for categorization.
 */
enum status_class {
        STATUS_CLASS_FAULT = 0,
        STATUS_CLASS_WARNING = 1,
};

/**
 * @brief Initialize the status module.
 */
void status_init(void);

/**
 * @brief Set the given status bit.
 */
void status_set(u16 id);

/**
 * @brief Clear the given status bit.
 */
void status_clear(u16 id);

/**
 * @brief Toggle the given status bit.
 */
void status_toggle(u16 id);

/**
 * @brief Check whether a given status bit is set.
 */
bool status_is_set(u16 id);

/**
 * @brief Check whether any bit in the given class is set.
 */
bool status_any(enum status_class cls);

/**
 * @brief Clear all bits in the given class.
 */
void status_clear_all(enum status_class cls);

/**
 * @brief Snapshot all status registers into a destination buffer.
 *
 * @param cls The class of status.
 * @param dst Destination array (size must be at least NUM_STATUS_BANKS).
 * @param len Number of entries to write (must match NUM_STATUS_BANKS).
 */
void status_snapshot(enum status_class cls, u16 *dst, usize len);

#endif // STATUS_H
