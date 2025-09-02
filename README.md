
# STM32 NUCLEO-F429ZI RTOS Temperature & Humidity OLED Project

This project demonstrates how to use the STM32 NUCLEO-F429ZI board with a DHT11 temperature & humidity sensor and a 128x64 OLED (I2C, u8g2 driver), leveraging CMSIS-RTOS v2 (FreeRTOS) for professional multitasking display. The codebase is modular.

## Features
- **Hardware**: STM32 NUCLEO-F429ZI, DHT11 sensor, OLED (I2C, PB8/PB9)
- **Software**: STM32 HAL, FreeRTOS (CMSIS-RTOS v2), u8g2 graphics library
- **Functionality**:
  - OLED displays real-time temperature & humidity (Temp&Humi Display) data
  - Project name (Temp&Humi Display Display) shown at the bottom of the OLED
  - Temp&Humi Display data is periodically acquired by an RTOS task and sent via queue
  - Professional Doxygen comments, clear structure, easy to maintain and extend

## Directory Structure
```
NUCLEO-F429ZI_OLED_DHT11_RTOS/
├── Core/
│   ├── Inc/         # Header files (main.h, dht11_rtos_task.h, oled_rtos_task.h ...)
│   └── Src/         # Source files (main.c, dht11_rtos_task.c, oled_rtos_task.c ...)
├── Hardware/
│   ├── dht11/       # DHT11 driver
│   ├── oled/        # OLED driver
│   └── u8g2/        # u8g2 graphics library
├── Drivers/         # HAL, CMSIS, etc.
├── MDK-ARM/         # Keil project files
├── Middlewares/     # Third-party middleware (e.g., FreeRTOS)
├── README.md        # This documentation
└── LICENSE          # License file
```

## Quick Start
1. **Hardware Connection**:
   - OLED SCL → PB8, SDA → PB9, VCC → 5V, GND → GND
   - DHT11 data pin to designated GPIO, per schematic
   - 4.7kΩ pull-up resistors required on SCL/SDA
   - (Display mode switching logic can be extended in software)
2. **Development Environment**:
   - STM32CubeIDE or Keil uVision
   - Use STM32CubeMX to verify I2C1/FreeRTOS configuration
3. **Build & Flash**:
   - Import the project into your IDE, build, and flash to NUCLEO-F429ZI
4. **Display Verification**:
   - Default shows Temp&Humi Display data on OLED

## Main Code Structure
- `dht11_rtos_task.c/h`: DHT11 acquisition task, data queue
- `oled_rtos_task.c/h`: OLED display task, project name display
- `oled_driver.c/h`: OLED initialization and u8g2 interface
- `Hardware/dht11/`: DHT11 low-level driver
- `Hardware/oled/`: OLED low-level driver
- `Hardware/u8g2/`: u8g2 graphics library

## Advanced Features
- **Doxygen Documentation**: All core code is documented with professional English Doxygen comments
// ...existing code...
- **Error Handling**: UART3 outputs error messages; queue/task creation failure triggers Error_Handler

## License
This project is licensed under the MIT License. See LICENSE for details.
The u8g2 graphics library is licensed under Apache 2.0; please retain its LICENSE file.

## Acknowledgments
- [u8g2 Library](https://github.com/olikraus/u8g2)
- [STM32CubeMX](https://www.st.com/en/development-tools/stm32cubemx.html)
- [FreeRTOS](https://www.freertos.org)
