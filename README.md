# Korg MS-20M Extra Midi to CV Module
## Intro
The [Korg MS-20M](https://www.korg.com/us/products/dj/ms_20mkit/) is a desktop module that adds many features over the original MS-20, but it also removes two: there is no VCO2 CV IN, nor Control Wheel CV. The VCO2 CV IN mod was done by beatnic.jp (see [this link](http://beatnic.jp/wp/wp-content/uploads/2015/10/MS-20M_Kit_Reconstruction.pdf)). With this module, I solve for the missing control wheel CV. The module snoops into the Midi In signal from the 5-pin socket (not USB), and provides CV patch points for Pitch Bend (PB), Mod Wheel (MW, CC 1), Velocity (Vel) and Channel Aftertouch (AT). PB CV is -5V to +5V, while MW, Vel and AT CVs are 0V to +5V.

Note that with this module, and maybe some minor modifications, you could also program other additions from midi, such as CV out with a different note priority (MS-20M is high note legato priority), 2nd midi  note CV out, multi trigger out instead of legato, etc.

<img src="/images/Patch Points Zoom.jpeg" alt="Patch Points" width="400">
<sub>The MS-20M in the pictures also has the VCO2 CV In mod.</sub>

## Hardware
The module is based on an Arduino Nano, an MCP4728 Quad 12bit DAC, and a dual supply OpAmp. The MCP4728 outputs directly to the patch points 0 to +5V for MW, Vel and AT, and feeds 0 to +5V to the OpAmp for PB. The OpAmp converts the 0 to +5V signal from the DAC into a -5V to +5V signal for the patch point (OpAmp is in a differential amplifier configuration). The Arduino Nano grabs 12V from the Korg MS-20M, as well as the line level Midi signal, by connecting to a small solder point under a via in the power board. The Arduino supplies regulated 5V for the MCP4728, and for one of the inputs of the OpAmp (the circuit requires a relatively precise 5V source, which is provided by the Nano). The OpAmp uses the +15V and -15V patch points in the Korg MS-20M main PCB. The modulecan be build on a 50x70mm perfboard.

<img src="/images/Module.jpeg" alt="Module" width="400">
<sub>The module in the picture uses different parts than the ones I recommend in the BOM below.</sub>

### Internal placement of the module:
The module can be placed at the bottom side of the back of the MS-20M, in the opposite side of the power connector. This placement interferes with one of the screws of a rubber foott, so if you use the rubber feet you might need to change the location of the module or find a way to attach the rubber foot without a screw. I used a 3D printed part to screw the module to one of the bottom screws of the MS-20M, whichand together with some velcro, the module can be fixed hidden inside the MS-20M.

<img src="/images/Installed Module.jpeg" alt="Installed Module" width="600">
<img src="/images/Installed Module Zoom.jpeg" alt="Installed Module Zoom" width="400">
<sub>The MS-20M in the pictures also has the VCO2 CV In mod.</sub>

### Schematic
<img src="/images/Schematic.png" alt="Schematic" width="600">

### Connections from the MS-20M:
* Midi In line level (Power Board, I solder on a via, see pictures)
* 12V (Power Board, relatively easy point on one of the connectors, see pictures)
* +15V, -15V, Ground (Main PCB, 3 patch points)
 
<img src="/images/Solder Point Midi 1.jpg" alt="Solder Point Midi 1" height="400"> <img src="/images/Solder Point Midi 2.jpg" alt="Solder Point Midi 2" height="400">

<img src="/images/Solder Point 12V.jpg" alt="Solder Point 12V" height="400"> <img src="/images/Solder Point 15V, Gnd.jpg" alt="Solder Point +-15V, Gnd" height="400"> 

### Bill of Materials
* 1x Arduino Nano
* 1x MCP 4728 (you can use the [Adafruit MCP4728](https://www.adafruit.com/product/4470). The MCP4728 IC only comes in a tiny form factor (MSOP10) suitable for SMD, not compantible with a perfboard. Alternatively, you can buy the IC and an adapter board. 
* 1x TL071 OpAmp (comes in DIP8 package, e.g. [TL071IP](https://mou.sr/45845I1))
* 2x 10K Ohm resistors
* 1x 0.1uF non polarized capacitor (decoupling capacitor for the MCP4728)
* 4x TS Audio Jacks (e.g. [ACJM-MN-2](https://mou.sr/3DDrzIO))
* 1x 50x70mm perfboard. (e.g. [Kit](https://www.ebay.com/itm/266349684113?mkcid=16&mkevt=1&mkrid=711-127632-2357-0&ssspo=Y32Ir_LeSfK&sssrc=2047675&ssuid=X02keB-tSR2&widget_ver=artemis&media=COPY))
* 5x screw terminal blocks (2x2P and 3x3P) (see kit above). 

For the 3D printed part:
* 2x Brass Threaded Insert Knurled Nuts M2x4x3.2 (I used inserts from [this kit](https://www.ebay.com/itm/305033383223?mkcid=16&mkevt=1&mkrid=711-127632-2357-0&ssspo=kbyZayRWTpy&sssrc=2047675&ssuid=X02keB-tSR2&widget_ver=artemis&media=COPY))
* 1x Brass Threaded Insert Knurled Nuts M3x6x5 (see above)
* 2x M2, 5mm screws (used to screw the perfboard to the adapter)
* 2x Velcro Strips (I used [Command Medium Picture Hanging Strips](https://a.co/d/0iIXbFK), one on the 3D printed part, and one inside the MS-20M casing, se picture above)

## Arduino Code
### Midi Library
I use two custom libraries. I introduced a change in the standard [Arduino Midi Library](https://github.com/FortySevenEffects/arduino_midi_library), that modifies the way it handles Pitch Bend data. Instead of providing Pitch Bend values of -8192 to 8192, with 0 being no Pitch Bend, the modified library provides values from 0 to 16383, with 8192 being no Pitch Bend. This minor change reduces overhead, since the DAC receives only positives values, and it makes no sense for the Arduino to deduct 8192 from the Pitch Bend midi message only to add it again before sending it to the DAC. 

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
In the 'arduino' folder there is a zip file with the [Modified Midi Library](https://github.com/retango/korg-ms20m-extra-midi2cv/raw/main/arduino/MIDI_Library_Modified_for_MS20M.zip).

### MCP4728 Library
The code uses to  [MCP4728 Library](https://github.com/BenoitSchillings/mcp4728) (by Benoit Schillings), not the official one. In the 'arduino' folder there is a zip file with this [MCP4728 Library](https://github.com/retango/korg-ms20m-extra-midi2cv/raw/main/arduino/mcp4728_for_MS20M.zip).

### Main Code
The main code uses callbacks to process midi messages. For MW, Vel and AT the code creates in the setup section a table of 12bit DAC values for each of the 128 possible midi values. For PB, the code bitshifts by 2 the 14bit Midi PB value in order to get a 12bit DAC value (a fast way of integer dividing by 4).

## Content in this GitHub Project
* Arduino Code
* Arduino libraries
* STL and Rhino file for the 3D printed Adapter
* PDF and Word for the front panel sticker (it uses the News Gothic font, embedded in the Word doc)
* PDF and Kicad Schematics

## Useful Links
[Design a Unipolar to Bipolar Converter for a Unipolar Voltage Output DAC](https://masteringelectronicsdesign.com/design-a-unipolar-to-bipolar-converter-for-a-unipolar-voltage-output-dac/).

[Another MIDI to CV Box 7 Steps](https://www.instructables.com/Another-MIDI-to-CV-Box-/)

[Mod Wiggler post: Korg MS20M what are the 14 spare holes for?](https://modwiggler.com/forum/viewtopic.php?t=132096). This one has more cool mods.

[beatnic.jp Adding mods to MS-20M](http://beatnic.jp/takedanotes/adding-mods-to-ms-20m/). The original VCO2 CV IN Mod

[MIDI4CV - Arduino MIDI to 4xCV Interface for VCO Pitch Control](https://www.instructables.com/MIDI4CV-Arduino-MIDI-to-4xCV-Interface-for-VCO-Pit/)

