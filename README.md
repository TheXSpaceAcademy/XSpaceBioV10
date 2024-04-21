# XSpaceBioV10 Library

Welcome to the XSpaceBioV10 library, developed to support biomedical projects with enhanced capabilities for monitoring and analyzing physiological signals. This library is specially designed to work with sensors like the AD8232 for ECG (Electrocardiography) and the AFE4490 for PPG (Photoplethysmography), enabling advanced health monitoring in Arduino-based projects.

## Features

- Support for multiple AD8232 ECG sensors.
- Integration with the AFE4490 for PPG readings to measure blood oxygen saturation (SpO2).
- Efficient SPI communication for sensor data acquisition.
- Functions to analyze signal data, including finding maximum, minimum, and dominant frequency.

## Installation

### Arduino IDE

To install the XSpaceBioV10 library, download the ZIP file of the repository and follow these steps in the Arduino IDE:

1. Go to Sketch > Include Library > Add .ZIP Library...
2. Navigate to the downloaded file and select it.
3. The library will now be available in your Arduino IDE.

## Quick Start

Below is a simple example to get you started with the XSpaceBioV10 library by initializing an AD8232 sensor and reading values:

```cpp
#include <Arduino.h>
#include <XSpaceBioV10.h>

XSpaceBioV10Board XSBoard;

void setup() {
  // Initialize AD8232 Sensor 0
  XSBoard.AD8232_init(0, 33, 25, 14, 34); // Setup pins for SDN, LO+, LO-, and OUT
}

void loop() {
  double voltage = XSBoard.AD8232_GetVoltage(0); // Read differential voltage from sensor 0
  Serial.println(voltage);
  delay(10);
}

```

## Documentation

For detailed documentation, including setup instructions, API reference, and more examples, please visit our [GitHub repository](https://github.com/TheXSpaceAcademy/XSpaceBioV10).

## Contributing

Contributions to the XSpaceBioV10 library are welcome. If you have suggestions or enhancements, please submit them as issues or pull requests on GitHub.

## License

XSpaceBioV10 is licensed under the MIT License. See the [LICENSE](https://github.com/TheXSpaceAcademy/XSpaceBioV10/blob/main/LICENSE) file for more details.

## Acknowledgments

Thank you to everyone who has contributed to the development and improvement of this library. Your efforts continue to support and inspire innovation in biomedical engineering.
