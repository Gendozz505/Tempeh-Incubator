# Tempeh-Incubator
**I'm glad to introduce you a launchpad with couple interesting features and comfortable interface for cooking lovers.**

You will need:
  * Arduino UNO
  * DHT22 sensor
  * 16x2 seeed LCD (if you use any other LCD you have to import a relevant librery)
  * Relay 9v to 220v
  * Switch  x2
  * Resistor 220 ohm  x2 (to avoid switch bouncing)
  * Greeb LED
  * Red LED
  * AC power adapter 9V
  * Heating element
  
connect all your periferia as described below:

  * DHT22 data pin to port 2
  * UP button pin to port 12
  * DOWN button pin to port 13
  * Green LED (anode) to port 3
  * Red LED (anode) to port 5
  * Relay data pin to port 4
  
 
 ###!!!IMPORTANT!!!⋅⋅
 ⋅⋅⋅Do not connect relay VCC pin to 5V port of arduino to avoid overload. Insted you may solder it to "tip connector" or use special shield.
