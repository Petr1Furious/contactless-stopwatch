# Contactless stopwatch

Sketch for an Arduino contactless stopwatch. It was created for easy timing of solving Rubik's cube for speed.

## The device and files

The device consists of Arduino Nano, LCD screen, infrared rangefinder and 2 buttons.

The [source file](https://github.com/Petr1Furious/contactless-stopwatch/blob/main/contactless_stopwatch/contactless_stopwatch.ino) is located [here](https://github.com/Petr1Furious/contactless-stopwatch/blob/main/contactless_stopwatch).

## Usage

Instead of regular buttons, to start and stop the stopwatch uses infrared rangefinder to estimate distance to the nearest object above it.

First, it generates a Rubik's cube scramble. The user should scramble a Rubik's cube according to it. When the user is ready, they should hold the cube or hand above the rangefinder for half a second. As soon as they remove it, the stopwatch will start. After the Rubik's cube is solved, the user has to move their hand in front of the rangefinder to stop the stopwatch. After they remove it, they can reset the time by waving in front of it again.

The device is also capable of counting different average times.

## Settings

- **AVGmode** (whether the device should calculate average time)
- **Distance** (the maximum distance for the stopwatch to register it)
- **Scramble length**
- **Lights** (whether the side lights should be on or off)
- **Print scramble**
- **Reset with rangefinder**
- **Calculate AVG5/AVG12**
- **Beep** (interval at which the device will beep)
- **Buttons** (whether the stopwatch should also use buttons to register start or stop)

## Example

[Video example](https://drive.google.com/file/d/1IDDDfMxg3gMu3BhMweaLgeMKTUUtbX27/view?usp=sharing)
