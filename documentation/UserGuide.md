# The umlma User Guide

Lost your aircraft? Don't panic; wait for the beeps! After the *SOS Timer* expires, the Ultra-Micro Lost Model Alarm (umlma) beeps SOS every 10 seconds for many hours until you find your missing aircraft.

Also, umlma beeps W in Morse code after a preset *Flight Timer* expires to remind you to land and recharge your aircraft battery.

##Features
  * Beeps W to warn you to land and recharge
  * Beeps SOS to help you find your lost plane
  * Configurable *Flight Timer* (5, 10, ... 30 minutes) and *SOS Timer*
  * Ultra light, less than 4g with battery
  * Super-efficient 1mA buzzer delivers 80dbA @ 10cm
  * Powered by compact CR1225 battery with low battery warning

##Powering Up
  * Install a CR1225 battery if one isn't installed yet
  * Short the SWITCH pin headers with supplied shorting block
  * After power up, umlma checks the battery voltage and beeps status (see below)

| Beep | Text | Description |
| --- | --- | --- |
| --- -.- | OK | Battery voltage ok |
| ...---... | SOS | Time to get a new CR1225 battery |

* dit = .
* dah = -

###Voltage Calibration
The umlma is calibrated for the low voltage threshold of ~2.5V after being assembled. The firmware performs auto-calibration on initial startup.

##How It Works
Typical RC plane batteries support flight times from 5-15 minutes and running your LiPo battery down too low will ruin it.

After the *Flight Timer* expires, umlma will start beeping W in Morse Code every 10 seconds to warn you it is time to land.

If you lose your plane, the umlma will start to beep SOS in Morse Code after the *SOS Timer* expires. The *SOS Timer* is automatically set to twice the value of the *Flight Timer*

| Beep | Text | Description |
| --- | --- | --- |
| .-- | W | Flight time warning, time to land |
| ...---... | SOS | Listen for this when looking for your lost plane |

* dit = .
* dah = -

##Setting the Flight Timer
It's easy to configure the *Flight Timer*. 

1. Hold down the pushbutton switch while powering on
2. The umlma will beep out the current Flight Timer (as in the table below)
  * If the current *Flight Timer* is what you want, release the button
  * Or, to increment the timer, keep holding the button another 3 seconds
3. umlma will increment the *Flight Timer* by 5 minutes
4. go to 2.

| Beep | Flight Timer | SOS Timer |
| --- | ------------ | --------- |
| . | 5m | 10m |
| - | 10m | 20m |
| -. | 15m | 30m |
| -- | 20m | 40m |
| --. | 25m | 50m |
| --- | 30m | 60m |

* dit = .
* dah = -

##Recovery
###How Long Does It Beep?
The umlma is designed with the optimum tradeoffs between weight, size, and battery life. The umlma is designed to conserve as much battery power as possible to be devoted to recovery. A new battery should continue beeping for approximately 20-30 hours, based on theoretical calculations. Temperature and battery age and construction will affect its longevity.

###How Loud Is It?
Because of weight and size constraints, the umlma uses a CR1225 battery and 
small, efficient buzzer which delivers 80dBA @ 10cm.  As a result, there is a
limit to how close you have to be to the umlma to hear it beeping.  Consider
getting help from a younger person with better hearing. :)
  
##Larger Battery
Instead of the CR1225, you can connect a larger battery to the VCC (positive) and GND (ground) pins on the board. 

A dedicated battery is best. If you lose your plane after running the flight battery down, there will still be power to run the umlma.

##Remote Switch
If you mount your UMLMA inside your plane, you can install an extension wire from the SWITCH pins to a remote switch outside the plane.

## Questions or Problems?
Contact me (Michael Shimniok) at http://www.bot-thoughts.com for more help