# Pinout

## Raspberry Pi Pico

### OLED Display (I2C)

| Pico Pin | OLED |
|-----------|------|
| 3V3 | VCC |
| GND | GND |
| GP0 | SDA |
| GP1 | SCL |

---

### microSD Module (SPI)

| Pico Pin | SD Module |
|-----------|-----------|
| 3V3 | VCC |
| GND | GND |
| GP18 | SCK |
| GP19 | MOSI |
| GP16 | MISO |
| GP17 | CS |

---

### Navigation Buttons

| Pico Pin | Function |
|-----------|----------|
| GP2 | UP |
| GP3 | DOWN |
| GP4 | SELECT |

Buttons are configured using the internal pull-up resistors.

Logic:

Pressed = LOW

Released = HIGH

---

### UART Connection

| Pico Pin | ProMicro Pin |
|-----------|--------------|
| GP8 (TX) | RX1 (Pin 0) |
| GP9 (RX) | TX1 (Pin 1) |
| GND | GND |

UART is used to transfer payload commands from the Pico to the ProMicro.

---

# Arduino ProMicro

## USB

The ProMicro connects directly to the target host through USB.

It enumerates as a standard USB HID keyboard.

---

## UART

| ProMicro Pin | Pico |
|---------------|------|
| RX1 (0) | Pico TX |
| TX1 (1) | Pico RX |
| GND | GND |

---

## System Diagram

          +----------------------+
          | Raspberry Pi Pico    |
          |----------------------|
          | OLED Menu            |
          | Button Handler       |
          | SD Card Reader       |
          | Payload Selection    |
          +----------+-----------+
                     |
                 UART Serial
                     |
          +----------v-----------+
          | Arduino ProMicro     |
          |----------------------|
          | HID Keyboard Engine  |
          +----------+-----------+
                     |
                   USB HID
                     |
                Target Computer

---

## Data Flow

```
microSD
   │
   ▼
Pico
   │
OLED Menu
   │
User selects payload
   │
UART
   ▼
ProMicro
   │
USB HID
   ▼
Target Host
```

---

## Notes

- Both boards must share a common GND.
- The Pico only manages the user interface.
- The ProMicro is solely responsible for USB HID emulation.
- Payload files are stored on the microSD card.
- The project architecture separates UI logic from HID execution, improving responsiveness and maintainability.
