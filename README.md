# Tide-Pool
**This repository contains the code to run the tide pool
for the marine lab biology group.**

This code controls the pumping system for both the tide pool and the stingray
lagoon. There are currently two pumps that when one is on, it empties the tank
it is in filling the other tank.

![Simplified Tide Pool Diagram](SimplifiedTidePool.png?raw=true "Simplified Tide Pool Diagram")

Currently, the tide pool and lagoon oscillate between high and low tide every six and a half hours. The system wants the tide pool to be at high tide when the lagoon is at low tide and vice versa. When the [arduino program](TidePoolLCD/TidePoolLCD.ino) starts, the tide pool fills with water from the lagoon. This filling by turning the pump on for five minutes and then turning it off for the next five minutes repeats until the high tide sensor is triggered. If for some reason the water level decreases, the pump will fill again until the high tide sensor is on. After six and a half hours passed, the system then will pump water from the tide pool into the lagoon until the tide pool's low tide sensor is triggered. After six and a half hours pass, the pump to fill the tide pool turns on and this tidal system repeats.

The schematic for the current system looks like the below image:
![Tide Pool Schematic](TidePoolProposed_schem.png?raw=true "Tide Pool Schematic")

**Soon to be implemented** an  ultrasonic sensor to sense the exact tidal level. Then, we will mimic the tidal predictions from Kiptopeke, VA.
