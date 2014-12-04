# Ultra Micro Lost Model Alarm User Guide

Lose your aircraft? Don't panic; wait for the beep. After the SOS time
elapses, the Ultra-Micro Lost Model Alarm (umlma) will beep SOS every 10
seconds for many hours until you find your missing aircraft.

Also, don't run your aircraft battery too low. The umlma reminds you to land
and recharge by beeping W in Morse code after the warning time elapses.

Features
--------
  * Ultra light at 4g
  * Powered by compact CR1225 battery
  * Low lma battery warning
  * If lma battery is ok
  * Efficient 1mA buzzer delivers 80dbA @ 10cm
  * After warning time elapses, beeps W in Morse code every 10 seconds
  * After the SOS time elapses, beeps SOS in Morse code every 10 seconds
  
Powering Up
-----------
  * Install a CR1225 battery if one isn't installed yet
  * Short the SWITCH pin headers with supplied shorting block
  * If the umlma beeps...
    * SOS (... --- ...), get a new CR1225 battery.
    * OK (--- -.-), strap it on the aircraft and fly
  
Warning
-------
The umlma will beep W (.--) every 10 seconds after the Warning time has elapsed
until the SOS time has elapsed.

SOS
---
After the SOS time elapses, the umlma will start beeping SOS (... --- ...) and
continue to do so until the battery dies or you power off the device.

Ideally, you'd land and recharge your aircraft before the SOS time elapses.
So, if you lose your aircraft, listen for the W or SOS beep.

Recovery
--------
Because of weight and size constraints, the umlma uses a CR1225 battery and 
small, efficient buzzer which delivers 80dBA @ 10cm.  As a result, there is a
limit to how close you have to be to the umlma to hear it beeping.  Consider
getting help from a younger person with better hearing. :)
  
Beeps
-----
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
