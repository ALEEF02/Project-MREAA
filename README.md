# Project-MREAA

**M**otorized  
**R**otating  
**E**levation and  
**A**zimuth  
**A**ntenna  

I have just finished the build and am working on a guide! Stay tuned!

### Update July 10th, 2020
I am still working on this project, but 3 different people which I have asked to 3D print my main ring gear have fallen through (all for different reasons). I am currently working on getting my own 3D printer to print the part myself. Hopefully this project should be finished by August.

### Update October 2nd, 2020
Just about done! Need to print one more gear for the azimuth motor, which I've had to reprint multiple times due to mis-sizing on the internal hole. I am very behind schedule due to reopening of schools and my part-time job. Finished version should be (hopefully) published by month end. :D

### Update October 24th, 2020
I have finished the antenna! With this commit, I am uploading the code for the main and slave Arduinos as well as a setup test for the BNO055. Pictures, videos, parts list, and guide to come shortly!

### Update March 7th, 2021
With this update comes a preliminary guide with most of the critical information. It is *likely* I will add to it in the future.

# Full Guide

Here's a full instruction set on how to build your own antenna and how to wire it

## Parts
* 2x Arduino UNO
* 2x Stepper Motors
  * We used [these 2A motors](https://www.amazon.com/STEPPERONLINE-Stepper-Bipolar-Connector-compatible/dp/B00PNEQKC0)
* 2x Stepper Motor Drivers
  * We used [these 9-40V drivers](https://www.amazon.com/KNACRO-%E3%80%90Upgraded-Version%E3%80%91-Segments-Suitable/dp/B073NZBDNH)
* 1x Orientation device
  * Must have gyroscope and compass
  * We used the [BNO055 from Adafruit](https://www.adafruit.com/product/2472), which the code is written for
* Batteries to power the Stepper Motor Drivers
  * Must have enough voltage for both drivers
    * In our case, that's 2x9V = 18V
  * We used 3x [7.4V 610mAh LIPOs](https://www.amazon.com/610mAh-Battery-Hubsan-Quadcopter-Charging/dp/B082CVWS2W) we had lying around, wired in series
* Wiring
  * Jumper Wires
  * Longer Wire
    * Enough to where it can wrap several times while the antenna moves
	
#### Optional - the following can be changed to fit the needs of the build
*Ex: We modified the build afterwards to mount a camera for panorama shots*
<img src="/media/camera mount.jpg" width="300" alt="Camera Mount">
* 3D printer (gears)
  * *Both gears are provided in .stl format and can be found [here](https://github.com/ALEEF02/Project-MREAA/tree/master/3D%20Models)*
* 1/2 inch aluminum square tube (boom)
* 3x copper pipes (director, driven element, and reflector)
  * Lengths depend on frequency of the antenna 
* U-bolts
* Sliding door wheels
    * Used to move the antenna on the base
	<img src="/media/wheels.JPG" width="300" alt="Sliding Wheels">
* Various other parts as needed...
	
## Steps

### Yagi Antenna
Our goal was to receive NOAA weather satellite signals at 137MHz. We sought to make a 3 element Yagi Uda antenna.

We began with rough calculations provided by [online calculators](https://www.changpuak.ch/electronics/yagi_uda_antenna_DL6WU.php). With these numbers we got a piece of 1/2 inch aluminum square tube as the boom. Three copper pipes (one director, one driven element, and one reflector), about 1/2 inch in diameter and cut to the specified lengths, were mounted to the boom at the calculated measurements.

<img src="/media/wheels.JPG" width="300" alt="Sliding Wheels">
<img src="/media/boom holes.JPG" width="300" alt="Sliding Wheels">

Mounting of the elements was done with U-bolts through the boom. The positive and negative leads were soldered to a length of coaxial cable and the joint was housed in a PVC junction box.

<img src="/media/ubolt.JPG" width="300" alt="Using ubolts">
<img src="/media/copper tubing to boom.JPG" width="300" alt="Attaching copper tubing to the boom">
<img src="/media/coax.JPG" width="300" alt="Coax Cabling">
<img src="/media/coax connect.JPG" width="300" alt="Coax cabling connected to the copper tubing">

### Main Assembly
The whole antenna assembly was mounted on an old skateboard truck with the bearings pressed into two supports that held the antenna off the ground.

<img src="/media/skateboard.JPG" width="300" alt="Sliding Wheels">
<img src="/media/mount.JPG" width="300" alt="Full mounting solution">

Vertical movement was accomplished by means of a screw that was allowed to change its angle to accommodate for the changing position of the stationary nut attached to the boom.

<img src="/media/finished.jpg" width="300" alt="Finished, no wiring">

### Wiring
Refer to the following diagram for wiring the antenna. A breadboard as displayed is not required, as you can solder the wires as well for a more permanent build.
![Wiring Diagram](/media/wiring.png)

## Extra Photos and Videos
Early wiring and testing of the stepper motor using a compass

<img src="/media/early wiring 1.jpg" width="300" alt="The beginnings">

![Using a compass (video)](https://user-images.githubusercontent.com/12904259/111854651-87a7c080-88dd-11eb-8fc2-6c3c1abd6f29.mp4)

First movement to the moon, October 24th, 2020 at 8:06 PM PDT
![Movement to the Moon (video)](https://user-images.githubusercontent.com/12904259/110265670-f77a8a80-7f70-11eb-8161-bef4a66bf4ca.mp4)

Tracking NOAA-18
![Movement to NOAA-18](https://user-images.githubusercontent.com/12904259/111854695-c2115d80-88dd-11eb-92d7-0e3e7319f4ea.mp4)
