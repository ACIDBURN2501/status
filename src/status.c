/*
 * @copyright MIT
 *
 * @file: status.c
 *
 * @brief Implementation of core status tracking functionality.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

#include "status.h"

/* ---------------- Configuration ------------------------------------------- */

_Static_assert(NUM_STATUS_BANKS <= 4096u, "Too many banks for 12-bit field");
_Static_assert((0u <= 15u),
               "bit field is 4 bits (0..15)"); /* documentation hint */
_Static_assert(NUM_STATUS_BITS <= 16U,
               "NUM_STATUS_BIT must fit within uint16_t shift limit");

#define UNSET_ID 0xFFFFu

static volatile uint16_t fault_banks[NUM_STATUS_BANKS];
static volatile uint16_t warning_banks[NUM_STATUS_BANKS];
static volatile uint16_t info_banks[NUM_STATUS_BANKS];

static uint16_t last_fault_id = UNSET_ID;
static uint16_t last_warning_id = UNSET_ID;
static uint16_t last_info_id = UNSET_ID;

static status_err_cb_t err_cb = NULL;

/* ---------------- Helpers ------------------------------------------------- */

/**
 * @brief Computes the minimum of two values.
 *
 * @details
 *    This inline function takes two arguments and returns the
 *    smaller value.
 *
 * @param a         The second value to compare.
 * @param b         The second value to compare.
 *
 * @return          The minimum of `a` and `b`.
 */
static inline size_t
min_u16(size_t a, size_t b)
{
        return (a < b) ? a : b;
}

/* Writeable view */
static inline volatile uint16_t *
get_banks_mut(enum status_class cls)
{
        switch (cls) {
        case STATUS_CLASS_FAULT: return fault_banks;
        case STATUS_CLASS_WARNING: return warning_banks;
        case STATUS_CLASS_INFO: return info_banks;
        default: return NULL;
        }
}

/* Read-only view */
static inline const volatile uint16_t *
get_banks_ro(enum status_class cls)
{
        return get_banks_mut(cls); /* Upcast to const */
}

static void
invoke_err_cb(status_err_t err, uint16_t id)
{
        if (err_cb != NULL) {
                err_cb(err, id);
        }
}

static void
set_bit(uint16_t id, enum status_class cls)
{
        uint16_t bank = status_bank(id);
        uint16_t bit = status_bit(id);

        if (bank >= NUM_STATUS_BANKS) {
                invoke_err_cb(STATUS_ERR_INVALID_BANK, id);
                return;
        }

        if (bit >= NUM_STATUS_BITS) {
                invoke_err_cb(STATUS_ERR_INVALID_BIT, id);
                return;
        }

        volatile uint16_t *b = get_banks_mut(cls);
        if (b == NULL) {
                invoke_err_cb(STATUS_ERR_INVALID_ID, id);
                return;
        }

        STATUS_ENTER_CRITICAL();
        b[bank] |= ((uint16_t)1u << bit);

        switch (cls) {
        case STATUS_CLASS_FAULT: last_fault_id = id; break;
        case STATUS_CLASS_WARNING: last_warning_id = id; break;
        case STATUS_CLASS_INFO: last_info_id = id; break;
        }
        STATUS_EXIT_CRITICAL();
}

static void
clear_bit(uint16_t id, enum status_class cls)
{
        uint16_t bank = status_bank(id);
        uint16_t bit = status_bit(id);

        if (bank >= NUM_STATUS_BANKS) {
                invoke_err_cb(STATUS_ERR_INVALID_BANK, id);
                return;
        }

        if (bit >= NUM_STATUS_BITS) {
                invoke_err_cb(STATUS_ERR_INVALID_BIT, id);
                return;
        }

        volatile uint16_t *b = get_banks_mut(cls);
        if (b == NULL) {
                invoke_err_cb(STATUS_ERR_INVALID_ID, id);
                return;
        }

        STATUS_ENTER_CRITICAL();
        b[bank] &= ~((uint16_t)1u << bit);
        STATUS_EXIT_CRITICAL();
}

static void
toggle_bit(uint16_t id, enum status_class cls)
{
        uint16_t bank = status_bank(id);
        uint16_t bit = status_bit(id);

        if (bank >= NUM_STATUS_BANKS) {
                invoke_err_cb(STATUS_ERR_INVALID_BANK, id);
                return;
        }

        if (bit >= NUM_STATUS_BITS) {
                invoke_err_cb(STATUS_ERR_INVALID_BIT, id);
                return;
        }

        volatile uint16_t *b = get_banks_mut(cls);
        if (b == NULL) {
                invoke_err_cb(STATUS_ERR_INVALID_ID, id);
                return;
        }

        STATUS_ENTER_CRITICAL();
        b[bank] ^= ((uint16_t)1u << bit);
        STATUS_EXIT_CRITICAL();
}

