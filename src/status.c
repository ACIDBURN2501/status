/*
 * @copyright MIT
 *
 * @file: status.h
 *
 * @brief Implementation of core status tracking functionality.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

#include "../include/status.h"

/* ---------------- Configuration ------------------------------------------- */

static uint16_t fault_banks[NUM_STATUS_BANKS];
static uint16_t warning_banks[NUM_STATUS_BANKS];
static uint16_t info_banks[NUM_STATUS_BANKS];

static uint16_t last_fault_id = 0xFFFF;
static uint16_t last_warning_id = 0xFFFF;
static uint16_t last_info_id = 0xFFFF;

/* ---------------- Helpers ------------------------------------------------- */

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
_is_valid(uint16_t id)
{
        ASSERT(status_bank(id) < NUM_STATUS_BANKS);
        ASSERT(status_bit(id) < 16u);
}

static inline uint16_t *
_get_bank_array(enum status_class cls)
{
        switch (cls) {
        case STATUS_CLASS_FAULT: return fault_banks;
        case STATUS_CLASS_WARNING: return warning_banks;
        case STATUS_CLASS_INFO: return info_banks;
        default: return NULL;
        }
}

void
_set(uint16_t id, enum status_class cls)
{
        _is_valid(id);
        uint16_t bank = status_bank(id);
        uint16_t bit = status_bit(id);

        uint16_t *b = _get_bank_array(cls);
        if (b == NULL) {
                return;
        }

        if (bank < NUM_STATUS_BANKS) {
                b[bank] |= (uint16_t)(1u << bit);
                switch (cls) {
                case STATUS_CLASS_FAULT: last_fault_id = id; break;
                case STATUS_CLASS_WARNING: last_warning_id = id; break;
                case STATUS_CLASS_INFO: last_info_id = id; break;
                }
        }
}

void
_clear(uint16_t id, enum status_class cls)
{
        _is_valid(id);
        uint16_t bank = status_bank(id);
        uint16_t bit = status_bit(id);

        uint16_t *b = _get_bank_array(cls);
        if (b == NULL) {
                return;
        }

        if (bank < NUM_STATUS_BANKS) {
                b[bank] &= ~(uint16_t)(1u << bit);
        }
}

void
_toggle(uint16_t id, enum status_class cls)
{
        _is_valid(id);
        uint16_t bank = status_bank(id);
        uint16_t bit = status_bit(id);

        uint16_t *b = _get_bank_array(cls);
        if (b == NULL) {
                return;
        }

        if (bank < NUM_STATUS_BANKS) {
                b[bank] ^= (uint16_t)(1u << bit);
        }
}

bool
_is_set(uint16_t id, enum status_class cls)
{
        _is_valid(id);
        uint16_t bank = status_bank(id);
        uint16_t bit = status_bit(id);

        if (bank >= NUM_STATUS_BANKS) {
                return false;
        }

        uint16_t *b = _get_bank_array(cls);
        if (b == NULL) {
                return false;
        }

        return (b[bank] & (uint16_t)(1u << bit)) != 0;
}

/* ---------------  Public Interface ---------------------------------------- */

void
status_init(void)
{
        memset(fault_banks, 0u, sizeof(fault_banks));
        memset(warning_banks, 0u, sizeof(warning_banks));
        memset(info_banks, 0u, sizeof(info_banks));
        last_fault_id = 0xFFFF;
        last_warning_id = 0xFFFF;
        last_info_id = 0xFFFF;
}

void
status_set_warning(uint16_t id)
{
        _set(id, STATUS_CLASS_WARNING);
}

void
status_set_fault(uint16_t id)
{
        _set(id, STATUS_CLASS_FAULT);
}

void
status_set_info(uint16_t id)
{
        _set(id, STATUS_CLASS_INFO);
}

void
status_clear_warning(uint16_t id)
{
        _clear(id, STATUS_CLASS_WARNING);
}

void
status_clear_fault(uint16_t id)
{
        _clear(id, STATUS_CLASS_FAULT);
}

void
status_clear_info(uint16_t id)
{
        _clear(id, STATUS_CLASS_INFO);
}

void
status_toggle_warning(uint16_t id)
{
        _toggle(id, STATUS_CLASS_WARNING);
}

void
status_toggle_fault(uint16_t id)
{
        _toggle(id, STATUS_CLASS_FAULT);
}

void
status_toggle_info(uint16_t id)
{
        _toggle(id, STATUS_CLASS_INFO);
}

bool
status_is_warning_set(uint16_t id)
{
        return _is_set(id, STATUS_CLASS_WARNING);
}

bool
status_is_fault_set(uint16_t id)
{
        return _is_set(id, STATUS_CLASS_FAULT);
}

bool
status_is_info_set(uint16_t id)
{
        return _is_set(id, STATUS_CLASS_INFO);
}

bool
status_any(enum status_class cls)
{
        const uint16_t *banks = _get_bank_array(cls);

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
        uint16_t *banks = _get_bank_array(cls);

        if (banks == NULL) {
                return;
        }

        for (size_t i = 0; i < NUM_STATUS_BANKS; ++i) {
                banks[i] = 0;
        }
}

uint16_t
status_last_fault(void)
{
        return last_fault_id;
}

uint16_t
status_last_warning(void)
{
        return last_warning_id;
}

uint16_t
status_last_info(void)
{
        return last_info_id;
}

void
status_snapshot(enum status_class cls, uint16_t *dst, size_t len)
{
        const uint16_t *banks = _get_bank_array(cls);
        for (size_t i = 0; i < len && i < NUM_STATUS_BANKS; ++i) {
                dst[i] = banks[i];
        }
}
