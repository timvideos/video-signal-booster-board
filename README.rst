Video Booster Board
===================

This repository contains a KiCad PCB project for Video Booster Board.
The board utilizes a pair of PTN3363 HDMI signal boosters to improve signal quality transmitted over long HDMI cables.
Booster configuration and HDMI control signals are managed by EFM32 microcontroller.

|

Video Booster Board data flow diagram:

.. image:: ./Images/vsb-data-flow.svg

|

Board visualization:

.. image:: ./Images/vsb-board.png


Getting Started with EFM32HG
============================

EFM32HG322 present on this board has factory flashed USB bootloader.  Informations about it are summarized in document `AN0042 <https://www.silabs.com/documents/public/application-notes/an0042-efm32-usb-uart-bootloader.pdf>`_. Regarding to it to enter bootloader it is necessary to short DBG_SWCLK pin (PF0) to 3V3. 
Those pins are exposed on 4-pin SWD connector (J11). After that plug a micro USB cable into J1 connector and connect it to a PC. The MCU should enumerate as USB CDC device. It's possible to print the chip ID by opening a serial terminal e.g. picocom and typing 'i' letter. 



