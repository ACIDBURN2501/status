# Status Module for Embedded Systems

[![Run Unity Tests](https://github.com/ACIDBURN2501/status/actions/workflows/test.yml/badge.svg)](https://github.com/ACIDBURN2501/status/actions/workflows/test.yml)

This module provides a lightweight status register system for embedded systems.
It allows tracking of faults and warnings using banked bitfields, encoded as
compact 16-bit status IDs. This design minimizes runtime overhead and is
suitable for small targets such as microcontrollers.

The module uses separate internal banks for **faults** and **warnings**, and
provides explicit APIs for setting, clearing, querying, and toggling status bits.

⚠️ **Note:** Status IDs must be created using the `STATUS_ENCODE(bank, bit)`
macro, and the bank must be less than `NUM_STATUS_BANKS`, with bit values
ranging from 0–15. Runtime assertions are included to validate ID use.

## Features
- Independent tracking of faults and warnings
- Compact 16-bit encoded status IDs
- Runtime assertions for ID safety (bank/bit bounds)
- Explicit API separation for fault vs warning logic
- `status_any()` to check for any active condition
- `status_reset_all()` and `status_clear_all()` support

## Integration
```c
#include "status.h"
#include "status_ids.h" /* User generated */
```

Your project must define `status_ids.h` using the `STATUS_ENCODE(bank, bit)`
macro to create meaningful status IDs.

## Example `status_ids.h`
```c
#define STATUS_ID_FAULT_OVERCURRENT   STATUS_ENCODE(0U, 0U)
#define STATUS_ID_WARN_TEMP_HIGH      STATUS_ENCODE(1U, 0U)

status_set_fault(STATUS_ID_FAULT_OVERCURRENT);

if (status_is_fault_set(STATUS_ID_FAULT_OVERCURRENT)) {
    enter_fault_state();
}
```

An full example is provided by `examples/status_ids.h`.

## Building the Project with Meson

This project uses [Meson](https://mesonbuild.com) as the build system.

```bash
meson setup builddir
meson compile -C builddir
```

## Running Unit Tests with Meson

Ensure Unity is available via `subprojects/unity.wrap` (already configured).

```bash
meson test -C builddir --print-errorlogs
```
