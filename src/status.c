/*
 * @copyright MIT
 *
 * @file: status.h
 *
 * @brief Implementation of core status tracking functionality.
 */

#include "../include/status.h"

static u16 fault_banks[NUM_STATUS_BANKS];
static u16 warning_banks[NUM_STATUS_BANKS];

void
status_init(void)
{
        for (usize i = 0; i < NUM_STATUS_BANKS; ++i) {
                fault_banks[i] = 0u;
                warning_banks[i] = 0u;
        }
}

static inline u16 *
get_bank_array(enum status_class cls)
{
        return (cls == STATUS_CLASS_FAULT) ? fault_banks : warning_banks;
}

void
status_set_warning(u16 id)
{
        u16 bank = status_bank(id);
        u16 bit = status_bit(id);
        if (bank < NUM_STATUS_BANKS) {
                warning_banks[bank] |= (u16)(1u << bit);
        }
}

void
status_set_fault(u16 id)
{
        u16 bank = status_bank(id);
        u16 bit = status_bit(id);
        if (bank < NUM_STATUS_BANKS) {
                fault_banks[bank] |= (u16)(1u << bit);
        }
}

void
status_clear_warning(u16 id)
{
        u16 bank = status_bank(id);
        u16 bit = status_bit(id);
        if (bank < NUM_STATUS_BANKS) {
                warning_banks[bank] &= ~(u16)(1u << bit);
        }
}

void
status_clear_fault(u16 id)
{
        u16 bank = status_bank(id);
        u16 bit = status_bit(id);
        if (bank < NUM_STATUS_BANKS) {
                fault_banks[bank] &= ~(u16)(1u << bit);
        }
}

void
status_toggle_warning(u16 id)
{
        u16 bank = status_bank(id);
        u16 bit = status_bit(id);
        if (bank < NUM_STATUS_BANKS) {
                warning_banks[bank] ^= (u16)(1u << bit);
        }
}

void
status_toggle_fault(u16 id)
{
        u16 bank = status_bank(id);
        u16 bit = status_bit(id);
        if (bank < NUM_STATUS_BANKS) {
                fault_banks[bank] ^= (u16)(1u << bit);
        }
}

bool
status_is_warning_set(u16 id)
{
        u16 bank = status_bank(id);
        u16 bit = status_bit(id);
        if (bank >= NUM_STATUS_BANKS) {
                return false;
        }
        return (warning_banks[bank] & (u16)(1u << bit)) != 0;
}

bool
status_is_fault_set(u16 id)
{
        u16 bank = status_bank(id);
        u16 bit = status_bit(id);
        if (bank >= NUM_STATUS_BANKS) {
                return false;
        }
        return (fault_banks[bank] & (u16)(1u << bit)) != 0;
}

bool
status_any(enum status_class cls)
{
        const u16 *banks = get_bank_array(cls);
        for (usize i = 0; i < NUM_STATUS_BANKS; ++i) {
                if (banks[i] != 0) {
                        return true;
                }
        }
        return false;
}

void
status_clear_all(enum status_class cls)
{
        u16 *banks = get_bank_array(cls);
        for (usize i = 0; i < NUM_STATUS_BANKS; ++i) {
                banks[i] = 0;
        }
}

void
status_snapshot(enum status_class cls, u16 *dst, usize len)
{
        const u16 *banks = get_bank_array(cls);
        for (usize i = 0; i < len && i < NUM_STATUS_BANKS; ++i) {
                dst[i] = banks[i];
        }
}
