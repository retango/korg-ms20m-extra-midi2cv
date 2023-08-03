# korg-ms20m-extra-midi2cv
Arduino Nano Midi-to-CV internal module for the Korg MS-20M, get CV for PBend, MWheel, Vel and ATouch

This is an Arduino Nano based module that snoops into the Midi in (5-pin socket only, not USB) of the Korg MS-20M (Desktop Kit), and creates CV signals for Pitch Bend (PB), Mod Wheel (MW, CC 1), Velocity (Vel) and Channel Aftertouch (AT). PB CV is -5V to +5V, while MW, Vel and AT CVs are 0V to 5V. You can add connectors with this 4 CV signals using 4 of the 14 holes available in the MS-20M.

The module requires an Arduino Nano, and MCP4728 Quad 12bit DAC, and a dual supply OpAmp. The MCP outputs directly to the patch points 0 to 5V for MW, Vel and AT, and feeds 0 to 5V to the OpAmp for PB. The OpAmp coverts the 0 to 5V signal from the DAC into a -5V to +5V signal (differential amplifier configuration).

The arduino Nano gets 12V from the Korg MS-20, as well as a line level midi signal snooped from the Midi in circuit. The Arduino supplies regulated, steady 5V for the MCP4728, and for one of the inputs of the OpAmp. The OpAmp gets the dual voltage from the +15V and -15V patch points in the Korg MS-20M main PCB.

Connections from the MS-20M:
* Midi In line level (Power Board, see pictures)
* 12V (Power Board, see pictures)
* +15V, -15V, Ground (Main PCB, 3 big patch points)

You can build the module using a 50*70 mm perfboard (double sided prototype board), 5 screw terminal block: 2x2P and 3x3P ;

Internal placement of the module:
I use a spot to the left side of the power board, which works in my case since I'm not using the 4 rubber feet of the MS-20M. Where I placed it, the module interferes with on of the 4 feet, so if you are using them, you may need to find another location. I created a small 3D printed adapter part to fix the module in place, using one of the bottom screws, as well as velcro. 

Bill of Materials
1 Arduino Nano v2
1 50*70mm perfboard and screw terminal blocks (https://www.ebay.com/itm/266349684113?mkcid=16&mkevt=1&mkrid=711-127632-2357-0&ssspo=Y32Ir_LeSfK&sssrc=2047675&ssuid=X02keB-tSR2&widget_ver=artemis&media=COPY)
1 MCP 4728 (you can use the Adafruit MCP4728, since the MCP4728 only comes in a tiny form factor, only for surface mount soldering, which will not work on the perfboard) (https://www.adafruit.com/product/4470)
1 TL071 OpAmp
2 10K Ohm resistors
1 0.1uF capacitor
4 TS Audio Jacks



