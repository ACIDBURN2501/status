#include "../include/status.h"

#define NUM_STATUS_BANKS         (4u)
#define STATUS_ENCODE(bank, bit) (((bank) << 8) | (bit))
#define STATUS_BANK(id)          (((id) >> 8) & 0xFF)
#define STATUS_BIT(id)           ((id) & 0xFF)

static uint16_t fault_banks[NUM_STATUS_BANKS];
static uint16_t warning_banks[NUM_STATUS_BANKS];

void
status_init(void)
{
        for (size_t i = 0; i < NUM_STATUS_BANKS; ++i) {
                fault_banks[i] = 0;
                warning_banks[i] = 0;
        }
}

static inline uint16_t *
get_bank_array(enum status_class cls)
{
        return (cls == STATUS_CLASS_FAULT) ? fault_banks : warning_banks;
}

void
status_set(u16 id)
{
        uint16_t bank = STATUS_BANK(id);
        uint16_t bit = STATUS_BIT(id);
        if (bank < NUM_STATUS_BANKS) {
                fault_banks[bank] |= (1U << bit); // Default to fault class
        }
}

void
status_clear(u16 id)
{
        uint16_t bank = STATUS_BANK(id);
        uint16_t bit = STATUS_BIT(id);
        if (bank < NUM_STATUS_BANKS) {
                fault_banks[bank] &= ~(1U << bit);
        }
}

void
status_toggle(u16 id)
{
        uint16_t bank = STATUS_BANK(id);
        uint16_t bit = STATUS_BIT(id);
        if (bank < NUM_STATUS_BANKS) {
                fault_banks[bank] ^= (1U << bit);
        }
}

bool
status_is_set(u16 id)
{
        uint16_t bank = STATUS_BANK(id);
        uint16_t bit = STATUS_BIT(id);
        if (bank >= NUM_STATUS_BANKS) {
                return false;
        }
        return (fault_banks[bank] & (1U << bit)) != 0;
}

bool
status_any(enum status_class cls)
{
        const uint16_t *banks = get_bank_array(cls);
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
