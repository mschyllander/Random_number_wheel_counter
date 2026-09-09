# Random Number Wheel Counter

ESP32-C3 Zephyr demo for an ST7735 SPI TFT display.

The display shows a Matrix-style falling-number background and six large,
transparent rolling number wheels. Each wheel spins and stops independently
on a new random value.

## Build

```powershell
west build -p always -b esp32c3_devkitm/esp32c3
west flash
```

The pin assignments are in `boards/esp32c3_devkitm.overlay` and the ST7735
control pins are documented in `src/st7735.c`.
