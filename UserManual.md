# User Manual

## System Overview

Currently, the tide pool and lagoon oscillate between high and low tide every six and a half hours. The system wants the tide pool to be at high tide when the lagoon is at low tide and vice versa. When the [arduino program](TidePoolLCD/TidePoolLCD.ino) starts, the tide pool fills with water from the lagoon. This filling by turning the pump off for five minutes and then turning it on for the next five minutes repeats until the high tide sensor is triggered. **There is a delay of the remaining five minutes that the pump is on for the pump to turn off and the LCD screen to update. This is so the pumps don't cycle on and off too quickly.** If for some reason the water level decreases, the pump will fill again until the high tide sensor is on. After six and a half hours passed, the system then will pump water from the tide pool into the lagoon until the tide pool's low tide sensor is triggered. After six and a half hours pass, the pump to fill the tide pool turns on and this tidal system repeats.

This system is connected to hardware that connects it to Wi-Fi called the Adafruit Huzzah ESP8266 Breakout Board. This system gets the height values from the ultrasonic sensor that the Arduino records. These values are then sent to Adafruit io, a free online tool. The levels are recorded every three seconds and are kept for a month and can be found at [Adafruit IO Online Dashboard](https://io.adafruit.com/anonymousAlligator/dashboards/ultrasonicsensordashboard). If the Arduino stops, the data isn't sent to Adafruit io for an hour, an email notification is sent.

## Outputs
### LCD Screen
Currently, the LCD Screen has two rows, the top row prints the current state.
The current state may be any of the following:

| State Message | Meaning |
| ------- | ------- |
| Tide Pool Full | The tide pool finished filling and switches over to draining |
| Filling Tidepool | This is displayed in the 5 minute interval of where the pump filling the tide pool is on |
| Pause Filling Tide | This is displayed in the 5 minute interval where the fill pump turns off, to slow the fill cycle |
| Stop fill high tide | The high tide sensor is triggered, so the fill pump is turned off |
| Tide Pool Drained | The tide pool finished the 6.5 hour draining cycle and will now switch to filling the tide pool |
| Draining Tidepool | This is displayed in the 5 minute interval where the pump to drain the tide pool is on |
| Pause Draining Tide | This is displayed in the 5 minute interval where the drain pump turns off, slowing down the drain cycle |
| Stop fill low tide | The low tide sensor is triggered, so the drain pump is turned off |

The second row of the LCD displays the current sensor states

| Message | Meaning |
| ------- | ------- |
|         | The tide pool water level neither low or high |
| LOW TIDE | The tide pool water level is below the low water sensor |
| HIGH TIDE | The tide pool water level is above the high water sensor |
| SENSOR ERROR | Both low and high tide is detected. This should be impossible, and indicates one of the sensors is not working |


## LED indicator
The second way to tell whether the Arduino is working is by looking at the LED
light in the black container. This LED blinks fast (every 0.2 seconds) if a
pump should be on. The LED blinks slow if both pumps should be off. If the LED
is not blinking, at the high or low tide sensor is on. The color of the light
indicates whether the tide pool is filling or draining

| Color | Meaning |
| ----- | ------- |
| Blue  | The tide pool is draining see the above paragraph if the pump should be on or not |
| Green | The tide pool is filling  see the above paragraph if the pump should be on or not |



## Stuff isn't working
A good way of fixing most issues is to restart the Arduino and/or the Wi-Fi board. The way to reset these are pictured in the below text. In addition, there are some ways of diagnosing issues included below.

 **Problem: The online log isn't updating.**
 **Cause:** this could have multiple causes. I need you to do a few things to help figure out what the cause could be.
1. First, try flipping the high or low tide sensor. If the LCD screen doesn't change color when the sensor is flipped, the Arduino is halted. Restart the Arduino by pressing the reset button pictured below. If the green LED doesn't light up when the button is pressed, go to step 5. If the LCD screen updated when the sensor if flipped, proceed to 2.
![Image of Grove Seeed Base Shield V2 Reset Button](https://raw.githubusercontent.com/meyerpa/Tide-Pool/master/Reset%20Arduino.png)
2. The Arduino is running fine. This means that it could be a problem either with the ultrasonic sensor detecting a distance of 0 or with the Wi-Fi chip not connecting to the Adafruit IO website properly. Try resetting the Wi-Fi chip by pressing the button indicated in the image below. If the red and blue LCDs doesn't light up when reset, go to step 6. This chip could have lost connectivity to the wireless network. Wait two minutes and check the feed. If the feed is still not updating, go to 3.
![Image of Adafruit Huzzah ESP8266 Reset Button](https://raw.githubusercontent.com/meyerpa/Tide-Pool/master/Reset%20Wi-Fi.jpg)
3. Replace the ultrasonic sensor with a new one. The environment we are working in is corrosive to the sensor, and the sensor could be broken. If this doesn't
fix the issue proceed to step 4.
4. Make sure all of the connections are correct. The pins should be connected as follows:

| Arduino | Ultrasonic Sensor |
| ------- | ----------------- |
| 5V | Vcc |
| Gnd | Gnd |
| 6 | Trig |
| 7 | Echo |

5. Ensure the Arduino's power adapter is plugged in and the red LED is illuminated. If there still isn't any power, go to step 7.

6. Ensure that the Wi-Fi chip is plugged in and connected correctly. Make sure the Serial cable's black wire is connected to ground and is plugged in to the tan power strip. The Wi-Fi chip should be connected to the Bi-directional logic converter as follows:

| Logic Converter | Wi-Fi Chip |
| --- | --- |
| LV | V+ |
| GND | GND |
| LV1 | 13 |
| LV2 | 12 |

Then the Logic Converter chip should be connected to the Arduino as follows:

| Arduino | Logic Converter |
| --- | --- |
| 5V | HV |
| Gnd | GND |
| HV1 | 4 |
| HV2 | 5 |

7. Ensure the power strip is turned on. Make sure that the tan power strip red button is flipped to RESET. Still not working? Go to step 8.

8. Make sure the GFCI outlet is powered on. This outlet is outside the waterproof box. It powers the power strip. Make sure the green light on this is on. If it is not, reset it. If the power still doesn't turn on, the breaker outside the room may need to be flipped.

**Problem: The online log says the tide has been high or low for over 6 hours**
**Cause:** The drain pump is not getting turned on. Do the following steps
1. Make sure the relay is plugged powered on and is connected incorrectly. The Arduino's pin and the relay should be connected as follows:

| Arduino | Relay |
| ------- | ----- |
| 5V | Vcc |
| 12 | IN1 |
| 11 | IN2 |
| GND | Gnd |

**Problem: The high or low sensor is stuck**
**Cause:** First make sure, the white bobbers are clean and can easily slide. Second, make sure the connections are correct. Check that pins 2 and 3 are connected to the wire stands in the brown breadboard. Also, make sure below each resistor, the ground is connected properly.

**Problem: The distance seems to be messed up**
**Cause:** The ultrasonic sensor could be connected improperly or could be corroded. Try replacing the ultrasonic sensor inside the white 4" PVC pipe by disconnecting the wires and plugging in a new ultrasonic sensor. If the readings are still faulty, make sure the ultrasonic sensor is connected properly:

| Arduino | Ultrasonic Sensor |
| ------- | ----------------- |
| 5V | Vcc |
| Gnd | Gnd |
| 6 | Trig |
| 7 | Echo |
