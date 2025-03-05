# SAA1057 PLL Controller Library

## Description

A library for Arduino that provides functions to configure and operate the SAA1057 PLL integrated circuit. This library enables control over various aspects of the PLL's operation.

## Features

- **Arduino Compatibility**: Works with most Arduino boards.
- **PLL Control Functions**: Functions to set frequency shifts, data configuration, and more.
- **Configuration**: Easy-to-use API for setting up pins and operating modes.
- **DIP Switch Integration**: Support for configuring pins via DIP switches.

## Requirements

- **Arduino IDE**: To install the library.
- **Hardware**: SAA1057 PLL circuit integrated into your project.
- **Board Compatibility**: Most Arduino-compatible boards supported.

## Installation

To install the library:

1. Download the repository from GitHub:
   [https://github.com/junon10/saa1057](https://github.com/junon10/saa1057)

2. In Arduino IDE, go to Sketch > Include Library > Add ZIP File...

3. Select the downloaded zip file.

## Usage

```c++
#include <SAA1057.h>

// Declare an instance of the SAA1057 PLL
SAA1057 pll;

// Configure the PLL pins
pll.begin(clock_pin, data_pin, dlen_pin);

// DIP Switch configuration example (b7 to b0)
pll.setDipSwPinout(b7, b6, b5, b4, b3, b2, b1, b0);

// Force data bits to 1 and send to WordB
pll.set(data, shiftLeft);

// Clear data bits (force to 0)
pll.clear(data, shiftLeft);

// Example of setting a specific configuration
pll.setDefaultConfig();

// Configure the operating frequency and lock speed
pll.setFrequency(108.0, CP_007); // Frequency in MHz, Lock Filter Speed (current)
// or
pll.setFrequencyByDipSw(CP_007);

// Commit changes
pll.commitConfig();
```

## Limitations

- For detailed technical specifications, refer to the SAA1057 datasheet.
- Function parameters and return values may vary based on hardware configuration.

## Author

- **Junon M.**  
  Contact: [junon10@tutamail.com](mailto:junon10@tutamail.com)

## Contributing

Contributions are welcome! Please fork the repository and send a pull request.

## Repository

- [https://github.com/junon10/saa1057](https://github.com/junon10/saa1057)

## References

- SAA1057 Datasheet
- Arduino Documentation

## Changelog

- **v1.0.6 (2024/06/15)**: Initial commit.
- **v1.0.7 (2025/01/09)**: Frequency configuration By DipSwitch integrated.
- **v1.0.8 (2025/01/13)**: Fixed bug in DipSwitch decoding.

## License

- GPLv3


