# Korg MS-20M Extra Midi to CV Module
Midi-to-CV internal module for the Korg MS-20M, to get CV for PBend, MWheel, Vel and ATouch


This is a module that snoops into the Midi in (5-pin socket only, not USB) of the Korg MS-20M (Desktop Kit), and creates CV signals for Pitch Bend (PB), Mod Wheel (MW, CC 1), Velocity (Vel) and Channel Aftertouch (AT). PB CV is -5V to +5V, while MW, Vel and AT CVs are 0V to 5V. You patch these 4 CV signals using 4 of the 14 holes available in the MS-20M.

The is based on an Arduino Nano, an MCP4728 Quad 12bit DAC, and a dual supply OpAmp. The MCP4728 outputs directly to the patch points for MW, Vel and AT, and feeds 0 to 5V to the OpAmp for PB. The OpAmp coverts the 0 to 5V signal from the DAC into a -5V to +5V signal for the patch point (OpAmp is in a differential amplifier configuration).

The Arduino Nano uses 12V from the Korg MS-20M, as well as a line level Midi signal snooped from the 5 Pin Midi In circuit. The Arduino supplies regulated 5V for the MCP4728, and for one of the inputs of the OpAmp (the circuit requires a relatively precise 5V source, which is provided by the Nano). The OpAmp uses the +15V and -15V patch points in the Korg MS-20M main PCB.

## Connections from the MS-20M:
* Midi In line level (Power Board, I solder on a via, see pictures)
* 12V (Power Board, relatively easy point on one of the connectors, see pictures)
* +15V, -15V, Ground (Main PCB, 3 patch points)

You can build the module using a 50*70 mm perfboard (double sided prototype board), and screw terminal blocks;

## Internal placement of the module:
I use a spot to the left bottom side at the back of the MS-20M, which is fine if you don't use the 4 rubber feet of the MS-20M. If you need the rubber feet, you may need to find another spot for the module since the place I use interferes with one of the 4 feet. I use a small 3D printed part to screw the module one of the bottom screws, as well as some velcro. 

## Bill of Materials:
* 1 Arduino Nano v2
* 1 50*70mm perfboard and 5 screw terminal blocks (2x2P and 3x3P). [Kit](https://www.ebay.com/itm/266349684113?mkcid=16&mkevt=1&mkrid=711-127632-2357-0&ssspo=Y32Ir_LeSfK&sssrc=2047675&ssuid=X02keB-tSR2&widget_ver=artemis&media=COPY)
* 1 MCP 4728 (you can use the [Adafruit MCP4728](https://www.adafruit.com/product/4470), since the MCP4728 only comes in a tiny form factor, only for surface mount soldering, which will not work on the perfboard) 
* 1 TL071 OpAmp
* 2 10K Ohm resistors
* 1 0.1uF capacitor
* 4 TS Audio Jacks

## Arduino Code
I use two custom libraries. I changed slightly the Midi library in the way it handles pitch bend. Instead of providing Pitch Bend values of -xx to xx, with 0 being no pitchbend, the library returns 0 to xx, with xx being 0 Pitch bend. This minor change is to reduce overhead, since I'm sending only positives values to the DAC, and it makes no sense for the Arduino to deduct xx from the midi message, only to add it again to send it to the DAC.

for MW, Vel and AT I use a table of DAC values for each of the 128 possible midi values, and for pitch bend I bitshift 2 to the right the 14bit Midi PB value in order to get a 12bit DAC value (just a fancy but very fast way of integer dividing by 4).

## Content:
* Arduino Code
* STL file for the 3D printed Adapter
* PDF for the front panel sticker
* PDF and Kicad Schematic

## Useful Links:
[Design a Unipolar to Bipolar Converter for a Unipolar Voltage Output DAC](https://masteringelectronicsdesign.com/design-a-unipolar-to-bipolar-converter-for-a-unipolar-voltage-output-dac/).

[Another MIDI to CV Box 7 Steps](https://www.instructables.com/Another-MIDI-to-CV-Box-/)


