# Description #
This is a Ethernet controlled RGB led-strip music visualizer. It uses WS2812
strip for display, a PIC16F1825 as a host controller and a ENC28J60 for ethernet
communication. The communication is one-way with raw ethernet frames (Yay! It's terrible). This
project has three parts

- Host controller code in __micro__
- A daemon to listen to a pulseaudio monitor and send the visualization to the led-strip device with a hard-coded MAC address.
- A _completely untested_ board design for the device. I'll return to it someday. Maybe.

# Demo #
[In this YouTube video](https://youtu.be/0CW2xSNjpUU) is a demonstration of a breadboard prototype.


# Disclaimer #
This project is entirely for my own satisfaction and not left in a complete, nor readily consumable state.
