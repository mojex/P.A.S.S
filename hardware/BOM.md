# Bill of Materials (BOM)

## Overview

This project is based on two microcontrollers:

- Raspberry Pi Pico (User Interface & Storage)
- Arduino ProMicro (USB HID Engine)

The Pico is responsible for reading payloads from the microSD card,
displaying the menu on the OLED, and sending commands to the ProMicro.

The ProMicro acts as a USB HID keyboard and executes the selected payload.

---

## Required Components

| Qty | Component | Notes |
|----:|-----------|-------|
| 1 | Raspberry Pi Pico | User Interface Controller |
| 1 | Arduino ProMicro | USB HID Keyboard |
| 1 | SSD1306 OLED Display (128×64, I2C) | Menu Display |
| 1 | MicroSD Card Module (SPI) | Payload Storage |
| 1 | microSD Card | FAT32 formatted |
| 3 | Momentary Push Buttons | UP / DOWN / SELECT |
| 1 | USB Cable (ProMicro) | Connected to Target Host |
| 1 | USB Cable (Pico) | Power / Programming |
| 6 | Female-to-Female Jumper Wires | UART & Power |
| several | Dupont Wires | General wiring |

---

## Optional Components

| Qty | Component | Purpose |
|----:|-----------|---------|
| 1 | 3D Printed Enclosure | Portable device |
| 1 | Power Switch | Portable operation |
| 1 | Li-Ion Battery | Standalone power |
| 1 | TP4056 Charger Module | Battery charging |
| 1 | Status LED | Execution indicator |
| 1 | Buzzer | Audible feedback |

---

## Software Dependencies

### Pico

- SD
- SPI
- Wire
- Adafruit GFX
- Adafruit SSD1306

### ProMicro

- HID-Project Library

---

## Tested Hardware

- Raspberry Pi Pico
- Arduino ProMicro
- SSD1306 OLED (128×64 I2C)
- Generic SPI microSD Module

---

## Target Devices

The project has been tested for automating repetitive keyboard input on systems that accept USB HID keyboards, including BIOS/UEFI environments.

Example use cases include:

- HPE Gen8 Servers
- HPE Gen9 Servers
- Windows
- Linux
- UEFI Shell
