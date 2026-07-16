# STM32C031C6 Bare-Metal Driver Development

## Overview

This project focuses on the development of robust, MISRA-C:2012 compliant bare-metal drivers for the **STM32C031C6** microcontroller (ARM Cortex-M0+).

This repository prioritizes the creation of clean, maintainable driver layers, adhering to strict embedded best practices, without relying on vendor-provided HAL (Hardware Abstraction Layer) libraries.

## Architecture & Focus

The project is structured to separate hardware-specific driver implementation from application logic:

- **Drivers:** Low-level, register-access drivers for STM32 peripherals (RCC, GPIO, SPI).
- **App:** Minimal application demonstration and testing.

## Standards & Quality

- **Language Standard:** C (targeting MISRA-C:2012 compliance).
- **MCU:** STM32C031C6 (ARM Cortex-M0+).
- **Static Analysis:** Cppcheck with MISRA-C:2012 addon.
- **Testing:** Unit testing with Unity + CMock, managed via Ceedling.

## Project Structure

```text
/
├── drivers/      # Low-level peripheral drivers
├── app/          # Application-level entry points
└── tests/        # Unit tests
```

## Building

This project utilizes a `Makefile` / `CMake` based build system compatible with the `arm-none-eabi-gcc` toolchain.

## Testing

Testing is a first-class citizen. Every driver function is expected to have associated unit tests with high line coverage.

*Run tests:*
```bash
ceedling test:all
```

## License

This project includes components derived from various sources. Please check individual directories for license information.
