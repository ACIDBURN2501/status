/*
 * @copyright MIT
 *
 * @file: status.h
 *
 * @brief Provides runtime logic for setting, clearing, and querying fault and
 *        warning status bits defined by the application.
 */

#ifndef STATUS_H
#define STATUS_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#ifndef ASSERT
#ifdef DEBUG
#define ASSERT(c)                                                              \
        do {                                                                   \
                if (!(c))                                                      \
                        __builtin_trap();                                      \
        } while (0)
#else
#define ASSERT(c) ((void)0)
#endif
#endif

/**
 * @brief Optional primitive type aliases used throughout the status module.
 *
 * @note
 *    These will only be defined if the user explicity enables them by defining
 *    the `STATUS_U16_TYPE` or `STATUS_USIZE_TYPE`. The avoids conflicts with
 *    global project typedefs.
 */
#ifdef STATUS_U16_TYPE
typedef uint16_t u16;
#endif
ifdef STATUS_USIZE_TYPE typedef size_t usize;
#endif

/**
 * @brief
 *    Static assertions to ensure type sizes are consistent on target platform.
 */
_Static_assert(sizeof(u16) == 2, "u16 must be 16 bits");

/**
 * @def NUM_STATUS_BANKS
 * @brief The number of internal banks available for fault and warning bits.
 *
 * @details
 *    Each bank holds 16 bits. Users must ensure that any status ID encoded
 *    via `STATUS_ENCODE(bank, bit)` uses a `bank` value less than this.
 *
 * @note
 *    This value must match the maximum `bank + 1` used in status_ids.h.
 */
#define NUM_STATUS_BANKS         (8u)

/**
 * @name Status ID encoding helpers.
 * @{
 */

/**
 * @def STATUS_ENCODE
 * @brief Encodes a status bank and bit index into a single 16-bit status ID.
 *
 * @param bank      Logical bank index (0-based). Typically used to group
 *                  faults/warnings.
 *
 * @param bit       Bit position within the bank (0–15). Each bank can store up
 *                  to 16 bits.
 *
 * @details
 *    Each bank is assumed to store 16 bits. This macro encodes a `bank` and
 *    `bit` index into a compact 16-bit ID used for indexing status registers.
 *    The result can be passed to `status_set_fault()`, `status_clear_warn()`,
 *    etc.
 *
 * @return          A compact 16-bit status ID, suitable for use in the
 *                  `status` module.
 *
 * @note
 *    The maximum bit index is 15. Higher values are masked off.
 */
#define STATUS_ENCODE(bank, bit) (((u16)(bank) << 4u) | ((u16)(bit) & 0x0Fu))

/**
 * @brief Extracts the bank number from an encoded status ID.
 *
 * @param id        A status ID encoded using `STATUS_ENCODE()`.
 *
 * @return          The bank index (0-based).
 */
static inline u16
status_bank(u16 id)
{
        return (u16)(id >> 4u);
}

/**
 * @brief Extracts the bit index from an encoded status ID.
 *
 * @param id        A status ID encoded using `STATUS_ENCODE()`.
 *
 * @return          The bit index (0–15) within the bank.
 */
static inline u16
status_bit(u16 id)
{
        return (u16)(id & 0x0Fu);
}

/**
 * End: Status ID encoding helpers.
 * @}
 */

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
 * @brief Set the given warning status bit.
 */
void status_set_warning(u16 id);

/**
 * @brief Set the given fault status bit.
 */
void status_set_fault(u16 id);

/**
 * @brief Clear the given warning status bit.
 */
void status_clear_warning(u16 id);

/**
 * @brief Clear the given fault status bit.
 */
void status_clear_fault(u16 id);

/**
 * @brief Toggle the given warning status bit.
 */
void status_toggle_warning(u16 id);

/**
 * @brief Toggle the given fault status bit.
 */
void status_toggle_fault(u16 id);

/**
 * @brief Check whether a given warning status bit is set.
 */
bool status_is_warning_set(u16 id);

/**
 * @brief Check whether a given fault status bit is set.
 */
bool status_is_fault_set(u16 id);

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
 * @param cls       The class of status.
 * @param dst       Destination array (size must be at least NUM_STATUS_BANKS).
 * @param len       Number of entries to write (must match NUM_STATUS_BANKS).
 */
void status_snapshot(enum status_class cls, u16 *dst, usize len);

#endif // STATUS_H
