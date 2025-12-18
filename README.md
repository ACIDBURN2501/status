# Status Library

A lightweight C11 status register system for embedded systems.
Tracks faults, warnings, and info bits using banked bitfields encoded as compact 16-bit status IDs.

- **Banked Bitfields**: Efficient storage using arrays of `uint16_t`.
- **Compact IDs**: 16-bit IDs encode both bank and bit index.
- **Configurable**: Adjustable bank count and critical section macros.
- **Safety**: Error callbacks and runtime bounds checking.

## Layout

- `src/status.h` – Public API and configuration macros.
- `src/status_version.h.in` – Template for generating the version header.
- `src/status.c` – Implementation of the status module.

## Versioning

The project version is managed in `meson.build`. During the build process, a `status_version.h` file is generated in the build directory.

You can check the version in your code:

```c
#include "status.h"

#if STATUS_VERSION_MAJOR == 0
    // handle legacy behavior
#endif
```

## Usage

### 1. Define Status IDs
Create a `status_ids.h` file for your application using the `STATUS_ENCODE` macro.

```c
/* status_ids.h */
#include "status.h"

// Bank 0: Power Faults
#define STATUS_ID_FAULT_OVERCURRENT    STATUS_ENCODE(0u, 0u)
#define STATUS_ID_FAULT_OVERVOLTAGE    STATUS_ENCODE(0u, 1u)

// Bank 1: Thermal Warnings
#define STATUS_ID_WARN_HIGH_TEMP       STATUS_ENCODE(1u, 0u)
```

### 2. Integrate
Initialize the module and use the IDs to set/clear status bits.

```c
#include "status_ids.h"

void app_init(void) {
    status_init();
    
    // Optional: Register error callback
    status_set_err_callback(my_error_handler);
}

void check_power(void) {
    if (voltage > MAX_VOLTAGE) {
        status_set_fault(STATUS_ID_FAULT_OVERVOLTAGE);
    }
}
```

## Configuration

The following macros can be overridden (e.g., via compiler flags or a config header included before `status.h`):

- `NUM_STATUS_BANKS` (default `12`)
  - The number of `uint16_t` banks for each status class.

- `STATUS_ENTER_CRITICAL()` / `STATUS_EXIT_CRITICAL()` (default no-op)
  - **Required for thread safety.**
  - Define these macros to disable/enable interrupts if status bits are modified from both ISRs and the main loop.

## Concurrency

- **Atomic Access**: The library uses read-modify-write operations on volatile arrays.
- **Critical Sections**: You **must** provide `STATUS_ENTER_CRITICAL` and `STATUS_EXIT_CRITICAL` implementations if there is preemption between different execution contexts (e.g., ISR vs Thread) accessing the status module.

## Build & Test

This project uses [Meson](https://mesonbuild.com).

```bash
meson setup build
meson compile -C build
meson test -C build

# run a single test
meson test -C build status:test_status --print-errorlogs
```

## Formatting

```bash
clang-format -i src/* tests/*
```
