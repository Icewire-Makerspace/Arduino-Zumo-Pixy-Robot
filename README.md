# Arduino Zumo Pixy Robot

## Description

A small tank-like robot that follows an object of a chosen colour.

#### Usage

After turning on the Zumo shield, [teach Pixy the object](http://www.cmucam.org/projects/cmucam5/wiki/Teach_Pixy_an_Object_2) to follow using colour signature 1 (red). Hold the object the desired distance away from the robot and press the Zumo user button. The robot will attempt to keep itself at the same position from the object that it was when you pressed the button.

## Hardware

- [Arduino Leonardo](http://www.arduino.cc/en/Main/arduinoBoardLeonardo)
- [Zumo Shield for Arduino](https://www.pololu.com/product/2508)
  - [User's Guide](https://www.pololu.com/docs/0j57)
- [Pixy](http://charmedlabs.com/default/pixy-cmucam5/)
  - [Wiki](http://www.cmucam.org/projects/cmucam5/wiki)

## Dependencies

- [Zumo Shield Libraries](https://github.com/pololu/zumo-shield)
- [Pixy Arduino Libraries](http://www.cmucam.org/projects/cmucam5/wiki/Latest_release)