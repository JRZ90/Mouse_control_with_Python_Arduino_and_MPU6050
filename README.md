
This is a way to control the mouse pointer and keystrokes in Windows with an arduino (without HID) and using an MPU6050 accelerometer and 
 Python as an interpreter of the arduino signals. My first idea was put the MPU on a hat for control the computer only with the movements
 of my head. I think this could be useful for certain people with some kind of movement restrictions.

## Requirements
### Hardware
* Arduino board with USB connection, any Arduino compatible board with CH340 chip work just fine
* MPU6050 accelerometer module

### Environment
* Windows
* Python 3
* Arduino

### Python modules
* PyAutoGUI 0.9.50
* pyserial 3.4

### Arduino libraries (included)
* I2Cdev
* MPU6050
