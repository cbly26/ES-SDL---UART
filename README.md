# Program:

**Authors:** Connor Bly and Adrian Petrillo
**Date:** 1/24/25

**Modified by:**
**Date:**

**Purpose:** Use serial communication (UART) to communicate between two nRF52840DKs, allowing for pressing of an internal button on one device to turn on the corresponding LED on the other device, and vice versa.

**Disclaimer:** We weren't able to complete this project due to time constraints, so this process will not take place if the .txt files are implemented. However, the startup code given by Nordic Development Academy does perform the intended exercise of allowing the keys "1-3" to toggle LEDs "1-3" on the connected device. We attempted to modify this working code into a finished product capable of completing our project, but unfortunately, we were unable to get the program working. Please find our attempted solutions in the "Receiver.txt" and "Sender.txt" files.

**Setup:** Starting with the prj.conf file, you will want to configure "SERIAL", "GPIO", "UART_ASYNC_API", and "EVENTS". Moving to the overlay file, set the pins to communicate via TX, RX, RTS, CTS with their correlating pins (should be P0.06, P0.08, P0.05, P0.07, respectively.)

In main.c, the sending and receiving code will be slighting different. Given that both computers will contain both sets of code eventually, we will assume that one computer is only a sender and one a receiver for now.

***Sender:*** An event is created for the sending of the information. The buttons, uart, and buffer are defined and initialized, along with a callback function that puts button registration information into the buffer for sending, and then sends the information. Inside main(), the uart and buttons are configured, and if the button is pressed, the eventManager sparks the process of sending the information inside of the buffer through the uart. After this is done, the while loop keeps this process going forever. Depending on which button is pushed, code inside the callback function should decide what information to place inside of the buffer for sending via uart.

***Receiver:*** 