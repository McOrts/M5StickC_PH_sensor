# pH sensor for water based solution based on ph-4502c and ESP32 microprocessor 

## The problem
My aquarium always has water turbidity problems.

## Deployed idea
<img src="https://github.com/McOrts/Water_flow_sensor_MQTT/blob/master/Pictures/WEMOS-D1-mini-PRO_pinout.jpg" width="400" align="left" />

I have developed my own home automation architecture. The archetype for any sensor includes a microcontroller based on ESP8266 and connectivity though a MQTT broker. 

Another server with Node-RED make the orchestration and integration of all the sensors and services.

The best implementation for this use case is WEMOS D1 mini PRO, since a large WiFi range required. And it provides a 5v supplier pinout on board needed for the water flow sensor.

## Things used in this project
**Hardware components**

- [WEMOS D1 Mini Pro 16M Bytes External Antenna](https://s.click.aliexpress.com/e/kMC1v8nW) ×	1	 
<img src="https://github.com/McOrts/Water_flow_sensor_MQTT/blob/master/Pictures/WemosD1MiniPro.PNG" width="250" align="right" />

- [G1/2 Inch Water Flow Hall Sensor Switch Flow Meter](https://s.click.aliexpress.com/e/pBkWOMCg) ×	1	
<img src="https://github.com/McOrts/Water_flow_sensor_MQTT/blob/master/Pictures/WaterFlowSensorHall.PNG" width="250" align="right" />

**Software apps and online services**
- [Arduino IDE](https://www.hackster.io/arduino/products/arduino-ide?ref=project-8e87cc)
- [Eclipse Mosquitto open source MQTT broker](https://mosquitto.org/)
- [Node-RED](https://nodered.org/)

## How does it Work?
This sensor sits in line with your water line that you want to measure and contains a pinwheel sensor to measure how much liquid has moved through it. There's an integrated magnetic hall effect sensor that outputs an electrical pulse with every revolution. The hall effect sensor is sealed from the water pipe and allows the sensor to stay safe and dry.
<img src="https://github.com/McOrts/Water_flow_sensor_MQTT/blob/master/Pictures/HT_Water_flow_sensor_hall_sensor.JPG" width="300" align="left" />
The sensor comes with three wires: red (5-24VDC power), black (ground) and yellow (Hall effect pulse output). By counting the pulses from the output of the sensor, you can easily calculate water flow. Each pulse is approximately 2.25 milliliters. Note this isn't a precision sensor, and the pulse rate does vary a bit depending on the flow rate, fluid pressure and sensor orientation. It will need careful calibration if better than 10% precision is required. However, its great for basic measurement tasks!

## Development
I have used prototyping material. What is working is proof of concept for the use case of the system control of my garden irrigation.

**Schematics**
<img src="https://github.com/McOrts/Water_flow_sensor_MQTT/blob/master/Pictures/Water_flow_sensor_MQTT_bb.png" width="300"  align="center" /> 
