# Air Quality Monitor
Air Quality Monitor, using the SCD40 sensor and having a 2.8" Touchscreen, controlled by an Arduino Nano.

## Purpose of the project
This is my first Project that i really want to "finish". I often had ideas for Projects and Prototyped them on Breadboards but never continued through with them. For the Air Quality Monitor i really wanted to go further and learn the steps involved to "complete" a Project. In my case the steps are the following:
* Choosing the components and Prototype them
* Writing the code with all the functions i want
* Designing a PCB to mount all the Components to it
* Designing a Case and let it 3D Print (because i dont have a printer)

These steps are obviously nothing special and are the common steps for every Project, but i never did it myself so i wanted to learn to do this all by myself. This is also my first Repo on GitHub so i'm also learning how to use it aswell.

## Hardware
* As Microcontroller i first started with the Arduino Uno R4 Wifi but as my goal was to complete this Project, i switched to the Arduino Nano Every to solder it to the PCB later.
* As the Sensor i picked the SCD40, more precise the [Breakoutboard from Adafruit](https://www.adafruit.com/product/5187) containing the Sensor. The Breakoutboard makes it super easy to connect the Sensor to any Microcontroller.
* I also used 3 [INA260 Current sensing Breakoutboards from Adafruit](https://www.adafruit.com/product/4226) for sensing the incoming, 5V and 3.3V Voltages and Currents. It's probally a bit overkill for a small Project like this but i like the data and also learning to use them for later Projects.
* The Air Quality Monitor also has a [2.8" Touchscreen from AZ-Delivery](https://www.az-delivery.de/en/products/2-8-zoll-lcd-tft-touch-display) used for Displaying the Data. The Touchscreen is used to switch the Darkmode or switch between the menus. The Screen also has a built-in SD-Card Slot, which i use to display small images for a more pleasing user experience.
* The Screen runs only on 3.3V while the Arduino runs on 5V so i also picked up a [Level-Shifter from AZ-Delivery](https://www.az-delivery.de/en/products/logiklevel-wandler-3-3v-5v?_pos=1&_psq=TX&_ss=e&_v=1.0) for shifting the SPI and other Data Signals to and from the Display.
* I created the schematic and layout for the PCB in EasyEDA, which was also my first time doing that aswell.
* The case for this Project is designed in Onshape, which is my first time designing something from scratch in a CAD Program. The Case should have the USB Port, Power Port and SD-Card Slot exposed, and have the SCD40 Sensor somewhat exposed so it can measure the Air.

## Software
For the Software i wanted to create something that uses all the capabilities of the Hardware. In the end, the Software should have the following features:
* Displaying the Temperature, Humidity and CO2 Level from the SCD40 to the Display.
* Displaying corresponding Images for the measured values and maybe having changing Images to better visulize the values.
* Having a toggle switch between a dark and bright mode.
* Having a Sensor Page where all the values from each Sensor is displayed, for a more complete overview of the System.
* Having a Debug Page where the Status off the Sensors is displayed and checked.
* Having a feature to alert when for example the incoming Voltage is too high/low.
