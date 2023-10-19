# Arduino Autonomous Remote Controlled Robot Car
As the name suggests, this project is powered by an Arduino (not literally however, because some components need more volts than it can produce.). This is an autonomous & remote controlled robot car.

The car follows 3 different modes in accordance to 4 possible positions of 2 physical switches on the exoskeleton of the car. It can be controlled with a mobile phone using Bluetooth® or can autonomously drive without piloting.

<p align="center">
  <img src="https://i.ibb.co/bz2bxkD/Picture1.png">
</p>

## The Different Driving Modes
### Bluetooth® Remote Control Piloting
This form of driving requires a driver to control the car using a Bluetooth® connection & the iOS app [BLEJoystick](https://apps.apple.com/sa/app/ble-joystick/id1098200556).

The control functions look like **this**:
<p align="center">
  <img src="https://i.ibb.co/5Yt2Q8v/Screenshot-2023-10-13-at-7-34-33-PM.png">
</p>

The code to link the Arduino serial output to the Bluetooth sensor to reach the controller is as follows:

On each button needed, an instruction was programmed.
| Command | Letter |
| --- | --- |
| Forward | A or a |
| Backward | C or c |
| Left | B or b |
| Right | D or d |
| U-turn | F or f |
| Stop | G or g |

### Autonomous Ultrasonic Based Driving (Obstacle Avoidance)
This mode of autonomous driving used the echolocating abilities of ultrasonic sensors to detect obstacles and maneuver in accordance to the path with least obstacles, proving useful for completing mazes & races.

### Autonomous Infrared Based Driving (Follow the Path)
This mode of autonomous driving used infrared light sensors to detect the path below the car, if there was a deviation from the intended path (into a differently lit area, unintended for driving) then the car would self-correct to follow the path. This takes advantage of the fleet of 5 infrared sensors below the car.

## **Equipment and Software**

- IOS device with BLEJoyStick installed.

- Arduino Mega

- I2C LCD

- Bluetooth module

- Ultrasonic Sonar Distance Sensors

- IR Sensors

- Motor Driver H-bridge L298D

- 4-wheeled robotic car chassis (with motors)

- Rechargeable battery pack 3.7 V + battery holder

- Breadboard

- Jumper wires male to female 20 cm (40 pcs)

- Jumper wires male to male 20 cm (40pcs)

## **Components**

### **H-Bridge IC**

We can use the H-bridge to set the the polarity (direction of the rotation) for the motors inside the robotic vehicle, we mainly allow DC motors to run forwards or backwards. And in a special command, we can make the car u-turns by make the second side polar opposite in terms of direction, but still retaining the same EM power.

### **Bluetooth Module**

Bluetooth module is a PCBA board which integrated Bluetooth functions can be used in short-distance wireless communication. using this device, we can connect it to our IOS devices and send commands to our Arduino. The program we write in the Arduino will decode the commands and execute instructions based on received letters from our apps.

### **Robot vehicle**

It's made of all the previous parts plus the tires attached with motors. It carries 2 batteries as a power supply that powers the Arduino mega, the motors, the H-Bridge, and the Bluetooth Module.

### **IR Sensors**

An infrared (IR) sensor is a type of sensor that detects infrared radiation emitted or reflected by an object. It can be used to detect motion, temperature, and proximity. IR sensors are commonly used with Arduino boards for various projects such as obstacle detection, line following robots, and more.

### **Ultrasonic Sonar Distance Sensors**

The Sensors picks up an analog values, and luckily Arduino has pins to input the analog values, but they need to be converted to a PWM of the specified Arduino model. They work at about 2cm to 400cm away, but we think 10cm-250cm will get you the best results.

### **Liquid Crystal Display (LCD)**

Standard LCDs typically require around 12 connections, which can be a problem if you do not have many GPIO pins available, but I2C wiring is very simple. You only need two data pins to control the LCD.Each rectangle is made up of a grid of 5×8 pixels. Later in this tutorial, I will show you how you can control the individual pixels to display custom characters on the LCD and shows 16 characters for each row.


## **Summary**

In conclusion, this project gave me a first-hand experience in connecting and configuring a Bluetooth module to an Arduino, how to set up an LCD, and how to use Infrared sensors. I was also able to strengthen my knowledge in using and configuring a Motor Driver H-bridge L298D Module. I used both components to add Bluetooth functionality to the Robot Car and tested its movement capabilities.
