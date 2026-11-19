# Factory Firmware

[English](README.md) | [简体中文](README_ZH.md)

This directory stores the tested factory image for the ESP32-C6-LCD-1.28 development board.

| File | Purpose |
| --- | --- |
| [ESP32-C6-LCD-1.28_FactoryOnly_20260813.bin](ESP32-C6-LCD-1.28_FactoryOnly_20260813.bin) | Factory recovery firmware for the ESP32-C6-LCD-1.28 board, dated 20260813. Flash from `0x0`. |

Before flashing:

- Back up any device-specific data that must be preserved.
- Confirm the board model and hardware revision.
- Erase the target device before writing the merged image.
- Follow the flashing procedure in the product documentation.

SHA-256: `59788AB31632396F27CFF960329F63E75192B39863F99CD34400D55CF609ACCC`

This file is a single merged BIN for factory recovery.
