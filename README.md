## LED Matrix

A simple 5x13 LED Matrix using SIPO shift registers to allow individual addressing of 65 LED's via 8 digital GPIO's.

## Why?

This is noting revolutionary, and it's certainly been done a million times before, but never by me. This is essentially
a learning experience for me. I've decided on a 5x13 matrix size as it will be convenient to adopt into a digital clock
using a 5x3 digits, plus a single column for ":"

## What's in the repo?

I'm starting this from the board up. I'll begin with the schematics, and subsequent PCB design (using kicad).
I'll follow this up with the software to drive the matrix. Prototyping will be done on an Arduino, with the 
intent to port over to an AVR, or possibly PIC MCU for the "production" version.