static bool
is_bit_set(uint16_t id, enum status_class cls)
{
        uint16_t bank = status_bank(id);
        uint16_t bit = status_bit(id);

        if (bank >= NUM_STATUS_BANKS) {
                invoke_err_cb(STATUS_ERR_INVALID_BANK, id);
                return false;
        }

        if (bit >= NUM_STATUS_BITS) {
                invoke_err_cb(STATUS_ERR_INVALID_BIT, id);
                return false;
        }

        const volatile uint16_t *b = get_banks_ro(cls);
        if (b == NULL) {
                invoke_err_cb(STATUS_ERR_INVALID_ID, id);
                return false;
        }

        return (b[bank] & ((uint16_t)1u << bit)) != 0;
}

/* ---------------  Public Interface ---------------------------------------- */

void
status_init(void)
{
        STATUS_ENTER_CRITICAL();
        for (size_t i = 0; i < NUM_STATUS_BANKS; ++i) {
                fault_banks[i] = 0U;
        }
        for (size_t i = 0; i < NUM_STATUS_BANKS; ++i) {
                warning_banks[i] = 0U;
        }
        for (size_t i = 0; i < NUM_STATUS_BANKS; ++i) {
                info_banks[i] = 0U;
        }
        last_fault_id = UNSET_ID;
        last_warning_id = UNSET_ID;
        last_info_id = UNSET_ID;
        STATUS_EXIT_CRITICAL();
}

void
status_set_err_callback(status_err_cb_t cb)
{
        err_cb = cb;
}

void
status_set_warning(uint16_t id)
{
        set_bit(id, STATUS_CLASS_WARNING);
}

void
status_set_fault(uint16_t id)
{
        set_bit(id, STATUS_CLASS_FAULT);
}

void
status_set_info(uint16_t id)
{
        set_bit(id, STATUS_CLASS_INFO);
}

void
status_clear_warning(uint16_t id)
{
        clear_bit(id, STATUS_CLASS_WARNING);
}

void
status_clear_fault(uint16_t id)
{
        clear_bit(id, STATUS_CLASS_FAULT);
}

void
status_clear_info(uint16_t id)
{
        clear_bit(id, STATUS_CLASS_INFO);
}

void
status_toggle_warning(uint16_t id)
{
        toggle_bit(id, STATUS_CLASS_WARNING);
}

void
status_toggle_fault(uint16_t id)
{
        toggle_bit(id, STATUS_CLASS_FAULT);
}

void
status_toggle_info(uint16_t id)
{
        toggle_bit(id, STATUS_CLASS_INFO);
}

bool
status_is_warning_set(uint16_t id)
{
        return is_bit_set(id, STATUS_CLASS_WARNING);
}

bool
status_is_fault_set(uint16_t id)
{
        return is_bit_set(id, STATUS_CLASS_FAULT);
}

bool
status_is_info_set(uint16_t id)
{
        return is_bit_set(id, STATUS_CLASS_INFO);
}

bool
status_any(enum status_class cls)
{
        const volatile uint16_t *b = get_banks_ro(cls);

        if (b == NULL) {
                invoke_err_cb(STATUS_ERR_NULL_PTR, 0xFFFF);
                return false;
        }

        for (size_t i = 0; i < NUM_STATUS_BANKS; ++i) {
                if (b[i]) {
                        return true;
                }
        }
        return false;
}

void
status_clear_all(enum status_class cls)
{
        volatile uint16_t *b = get_banks_mut(cls);

        if (b == NULL) {
                invoke_err_cb(STATUS_ERR_NULL_PTR, 0xFFFF);
                return;
        }

        STATUS_ENTER_CRITICAL();
        for (size_t i = 0; i < NUM_STATUS_BANKS; ++i) {
                b[i] = 0;
        }
        STATUS_EXIT_CRITICAL();
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
        const volatile uint16_t *src = get_banks_ro(cls);
        if ((src == NULL) || (dst == NULL) || (len == 0u)) {
                invoke_err_cb(STATUS_ERR_NULL_PTR, 0xFFFF);
                return;
        }

        const size_t copy_len = min_u16(len, NUM_STATUS_BANKS);

        STATUS_ENTER_CRITICAL();
        for (size_t i = 0; i < copy_len; ++i) {
                dst[i] = src[i];
        }
        STATUS_EXIT_CRITICAL();
}
