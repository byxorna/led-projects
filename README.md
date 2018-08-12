# d18-scarf
arduino code for particle photon driven scarf for DI8ORIENT 2018

NOTE: arduino code for trinket pro 5v in `arduino/`, particle photon code in `src/`. Some random bits are in `arduino/` too, like:
* lounge lighting background (more pornjy ambient stuff)
* different scarf loadouts for diff people with custom patters+colors
* some tree hacks for art installs last minute

## Features

* patterns and palettes rotate in auto mode
* interpolation between pattern decks produces smooth fades between pattern changes
  * behaves like VJing between 2 decks, and interpolates between diff patterns in real time for a very organic feel
* manually change patterns or brightness by holding setup button for a few seconds (particle only)

## Adding Patterns

Patterns are autocycled through. Define a function with the signature

```
void pattern_you_wrote(NSFastLED::CRGB* leds, DeckSettings* s)
```

And add a reference to it in
```
const FP patternBank[] = {
  &<pattern_you_wrote>,
  &pattern_other_patterns,
};
```


## Setup

```
# get particle-cli
$ npm install
```

## Compile

```
$ node_modules/particle-cli/bin/particle.js compile photon
attempting to compile firmware
downloading binary from: /v1/binaries/59e420266d43bc4ff5174c76
saving to: photon_firmware_1508122650655.bin
Memory use:
   text    data     bss     dec     hex filename
  15340     108    1588   17036    428c /workspace/target/workspace.elf

Compile succeeded.
Saved firmware to: /Users/gabe/code/particle-neopixel-ring-blinky/photon_firmware_1508122650655.bin
```

## Flash

```
$ node_modules/particle-cli/bin/particle.js flash --usb $(ls -t photon_firmware*bin|head -n1)
Found DFU device 2b04:d006
spawning dfu-util -d 2b04:d006 -a 0 -i 0 -s 0x080A0000:leave -D photon_firmware_1508122560924.bin
dfu-util 0.9

Copyright 2005-2009 Weston Schmidt, Harald Welte and OpenMoko Inc.
Copyright 2010-2016 Tormod Volden and Stefan Schmidt
This program is Free Software and has ABSOLUTELY NO WARRANTY
Please report bugs to http://sourceforge.net/p/dfu-util/tickets/

dfu-util: Invalid DFU suffix signature
dfu-util: A valid DFU suffix will be required in a future dfu-util release!!!
Opening DFU capable USB device...
ID 2b04:d006
Run-time device DFU version 011a
Claiming USB DFU Interface...
Setting Alternate Setting #0 ...
Determining device status: state = dfuIDLE, status = 0
dfuIDLE, continuing
DFU mode device DFU version 011a
Device returned transfer size 4096
DfuSe interface name: "Internal Flash   "
Downloading to address = 0x080a0000, size = 15448
Download        [=========================] 100%        15448 bytes
Download done.
File downloaded successfully

Flash success!
```

## Flashing over USB

To enter DFU Mode:
- Hold down BOTH buttons
- Release only the RST button, while holding down the MODE button.
- Wait for the LED to start flashing yellow
- Release the MODE button
- The Core now is in the DFU mode.

## TODO

* [ ] add a double buffering system for changing how crossfades work between patterns
* [ ] setup new pattern that waves intensity, color, and color offset entirely with noise generators
* [x] make non-active deck not get a pattern computed when not crossfading
* [ ] tweak brightness and pattern FPS
* [x] setup new pattern that waves intensity, color, and color offset into the color wave

## Ideas

### mesh timing protocol

Each node has a unique ID (mesh address?), and their boottime (its all relative so... maybe their curr_time?)
each node broadcasts their ID+curr time
if you hear from someone:
  if their ID is > yours
    walk towards their timestamp
    (what about if ts is lower than mine, and another is > mine)
  ignore if ID is < yours

