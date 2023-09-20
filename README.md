# PIC18F4620 Drivers Repository

This repository contains a collection of drivers and hardware abstraction layers (HAL) for the PIC18F4620 microcontroller. These drivers are designed to simplify the process of interfacing with various hardware components, making it easier to develop applications for the PIC18F4620 microcontroller.

## Table of Contents

- [Directories](#directories)
- [HAL (Hardware Abstraction Layer)](#hal-hardware-abstraction-layer)
  - [7_Seg](#7_seg)
  - [Button](#button)
  - [Chr_LCD](#chr_lcd)
  - [Dc_Motor](#dc_motor)
  - [Keypad](#keypad)
  - [LED](#led)
- [MCAL (Microcontroller Abstraction Layer)](#mcal-microcontroller-abstraction-layer)
  - [GPIO](#gpio)
  - [interrupt](#interrupt)
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

- **GPIO**: The GPIO (General-Purpose Input/Output) module is responsible for configuring and controlling the digital input and output pins of the PIC18F4620 microcontroller. It provides functions for pin initialization, read, write, and more.

- **std_libraries.h and std_types.h**: These header files define standard types and libraries used throughout the project. They enhance code portability and readability.

### Application

The Application directory is where you can place your specific application code that uses the drivers from the HAL and MCAL directories. This is where you can create projects and build applications tailored to your requirements.

## Usage

In the Application directory, you can develop your PIC18F4620 microcontroller applications using the provided drivers. You can include your main application code and organize your project files here.

