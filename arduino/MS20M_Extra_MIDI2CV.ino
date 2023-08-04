/* 
Alejandro Frenkel, 2023
https://github.com/retango/korg-ms20m-extra-midi2cv
 */

#include <mcp4728.h>
#include <MIDI.h> // THIS MIDI LIBRARY IS MODIFIED VS. THE STANDARD: #define MIDI_PITCHBEND_MIN 0 and #define MIDI_PITCHBEND_MAX 16383 in midi_Defs.h

#define MIDI_CHANNEL 1 //the MIDI channel you want your box to listen to (1-16)
#define CC_NUMBER 1 //MIDI CC number to read and send via CC Out

mcp4728 dac = mcp4728(0); //check MCP4728 DAC address, for the Adafruit MCP4728 module it's 0x60 or 0x64

//Define DAC Channels
byte PBChannel = 0;
byte CCChannel = 1;
byte velocityChannel = 2;
byte ATChannel = 3; 

MIDI_CREATE_DEFAULT_INSTANCE();
uint16_t mappedValues[128];

void setup() {  
  for (byte i = 0; i < 128; i++) {
    mappedValues[i] = map(i, 0, 127, 0, 4095);
  }

  MIDI.setHandleNoteOn(handleNoteOn);
  MIDI.setHandleControlChange(handleCC);
  MIDI.setHandleAfterTouchChannel(handleAT);
  MIDI.setHandlePitchBend(handlePB);
  MIDI.begin(MIDI_CHANNEL);// start MIDI and listen to channel "MIDI_CHANNEL"

  dac.begin();

  //Serial.begin(115200); // Use if you need to debug
  //printStatus(); // Print all internal value and setting of input register and EEPROM - unquote function below if using. 
  //setupDAC(); // You can run this to save setup data to the DAC eeprom - unquote function below if using.
}
 
void loop() {
  MIDI.read(MIDI_CHANNEL);
  }

void handleNoteOn(byte channel, byte note, byte velocity){
    dac.analogWrite(velocityChannel, mappedValues [velocity]);
}

void handleCC(byte channel, byte number, byte value){
   if(number == CC_NUMBER){
      dac.analogWrite(CCChannel, mappedValues [value]);}
} 

void handleAT(byte channel, byte value){
   dac.analogWrite(ATChannel, mappedValues [value]);
} 

void handlePB(byte channel, uint16_t value){
   dac.analogWrite(PBChannel, value >> 2); // THIS ASSUMES MIDI LIBRARY HAS THIS CHANGES: #define MIDI_PITCHBEND_MIN 0 and #define MIDI_PITCHBEND_MAX 16383 in midi_Defs.h
} 

/*
void setupDAC()
{
  dac.setVref(0, 0,0,0); // set to use external voltage reference (=VDD, 2.7 - 5.5V)
  dac.analogWrite(CCChannel, 0);
  dac.analogWrite(velocityChannel, 0);
  dac.analogWrite(ATChannel, 0);
  dac.analogWrite(PBChannel, 2048); // sets pitch bend to 0;

  dac.eepromWrite(CCChannel, 0);
  dac.eepromWrite(velocityChannel, 0);
  dac.eepromWrite(ATChannel, 0);
  dac.eepromWrite(PBChannel, 2048); // sets pitch bend to 0;
  dac.eepromWrite();
  delay (100);
  
}

void printStatus()
{
  Serial.println("NAME     Vref  Gain  PowerDown  Value");
  for (int channel=0; channel <= 3; channel++)
  { 
    Serial.print("DAC");
    Serial.print(channel,DEC);
    Serial.print("   ");
    Serial.print("    "); 
    Serial.print(dac.getVref(channel),BIN);
    Serial.print("     ");
    Serial.print(dac.getGain(channel),BIN);
    Serial.print("       ");
    Serial.print(dac.getPowerDown(channel),BIN);
    Serial.print("       ");
    Serial.println(dac.getValue(channel),DEC);

    Serial.print("EEPROM");
    Serial.print(channel,DEC);
    Serial.print("    "); 
    Serial.print(dac.getVrefEp(channel),BIN);
    Serial.print("     ");
    Serial.print(dac.getGainEp(channel),BIN);
    Serial.print("       ");
    Serial.print(dac.getPowerDownEp(channel),BIN);
    Serial.print("       ");
    Serial.println(dac.getValueEp(channel),DEC);
  }
  Serial.println(" ");
}
*/