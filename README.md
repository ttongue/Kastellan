# Kastellan

Kastellan is an system for access control to facilities and equipment. It works with Weigand 26-bit RFID readers (125 kHz) and any locking mechanism that can be controlled with a 24V 15A relay. This is a repository for the firmware installed on the controller.

INSTALLATION
------------

The firmware is compiled and installed using the Energia development environment. It has been tested with the TI TM4C1294 (Tiva C) microcontroller chipset, but should work with other microcontrollers with a WiFi or Ethernet network connection, though modifications to pin assignments will be necessary for that to work.

The kastellan-src directory should be linked in the Energia documents directory, while the folders in the libraries directory should be linked to the energia libraries directory.
