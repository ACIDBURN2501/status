/*
 * @copyright MIT
 *
 * @file: status.h
 *
 * @brief Implementation of core status tracking functionality.
 */
#include "../include/status.h"

#define NUM_STATUS_BANKS (4u)

static u16 fault_banks[NUM_STATUS_BANKS];
static u16 warning_banks[NUM_STATUS_BANKS];

/**
 * @def STATUS_BANK
 * @brief Extracts the bank number from an encoded status ID.
 *
 * @param id        A status ID encoded using `STATUS_ENCODE()`.
 *
 * @return          The bank index (0-based).
 */
#define STATUS_BANK(id) (((id) >> 4) & 0x0Fu)

/**
 * @def STATUS_BIT
 * @brief Extracts the bit index from an encoded status ID.
 *
 * @param id        A status ID encoded using `STATUS_ENCODE()`.
 *
 * @return          The bit index (0–15) within the bank.
 */
#define STATUS_BIT(id)  ((id) & 0x0Fu)

/* Catch any config errors */
_Static_assert(STATUS_BIT(0xFFFFu) <= 15u,
               "STATUS_BIT macro must produce values in [0, 15]");

void
status_init(void)
{
        for (usize i = 0; i < NUM_STATUS_BANKS; ++i) {
                fault_banks[i] = 0;
                warning_banks[i] = 0;
        }
}

static inline u16 *
get_bank_array(enum status_class cls)
{
        return (cls == STATUS_CLASS_FAULT) ? fault_banks : warning_banks;
}

void
status_set(u16 id)
{
        u16 bank = STATUS_BANK(id);
        u16 bit = STATUS_BIT(id);
        if (bank < NUM_STATUS_BANKS) {
                fault_banks[bank] |= (1U << bit); // Default to fault class
        }
}

void
status_clear(u16 id)
{
        u16 bank = STATUS_BANK(id);
        u16 bit = STATUS_BIT(id);
        if (bank < NUM_STATUS_BANKS) {
                fault_banks[bank] &= ~(1U << bit);
        }
}

void
status_toggle(u16 id)
{
        u16 bank = STATUS_BANK(id);
        u16 bit = STATUS_BIT(id);
        if (bank < NUM_STATUS_BANKS) {
                fault_banks[bank] ^= (1U << bit);
        }
}

bool
status_is_set(u16 id)
{
        u16 bank = STATUS_BANK(id);
        u16 bit = STATUS_BIT(id);
        if (bank >= NUM_STATUS_BANKS) {
                return false;
        }
        return (fault_banks[bank] & (1U << bit)) != 0;
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
