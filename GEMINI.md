Embedded Firmware Project Context
# STM32C031C6 Peripheral Driver Development | MISRA-C:2012 | Bare-Metal

---

## Project Identity

- **MCU:** STM32C031C6 (ARM Cortex-M0+, 48 MHz max, 32KB Flash, 12KB RAM, no FPU)
- **Interface:** SPI1, GPIO, RCC
- **Toolchain:** arm-none-eabi-gcc, Make/CMake
- **Test Framework:** Unity + CMock, runner: Ceedling
- **Static Analysis:** Cppcheck with MISRA addon

## Project Structure
project/
├── GEMINI.md
├── .gemini/
│ └── commands/
├── drivers/
│ ├── rcc/
│ ├── gpio/
│ ├── spi/
│ └── uart/
├── app/
│ └── main.c
├── tests/
│ ├── test_spi_driver.c
│ └── test_gpio_driver.c
└── project.yml

---

## MANDATORY CODING STANDARD — MISRA-C:2012

Apply ALL rules to EVERY file generated or modified.
Never violate without a written deviation justification in a comment.

### Mandatory Rules (zero exceptions)
- **Dir 1.1** — All compiler warnings treated as errors
- **Dir 4.1** — Run-time failures shall be minimized
- **Dir 4.7** — If a function returns error info, it shall be tested
- **Rule 1.3** — No undefined or critical unspecified behaviour
- **Rule 2.1** — No unreachable code
- **Rule 2.2** — No dead code
- **Rule 7.4** — String literals shall be pointer-to-const
- **Rule 8.4** — Compatible declaration required for every function with external linkage
- **Rule 11.3** — No cast from pointer to object type to different pointer to object type
- **Rule 11.6** — No cast from pointer to void to arithmetic type
- **Rule 13.6** — Sizeof shall not have operands with side effects
- **Rule 14.3** — Controlling expressions shall not be invariant
- **Rule 17.3** — No implicit function declaration

### Required Rules
- **Rule 5.1** — External identifiers distinct in first 31 chars
- **Rule 5.3** — No identifier hiding
- **Rule 8.2** — Function types in prototype form
- **Rule 8.5** — External object/function declared once in one header
- **Rule 8.7** — Functions/objects not needed outside TU shall use static
- **Rule 8.8** — static on all declarations of internal linkage
- **Rule 10.1** — Operands shall not be of inappropriate essential type
- **Rule 10.3** — Assigned value shall not have wider essential type
- **Rule 10.4** — Both operands of arithmetic shall have same essential type
- **Rule 14.4** — Controlling expression shall be essentially Boolean
- **Rule 15.5** — Single point of exit per function
- **Rule 15.7** — All if-else-if chains terminated with final else
- **Rule 16.4** — Every switch shall have a default clause
- **Rule 17.7** — Return value of non-void function shall be used
- **Rule 20.5** — #undef shall not be used
- **Rule 21.3** — No malloc, calloc, free, realloc
- **Rule 21.6** — No stdio.h in production code

### Advisory Rules
- **Rule 15.4** — Only one break per loop
- **Rule 15.6** — Loop body shall always be a compound statement
- **Rule 17.5** — Array parameter shall have static extent declared

---

## EMBEDDED C BEST PRACTICES

### Types
- ALWAYS use: uint8_t, int8_t, uint16_t, int16_t, uint32_t, int32_t
- NEVER use: plain int, char, long, short for hardware variables
- Use bool (stdbool.h) for Boolean, never int as Boolean

### Memory
- NO dynamic allocation — no malloc, calloc, realloc, free
- NO recursion
- static for all file-scope variables and functions
- const for all read-only data and configuration tables
- volatile for hardware registers, ISR-shared variables, DMA buffers

### Functions
- Validate all pointer arguments for NULL before dereferencing
- Check every non-void return value
- Check every Bosch API call against BME280_OK
- Single responsibility per function
- Max cyclomatic complexity: 10
- Max function length: 50 lines

### Naming Conventions
- Functions:  module_action_subject()    e.g. spi_transmit_byte()
- Constants:  SCREAMING_SNAKE_CASE       e.g. SPI1_BASE_ADDR
- Types:      module_type_t              e.g. spi_handle_t
- Files:      module_submodule.c/.h      e.g. stm32c031_spi.c

### No Magic Numbers
Every numeric literal must be a named macro:
/* WRONG  */ reg |= (1U << 6U);
/* RIGHT  */ reg |= (1U << SPI_CR1_SPE_BIT);

## TESTING REQUIREMENTS

### Framework
- Unity for assertions
- CMock for hardware mocking
- Ceedling as build and runner

### Coverage Requirements
- Every public function: minimum 1 happy-path test
- Every pointer parameter: 1 NULL-input test
- Every error return path: 1 negative test
- Every boundary condition: 1 boundary test
- Target: 90%+ line coverage

### Test Naming
test_<function_name>_<scenario>()
  e.g. test_bme280_port_init_returns_ok_on_success()
  e.g. test_spi_transmit_returns_fail_on_busy()

### Test Structure — AAA Pattern
void test_example(void)
{
    /* Arrange */
    /* Act     */
    /* Assert  */
}

### Mock Rules
- NEVER call real hardware in tests
- Mock all SPI, GPIO, RCC, UART functions via CMock
- Use _ExpectAndReturn() for return-value functions
- Use _Expect() for void functions
- Use _IgnoreAndReturn() only when call count is not under test

---

## RESPONSE FORMAT RULES

When generating code:
1. Header file (.h) before source file (.c)
2. File header block: filename, author placeholder, date, brief, MISRA note
3. #ifndef include guards in every header
4. Includes grouped: system → project → local
5. Doxygen comment block before every function
6. List any MISRA deviations with justification after the code

When generating test files:
1. Include setUp() and tearDown() (even if empty)
2. List all test names at top as comment index
3. End with main() calling UnityMain() if not using Ceedling runner

---

## ERROR CODE CONVENTION

typedef enum
{
    DRIVER_OK          =  0,
    DRIVER_ERR_NULL    = -1,
    DRIVER_ERR_TIMEOUT = -2,
    DRIVER_ERR_BUSY    = -3,
    DRIVER_ERR_FAULT   = -4
} driver_status_t;

---

## WHAT NOT TO DO

- No printf or stdio.h in production (drivers/, port/, app/, sensor/)
- No stdlib.h memory functions
- No HAL functions (no HAL_SPI_Transmit, no MX_ functions)
- No recursion
- No TODO comments — implement fully or mark explicit stub
- No // comments — use only /* */ block comments (MISRA Rule 3.1)
