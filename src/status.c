/*
 * @copyright MIT
 *
 * @file: status.h
 *
 * @brief Implementation of core status tracking functionality.
 */

#include <stdbool.h>

#include "../include/status.h"

static uint16_t fault_banks[NUM_STATUS_BANKS];
static uint16_t warning_banks[NUM_STATUS_BANKS];

/**
 * @brief
 *    Internal helper to validate that a status ID is within legal bounds.
 *
 * @details
 *    Checks that the bank index is less than `NUM_STATUS_BANKS` and
 *    the bit index is less than 16. Fails via ASSERT if out of range.
 *
 * @param id
 *    A status ID encoded using `STATUS_ENCODE()`.
 *
 * @note
 *    This is used by all public status APIs to enforce safety at runtime.
 */
static inline void
assert_valid(uint16_t id)
{
        ASSERT(status_bank(id) < NUM_STATUS_BANKS);
        ASSERT(status_bit(id) < 16u);
}

void
status_init(void)
{
        for (size_t i = 0; i < NUM_STATUS_BANKS; ++i) {
                fault_banks[i] = 0u;
                warning_banks[i] = 0u;
        }
}

static inline uint16_t *
get_bank_array(enum status_class cls)
{
        return (cls == STATUS_CLASS_FAULT) ? fault_banks : warning_banks;
}

void
status_set_warning(uint16_t id)
{
        assert_valid(id);
        uint16_t bank = status_bank(id);
        uint16_t bit = status_bit(id);

        if (bank < NUM_STATUS_BANKS) {
                warning_banks[bank] |= (uint16_t)(1u << bit);
        }
}

void
status_set_fault(uint16_t id)
{
        assert_valid(id);
        uint16_t bank = status_bank(id);
        uint16_t bit = status_bit(id);
        if (bank < NUM_STATUS_BANKS) {
                fault_banks[bank] |= (uint16_t)(1u << bit);
        }
}

void
status_clear_warning(uint16_t id)
{
        assert_valid(id);
        uint16_t bank = status_bank(id);
        uint16_t bit = status_bit(id);

        if (bank < NUM_STATUS_BANKS) {
                warning_banks[bank] &= ~(uint16_t)(1u << bit);
        }
}

void
status_clear_fault(uint16_t id)
{
        assert_valid(id);
        uint16_t bank = status_bank(id);
        uint16_t bit = status_bit(id);

        if (bank < NUM_STATUS_BANKS) {
                fault_banks[bank] &= ~(uint16_t)(1u << bit);
        }
}

void
status_toggle_warning(uint16_t id)
{
        assert_valid(id);
        uint16_t bank = status_bank(id);
        uint16_t bit = status_bit(id);

        if (bank < NUM_STATUS_BANKS) {
                warning_banks[bank] ^= (uint16_t)(1u << bit);
        }
}

void
status_toggle_fault(uint16_t id)
{
        assert_valid(id);
        uint16_t bank = status_bank(id);
        uint16_t bit = status_bit(id);

        if (bank < NUM_STATUS_BANKS) {
                fault_banks[bank] ^= (uint16_t)(1u << bit);
        }
}

bool
status_is_warning_set(uint16_t id)
{
        assert_valid(id);
        uint16_t bank = status_bank(id);
        uint16_t bit = status_bit(id);

        if (bank >= NUM_STATUS_BANKS) {
                return false;
        }

        return (warning_banks[bank] & (uint16_t)(1u << bit)) != 0;
}

bool
status_is_fault_set(uint16_t id)
{
        assert_valid(id);
        uint16_t bank = status_bank(id);
        uint16_t bit = status_bit(id);

        if (bank >= NUM_STATUS_BANKS) {
                return false;
        }

        return (fault_banks[bank] & (uint16_t)(1u << bit)) != 0;
}

bool
status_any(enum status_class cls)
{
        const uint16_t *banks = get_bank_array(cls);

        if (banks == NULL) {
                return false;
        }

        for (size_t i = 0; i < NUM_STATUS_BANKS; ++i) {
                if (banks[i] != 0) {
                        return true;
                }
        }
        return false;
}

void
status_clear_all(enum status_class cls)
{
        uint16_t *banks = get_bank_array(cls);

        if (banks == NULL) {
                return;
        }

        for (size_t i = 0; i < NUM_STATUS_BANKS; ++i) {
                banks[i] = 0;
        }
}

void
status_snapshot(enum status_class cls, uint16_t *dst, size_t len)
{
        const uint16_t *banks = get_bank_array(cls);
        for (size_t i = 0; i < len && i < NUM_STATUS_BANKS; ++i) {
                dst[i] = banks[i];
        }
}
