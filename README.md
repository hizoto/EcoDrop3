# EcoDrop3
This is an AüP (Ausbildungs übergreifende Projektarbeit) project from Team 3 at ZbW (zbw.ch).
The goal of this project ist to create an autonomous robot, which can collect 6 containers from 3 independent shafts.

There are two main controllers:
- ESP32: Main Controller
    - Hosting Webinterface
    - Controlling the Flow of the Program
    - Collecting Data
- Arduino:
    - Controlling the movement of the robot
    - Controlling lifting of the containers
    - Reading sensors affiliated with movement

The two controllers communicate via serial communication.
There are also multiple sensors using I2C:
On Arduino:
- ToF front VL53L0X
- ToF back VL53L0X
- Pixycam V2.0

On ESP32:
- Voltage and Current Sensor INA 3221 
- BME280 Temp + Humidity Sensor



# Dependencies
- 


# VS-Code Extensions to use
- Platformio
- ToDo Tree
- GitLens

