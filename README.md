# Door-Locker-Security-System
Developing a system to unlock a door using a password.
- Drivers used to build the project: GPIO, Keypad, LCD, Timer, UART, I2C, EEPROM, Buzzer and DC-Motor
- The used Microcontrollers in the project: ATmega32.
- The project is designed and implemented based on the layered architecture.

Human Machine Interface(HMI_ECU):
----------------------------------------
 This unit is just responsible of the interaction with the user through a Keypad that takes inputs, either an option or the password, and Liquid Crystal Display(LCD) screen that displays messages to the user.

- The architecture layer of HMI_ECU unit:
![HMI_ECU-Layer-ach](https://user-images.githubusercontent.com/104661871/215106843-2d086d52-54d5-42cf-ab81-3b026c04c208.png)

Control_ECU:
--------------------------------------
 This unit is responsible of all the processing and decisions in the system like password checking, comparing to that saved in the external Electrically Erasable Programmable Read-Only Memory (EEPROM), opening and closing the door through a DC-motor, and activating the system alarm.

- The architecture layer of Control_ECU unit:
![Control_ECU](https://user-images.githubusercontent.com/104661871/215108659-c6c290c5-b6e0-4779-b17e-e261dc5ddac2.png)

## The project schematic on the used simulation software (protues):

![Door security system](https://user-images.githubusercontent.com/104661871/215101577-e3218616-77c0-4961-b60a-37b6eaff2be0.png)

