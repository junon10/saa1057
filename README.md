# SAA1057 PLL Controller Arduino Library

## Description

A library for Arduino that provides functions to configure and operate the SAA1057 PLL integrated circuit. This library enables control over various aspects of the PLL's operation.

## Features

- **Arduino Compatibility**: Works with most Arduino boards.
- **PLL Control Functions**: Functions to set frequency shifts, data configuration, and more.
- **Configuration**: Easy-to-use API for setting up pins and operating modes.
- **DIP Switch Integration**: Support for configuring pins via DIP switches.

## Requirements

- **Arduino IDE**: To install the library.
- **Hardware**: SAA1057 PLL circuit integrated and discrete components.
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

// Declare WordB register
saa1057_wordB WordB;

// Declare an instance of the SAA1057 PLL
SAA1057 pll;

void setup() {

  // Configure the PLL pins
  pll.begin(clock_pin, data_pin, dlen_pin);

  // DIP Switch configuration example (b7 to b0)
  pll.setDipSwitchPins(7, 6, 5, 4, 3, 2, 1, 0);

  WordB.refined.ADDR = ADDR_WORDB;
  WordB.refined.FM = MODE_FM;
  WordB.refined.REF = REF_1KHZ;
  WordB.refined.CP = CP_0_07MA;
  WordB.refined.SB2 = SB2_ON;
  WordB.refined.SLA = SLA_ASYNC;
  WordB.refined.PDM = PDM_AUTO;
  WordB.refined.BRM = BRM_ECONOMY;
  WordB.refined.T = T_LOCK_DET;

  pll.set(WordB.raw);
  
  pll.setFrequency(108.0, CP_0_07MA); // Frequency in MHz, Lock Filter Speed (current)
  // or
  pll.setFrequencyFromDipSwitch(CP_0_07MA);

  pll.commitConfig();
}

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

- **v1.0.0.6 (2024/06/15)**: Initial commit.
- **v1.0.0.7 (2025/01/09)**: Frequency configuration By DipSwitch integrated.
- **v1.0.0.8 (2025/01/13)**: Fixed bug in DipSwitch decoding.
- **v1.0.0.9 (2026/02/14)**: Small refactor.
- **v1.0.0.10 (2026/03/14)**: Refactor.
- **v1.0.0.11 (2026/03/15)**: Ref frequency const bug fix.
- **v1.0.0.12 (2026/03/15)**: Another example added.

## License

- GPLv3


