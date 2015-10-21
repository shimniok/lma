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
  
##How It Works
Typical RC plane batteries support flight times from 5-15 minutes and running your LiPo battery down too low will ruin it.

After the *Flight Timer* expires, umlma will start beeping W in Morse Code (dit-dah-dah or .--) every 10 seconds to warn you it is time to land.

If you lose your plane, the umlma will start to beep SOS in Morse Code (dit-dit-dit dah-dah-dah dit-dit-dit or ...---...) after the *SOS Timer* expires. The *SOS Timer* is automatically set to twice the value of the *Flight Timer*

##Setting the Flight Timer
It's easy to configure the *Flight Timer*. 

1. Hold down the pushbutton switch while powering on
2. The umlma will beep out the current Flight Timer (see below)
  * If the current *Flight Timer* is what you want, release the button
  * Or, to increment the timer, keep holding the button
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
The umlma is designed with the optimum tradeoffs between weight, size, and battery life. The umlma is designed to conserve as much battery power as possible to be devoted to recovery. A new battery should continue beeping for almost 2

###How Loud Is It?
Because of weight and size constraints, the umlma uses a CR1225 battery and 
small, efficient buzzer which delivers 80dBA @ 10cm.  As a result, there is a
limit to how close you have to be to the umlma to hear it beeping.  Consider
getting help from a younger person with better hearing. :)
  
##Beeps
The umlma issues one of several beeps in morse code as follows
where . is dit (a short tone) and - is dah (a long tone):

 * W    .--          Warning time elapsed; time to land and recharge?
 * SOS  ... --- ...  On power up: dying CR1225 battery. While flying: SOS time elapsed; is your aircraft lost? Listen for this.
 * OK   --- -.-      CR1225 battery voltage is ok
 
Time Periods
------------
You can order the umlma with the following time periods pre-programmed:
  * Warn after 5 minutes; SOS after 10 minutes
  * Warn after 10 minutes; SOS after 20 minutes
  * Warn after 15 minutes; SOS after 30 minutes

Changing the time period requires reprogramming the device. See below.

Reconfiguring
-------------
To reconfigure the timeout periods, you'll need to
  * Grab the source code
  * Install AVR Studio 4 (Windows only)
  * Edit the config.h and change the time values
  * Recompile
  * Program MCU flash with an AVR programmer and pogo pin adapter with VCC connected to 5V
  * Contact me at http://www.bot-thoughts.com for more help
  
Calibration
-----------
The umlma is calibrated for the low voltage threshold at the 'factory'.
The firmware performs auto-calibration.
  * Grab the source code
  * Install AVR Studio 4 (Windows only)
  * Recompile
  * Remove the CR1225 battery
  * Use an AVR programmer with pogo pin adapter and VCC connected to a 5.0V source
  * Program the eeprom (using the lma.eep file)
  * After it resets, VCC=5V; the umlma will compute 2.5V threshold and write it to eeprom.
  * Contact me at http://www.bot-thoughts.com for more help
