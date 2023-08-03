# Korg MS-20M Extra Midi to CV Module
Midi-to-CV internal module for the Korg MS-20M, to get CV for PBend, MWheel, Vel and ATouch

This is a module that snoops into the Midi In of the Korg MS-20M (the 5-pin midi socket only, not USB), and creates CV signals for Pitch Bend (PB), Mod Wheel (MW, CC 1), Velocity (Vel) and Channel Aftertouch (AT). PB CV is -5V to +5V, while MW, Vel and AT CVs are 0V to +5V. You patch these 4 CV signals using 4 of the 14 holes available in the MS-20M.

<img src="/images/Patch Points Zoom.jpeg" alt="Patch Points" width="400">
<sub>The MS-20M in the pictures also has the VCO2 CV In mod.</sub>

## Hardware
The module is based on an Arduino Nano, an MCP4728 Quad 12bit DAC, and a dual supply OpAmp. The MCP4728 outputs directly to the patch points for MW, Vel and AT, and feeds 0 to 5V to the OpAmp for PB. The OpAmp coverts the 0 to 5V signal from the DAC into a -5V to +5V signal for the patch point (OpAmp is in a differential amplifier configuration). The Arduino Nano grabs 12V from the Korg MS-20M, as well as the line level Midi signal, which is snooped from a small solder point under a via in the power board, within the Midi In circuit. The Arduino supplies regulated 5V for the MCP4728, and for one of the inputs of the OpAmp (the circuit requires a relatively precise 5V source, which is provided by the Nano). The OpAmp uses the +15V and -15V patch points in the Korg MS-20M main PCB. You can build the module using a 50x70mm perfboard.

<img src="/images/Module.jpeg" alt="Module" width="400">

### Internal placement of the module:
The module can be placed at the left bottom side of the back of the MS-20M. This location interferes with one of the rubber feet, so if you use the rubber feet you might need to change the location or find another way to attach on of them. I use a 3D printed part to screw the module to one of the bottom holes, which together with a strip of velcro leaves the module fixed inside the MS-20M. You need to insert 2 M2 brass threaded inserts into the 3D printed part to screw the perfboard, and and additional M3 brass threaded insert that attaches to one of the bottom M3 screws of the MS-20M.

<img src="/images/Installed Module.jpeg" alt="Installed Module" width="600">
![Installed Module Zoom](https://github.com/retango/korg-ms20m-extra-midi2cv/assets/62658263/dc8de850-9f40-43de-ad03-1d188637e1e9)
<sub>The MS-20M in the pictures also has the VCO2 CV In mod.</sub>

### Schematic
![Schematic](https://github.com/retango/korg-ms20m-extra-midi2cv/assets/62658263/32c14f4a-8db4-464f-a62d-5a7c29958236)

### Connections from the MS-20M:
* Midi In line level (Power Board, I solder on a via, see pictures)
* 12V (Power Board, relatively easy point on one of the connectors, see pictures)
* +15V, -15V, Ground (Main PCB, 3 patch points)

### Bill of Materials:
* 1x Arduino Nano
* 1x MCP 4728 (you can use the [Adafruit MCP4728](https://www.adafruit.com/product/4470). The MCP4728 IC only comes in a tiny form factor (MSOP10) suitable for SMD, not compantible with a perfboard. Alternatively, you can buy the IC and an adapter board. 
* 1x TL071 OpAmp (comes in DIP8 package, e.g. [TL071IP] (https://mou.sr/45845I1))
* 2x 10K Ohm resistors
* 1x 0.1uF non polarized capacitor (decoupling capacitor for the MCP4728)
* 4x TS Audio Jacks (e.g. [ACJM-MN-2](https://mou.sr/3DDrzIO))
* 1x 50x70mm perfboard. (e.g. [Kit](https://www.ebay.com/itm/266349684113?mkcid=16&mkevt=1&mkrid=711-127632-2357-0&ssspo=Y32Ir_LeSfK&sssrc=2047675&ssuid=X02keB-tSR2&widget_ver=artemis&media=COPY))
* 5x screw terminal blocks (2x2P and 3x3P) (see kit above). 

For the 3D printed part:
* 2x Brass Threaded Insert Knurled Nuts M2x4x3.2 (I used inserts from [this kit](https://www.ebay.com/itm/305033383223?mkcid=16&mkevt=1&mkrid=711-127632-2357-0&ssspo=kbyZayRWTpy&sssrc=2047675&ssuid=X02keB-tSR2&widget_ver=artemis&media=COPY))
* 1x Brass Threaded Insert Knurled Nuts M3x6x5 (see above)
* 2x M2, 5mm screws
* 2x Velcro Strips (I used [Command Medium Picture Hanging Strips](https://a.co/d/0iIXbFK), one on the 3D printed part, and one inside the MS-20M casing)

## Arduino Code
I use two custom libraries. I introduced a change in the standard  [Arduino Midi Library](https://github.com/FortySevenEffects/arduino_midi_library), that modifies the way it handles Pitch Bend data. Instead of providing Pitch Bend values of -8192 to 8192, with 0 being no Pitch Bend, the modified library provides values from 0 to 16383, with 8192 being no Pitch Bend. This minor change reduces overhead, since the DAC receives only positives values, and it makes no sense for the Arduino to deduct 8192 from the Pitch Bend midi message only to add it again before sending it to the DAC.

In the [Arduino Midi Library](https://github.com/FortySevenEffects/arduino_midi_library), in the file [src/midi_Defs.h](https://github.com/FortySevenEffects/arduino_midi_library/blob/master/src/midi_Defs.h), the changes are:
```
#define MIDI_PITCHBEND_MIN      -8192
#define MIDI_PITCHBEND_MAX      8191
```
to:
```
#define MIDI_PITCHBEND_MIN      0
#define MIDI_PITCHBEND_MAX      16383
```
in the Arduino folder there is a zipper file with the [Modified Midi Library](https://github.com/retango/korg-ms20m-extra-midi2cv/raw/main/arduino/MIDI_Library_Modified_for_MS20M.zip).


for MW, Vel and AT I use a table of DAC values for each of the 128 possible midi values, and for pitch bend I bitshift 2 to the right the 14bit Midi PB value in order to get a 12bit DAC value (just a fancy but very fast way of integer dividing by 4).

## Content:
* Arduino Code
* STL and Rhino file for the 3D printed Adapter
* PDF and Word for the front panel sticker (it uses the News Gothic font, embedded in the Word doc)
* PDF and Kicad Schematics

## Useful Links:
[Design a Unipolar to Bipolar Converter for a Unipolar Voltage Output DAC](https://masteringelectronicsdesign.com/design-a-unipolar-to-bipolar-converter-for-a-unipolar-voltage-output-dac/).

[Another MIDI to CV Box 7 Steps](https://www.instructables.com/Another-MIDI-to-CV-Box-/)


