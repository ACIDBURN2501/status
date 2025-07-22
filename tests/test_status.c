#include <assert.h>
#include <stdlib.h>

#include "../examples/status_ids.h"
#include "../include/status.h"

void
setUp(void)
{
        status_init();
}

void
tearDown(void)
{
}

static void
test_fault_set_and_clear(void)
{
        setUp();

        status_set_fault(STATUS_ID_FAULT_OVERCURRENT);
        assert(status_is_fault_set(STATUS_ID_FAULT_OVERCURRENT) == true);

        status_clear_fault(STATUS_ID_FAULT_OVERCURRENT);
        assert(status_is_fault_set(STATUS_ID_FAULT_OVERCURRENT) == false);
}

static void
test_warn_set_and_clear(void)
{
        setUp();

        status_set_warning(STATUS_ID_WARN_TEMP_NEAR_LIMIT);
        assert(status_is_warning_set(STATUS_ID_WARN_TEMP_NEAR_LIMIT) == true);

        status_clear_warning(STATUS_ID_WARN_TEMP_NEAR_LIMIT);
        assert(status_is_warning_set(STATUS_ID_WARN_TEMP_NEAR_LIMIT) == false);
}

static void
test_info_set_and_clear(void)
{
        setUp();

        status_set_info(STATUS_ID_INFO_AC_LIVE);
        assert(status_is_info_set(STATUS_ID_INFO_AC_LIVE) == true);

        status_clear_info(STATUS_ID_INFO_AC_LIVE);
        assert(status_is_info_set(STATUS_ID_INFO_AC_LIVE) == false);
}

static void
test_toggle_fault(void)
{
        status_init();

        assert(status_is_fault_set(STATUS_ID_FAULT_OVERCURRENT) == false);
        status_toggle_fault(STATUS_ID_FAULT_OVERCURRENT);
        assert(status_is_fault_set(STATUS_ID_FAULT_OVERCURRENT) == true);
        status_toggle_fault(STATUS_ID_FAULT_OVERCURRENT);
        assert(status_is_fault_set(STATUS_ID_FAULT_OVERCURRENT) == false);
}

static void
test_toggle_warn(void)
{
        status_init();

        assert(status_is_warning_set(STATUS_ID_WARN_TEMP_NEAR_LIMIT) == false);
        status_toggle_warning(STATUS_ID_WARN_TEMP_NEAR_LIMIT);
        assert(status_is_warning_set(STATUS_ID_WARN_TEMP_NEAR_LIMIT) == true);
        status_toggle_warning(STATUS_ID_WARN_TEMP_NEAR_LIMIT);
        assert(status_is_warning_set(STATUS_ID_WARN_TEMP_NEAR_LIMIT) == false);
}

static void
test_toggle_info(void)
{
        setUp();

        assert(status_is_info_set(STATUS_ID_INFO_CAN_ACTIVE) == false);
        status_toggle_info(STATUS_ID_INFO_CAN_ACTIVE);
        assert(status_is_info_set(STATUS_ID_INFO_CAN_ACTIVE) == true);
        status_toggle_info(STATUS_ID_INFO_CAN_ACTIVE);
        assert(status_is_info_set(STATUS_ID_INFO_CAN_ACTIVE) == false);
}

static void
test_is_fault_set(void)
{
        status_init();

        status_set_fault(STATUS_ID_FAULT_OVERVOLTAGE);
        assert(status_is_fault_set(STATUS_ID_FAULT_OVERVOLTAGE) == true);
        assert(status_is_fault_set(STATUS_ID_FAULT_OVERCURRENT) == false);
}

static void
test_is_warn_set(void)
{
        status_init();

        status_set_warning(STATUS_ID_WARN_FAN_PERF_DROP);
        assert(status_is_warning_set(STATUS_ID_WARN_FAN_PERF_DROP) == true);
        assert(status_is_warning_set(STATUS_ID_WARN_CAN_LOAD_HIGH) == false);
}

static void
test_is_info_set(void)
{
        setUp();

        status_set_info(STATUS_ID_INFO_TEMP_CHANGING);
        assert(status_is_info_set(STATUS_ID_INFO_TEMP_CHANGING) == true);
        assert(status_is_info_set(STATUS_ID_INFO_AC_LIVE) == false);
}

static void
test_status_any(void)
{
        status_init();

        assert(status_any(STATUS_CLASS_WARNING) == false);

        status_set_fault(STATUS_ID_FAULT_UNDERVOLTAGE);
        assert(status_any(STATUS_CLASS_FAULT) == true);

        status_clear_fault(STATUS_ID_FAULT_UNDERVOLTAGE);
        assert(status_any(STATUS_CLASS_FAULT) == false);

        status_set_warning(STATUS_ID_WARN_TEMP_NEAR_LIMIT);
        assert(status_any(STATUS_CLASS_FAULT) == false);
}

static void
test_status_clear_all(void)
{
        status_init();

        status_set_fault(STATUS_ID_FAULT_OVERCURRENT);
        status_set_warning(STATUS_ID_WARN_TEMP_NEAR_LIMIT);
        assert(status_any(STATUS_CLASS_WARNING) == true);

        status_clear_all(STATUS_CLASS_FAULT);
        assert(status_any(STATUS_CLASS_FAULT) == false);
}
int
main(void)
{
        test_fault_set_and_clear();
        test_warn_set_and_clear();
        test_info_set_and_clear();
        test_toggle_warn();
        test_toggle_fault();
        test_toggle_info();
        test_is_warn_set();
        test_is_fault_set();
        test_is_info_set();
        test_status_any();
        test_status_clear_all();
        return EXIT_SUCCESS;
}
