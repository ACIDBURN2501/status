# Build & Test
- Build: `meson setup build && meson compile -C build`
- Run tests: `meson test -C build`
- Run single test: Modify `tests/test_status.c` main() to call only the desired test function, then rebuild & run.
- Lint: `clang-format -i src/* tests/*` (Config: .clang-format, 8-space indent, Linux style)

# Code Style
- **Standard**: C11 (`c_std=c11`).
- **Formatting**: 8 spaces indentation (no tabs). Function braces on new line, control flow braces on same line. Return type on separate line from function name.
- **Naming**: `snake_case` for functions/vars, `UPPER_CASE` for macros/enums. Internal helpers start with `_`.
- **Types**: Use `<stdint.h>` types (`uint16_t`, etc.) and `<stdbool.h>`.
- **Documentation**: Doxygen style (`@brief`, `@param`, `@return`) in headers.
- **Error Handling**: Defensive programming with early returns. Use `_Static_assert` for compile-time checks.
- **Imports**: Group standard lib includes, then local includes.
