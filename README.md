# PIC18F4620 Drivers Repository

This repository contains a collection of drivers and hardware abstraction layers (HAL) for the PIC18F4620 microcontroller. These drivers are designed to simplify the process of interfacing with various hardware components, making it easier to develop applications for the PIC18F4620 microcontroller.

## Table of Contents

- [Directories](#directories)
- [HAL (Hardware Abstraction Layer)](#hal-hardware-abstraction-layer)
  - [7_Seg](HAL/7_Seg)
  - [Button](HAL/Buttonn)
  - [Chr_LCD](HAL/Chr_LCD)
  - [Dc_Motor](HAL/Dc_Motor)
  - [Keypad](HAL/Keypad)
  - [LED](HAL/LED)
- [MCAL (Microcontroller Abstraction Layer)](#mcal-microcontroller-abstraction-layer)
  - [GPIO](MCAL/GPIO)
  - [interrupt](MCAL/interrupt)
  - [ADC](MCAL/ADC)
  - [CCP](MCAL/CCP)
  - [EEPROM](MCAL/EEPROM)
  - [I2C](MCAL/I2C)
  - [SPI](MCAL/SPI)
  - [USART](MCAL/USART)
  - [TIMER0](MCAL/TIMER0)
  - [TIMER1](MCAL/TIMER1)
  - [TIMER2](MCAL/TIMER2)
  - [TIMER3](MCAL/TIMER3)
  - [std_libraries.h and std_types.h](#std_librariesh-and-std_typesh)
- [Application](#application)
- [Usage](#usage)

## Directories

Here's an overview of the directories in this repository:

### HAL (Hardware Abstraction Layer)

The HAL directory contains drivers and abstractions for various hardware components. These drivers provide a high-level interface to interact with hardware, making it easier to develop applications on the PIC18F4620 microcontroller.

#### Modules

- **7_Seg**: This module provides drivers and functions to control 7-segment displays. You can easily display numbers or characters on 7-segment displays using these drivers.

- **Button**: The Button module includes drivers to handle button input. It allows you to detect button presses and releases, simplifying user input in your projects.

- **Chr_LCD**: This module is designed for character LCD displays. It provides functions to initialize the LCD, send commands and characters, and position the cursor.

- **Dc_Motor**: The Dc_Motor module contains drivers for DC motors. You can use these drivers to control the direction and speed of DC motors in your projects.

- **Keypad**: The Keypad module offers drivers for interfacing with keypad matrices. It simplifies reading keypresses from standard matrix keypads.

- **LED**: The LED module provides drivers for controlling LEDs. You can easily turn LEDs on or off and control their states using these drivers.

### MCAL (Microcontroller Abstraction Layer)

The MCAL directory contains low-level drivers and abstractions specific to the PIC18F4620 microcontroller. These drivers interact directly with the hardware registers of the microcontroller.

#### Modules

#### Modules
- **GPIO**: Configuration and control of digital input and output pins.
- **interrupt**: Handling interrupt-related functionality.
- **ADC**: Interface with the analog-to-digital converter peripheral.
- **CCP**: Configuration and control of Capture, Compare, and PWM features.
- **EEPROM**: Accessing internal EEPROM memory.
- **I2C**: Implementing I2C communication protocol.
- **SPI**: Configuring and controlling the Serial Peripheral Interface.
- **USART**: Configuration and control of the Universal Synchronous/Asynchronous Receiver/Transmitter.
- **TIMER0, TIMER1, TIMER2, TIMER3**: Configuration and control of timer peripherals.
- **std_libraries.h and std_types.h**: These header files define standard types and libraries used throughout the project. They enhance code portability and readability.

### Application

The Application files is where you can place your specific application code that uses the drivers from the HAL and MCAL directories. This is where you can create projects and build applications tailored to your requirements.


