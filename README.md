# Heads up display

## Overview

This project uses an ESP32 based microcontroller and an external heads up display that can output text and graphics.

## Background

I have a collection of microcontrollers and sensors. I am working my way through the collection of equipment I have and looking at fun projects I can put together, and also expand my knowledge base. 

## What did I use?

Here’s a picture of the components I used for this project:

<img width="50%" height="50%" alt="image" src="https://github.com/user-attachments/assets/b560eba8-89e8-404f-b776-1650bc3e6780" />

Here’s a closer look at the heads-up display (HUD):

<img width="50%" height="50%" alt="image" src="https://github.com/user-attachments/assets/9d461724-cb9c-48b2-8bd7-75f5ac3a7fa6" />

Here’s the other side:

<img width="50%" height="50%" alt="image" src="https://github.com/user-attachments/assets/04cf4d68-56ac-4447-8a5b-3ba68a5b61ed" />

The majority of promotional material I have seen for this HUD shows this side of the device. However, coordinates 0,0 are the top left of the previous picture. Perhaps this side is shown because it is colorful. I have been calling the other picture the front of the device, and this picture the back.

The microcontroller can be connected from either side. The device supports chaining to an additional HUD. 

## What does it look like?

This is what the project looks like when it’s running:

<img width="50%" height="50%" alt="image" src="https://github.com/user-attachments/assets/1cdb1b2e-a923-4f19-a25b-0202e638d90c" />

The demo project creates 4 different wireframe shapes – square, triangle, pentagon, and hexagon. The shapes are rotated to form a pattern.

A progress bar at the top of the screen that shows how far the rotation sequence has advanced.

The number at the left-hand side of the screen shows the current angle used for the shape rotation.

I used a mixture of graphics and text to show the capability of the HUD.

Here's a video showing the project working:

[![short video](https://img.youtube.com/vi/Fb4MtecQfEg/0.jpg)](https://www.youtube.com/watch?v=Fb4MtecQfEg)

## Technical Overview

The project uses an Arduino Sketch to output to the HUD, although the microcontroller I chose has its own screen. The integrated screen is bypassed, if the HUD is detected. The Sketch will output to the integrated screen if the HUD is not connected.

The Sketch uses a C++ class (called shape) to hold information about each of the shapes displayed, which are a square, triangle, pentagon, and hexagon. This is a simple class that holds the X and Y coordinates of the shape and its size. Vertices (sides) are added to the class and tracked in an STL vector. The class also contains the code to rotate the coordinates of the shape and re-draw the shape on the screen.

A simple rectangle is used for the progress bar and the text is output with a printf() statement.

The HUD is monochrome (black and blue, to be exact). The brightness of the screen can be set. The brightness range is 0..255, but I have found that lower brightness settings (0 or 1) can cause the HUD to flicker a lot. 

The HUD is 128 x 64 pixels:

<img width="50%" height="50%" alt="image" src="https://github.com/user-attachments/assets/e4ef2d08-7c5d-4b86-b533-00a3686c5e4b" />

This may seem like a small viewing area, but you can pack a lot of information onto this HUD. Note that only 56 of the 64 pixels are transparent.

To accommodate the shapes, progress bar and text output, I used a sprite that used the entire real estate of the screen:

<img width="50%" height="50%" alt="image" src="https://github.com/user-attachments/assets/83c7cd6e-7787-4df1-b125-1df8dbba8093" />

Sprites provide a way of updating screens, or on this case the HUD, without causing a lot of flicker.

In this picture, you can clearly see the HUD is transparent, and the microcontroller can be seen through the HUD screen:

<img width="50%" height="50%" alt="image" src="https://github.com/user-attachments/assets/4c3baad7-a0cf-440d-8ca7-4f8cbe91add1" />

The HUD also does quite well when it is close to a natural light source:

<img width="50%" height="50%" alt="image" src="https://github.com/user-attachments/assets/43d60df7-8ca0-4d57-b8f1-740bd6b6004f" />

However, the image is not entirely visible in direct light:

<img width="50%" height="50%" alt="image" src="https://github.com/user-attachments/assets/039361c9-9ad4-4a3a-abef-9cbdf08461b0" />

## What do I need?

You will need:

1. The project uses a Core2 AWS from M5Stack.
       
2. An M5Stack Glass2 Unit (this is the HUD).
       
3. The application only uses the standard M5Stack and Core2 libraries. 
       
4. A PC with Windows, Linux, or a Mac to install the Arduino IDE which can be downloaded here https://www.arduino.cc/en/software/. 
       
5. A USB A to USB C cable to connect the PC or Mac to the Core2.
       
6. The git utility to access the GIT repository (git clone https://github.com/davygotgit/headsup.git) or visit https://github.com/davygotgit/headsup and download a ZIP file.

Information for Core2 AWS is here https://shop.m5stack.com/products/m5stack-core2-esp32-iot-development-kit-for-aws-iot-edukit.

The main M5Stack site is here https://m5stack.com/.

The M5Stack Glass2 Unit is here https://shop.m5stack.com/products/glass-2-unit-w-1-51inch-transparent-oled?srsltid=AfmBOoqzCO80W9jh2gG6eyV1Dpc4Pl4G3myF978_f8uy_T8CW7YbnpMG. The Grove cable is included. Deeper technical information and sample code is here https://docs.m5stack.com/en/unit/Glass2%20Unit,

M5Stack also have the Glass Unit, which is here https://shop.m5stack.com/products/glass-unit-w-1-51inch-transparent-oled?srsltid=AfmBOoresOsa2vgJMHQk63RaNBJA1Ama_s4hv_NGQ-YCvHcrxWr3Ihe-. Technical information and code samples are here https://docs.m5stack.com/en/unit/Glass%20Unit.

The Glass and Glass2 units share some similarities e.g. they are both 128 x 64 pixels. The Glass Unit has 2 buttons and a buzzer, but only one HUD can be controlled. The Glass2 Unit does not have the buttons or buzzer, but can support 2 HUD devices.

The Grove interface, if you are not familiar, is standard on all M5Stack devices. It’s data and connection interface. I like Grove enabled microcontrollers and sensors as cables can only be plugged in one way.

How do I install and configure the tools?

Here are some instructions for downloading and installing GIT https://github.com/git-guides/install-git.

Here are some instructions on how to download and install the Arduino IDE https://docs.arduino.cc/software/ide-v2/tutorials/getting-started/ide-v2-downloading-and-installing/.

M5Stack have an excellent quick start guide here https://docs.m5stack.com/en/arduino/m5core2/program. I don’t recall having to install any driver on my version of Ubuntu 24.04.2 LTS. It’s possible this is already included in the kernel. On Linux you must add your account to the dialout group by running the following bash command:

	sudo usermod -a -G dialout <your_account>

For example, if your user account is fantasticfred:

	sudo usermod -a -G dialout  fantasticfred

You must log out your current session and log back in again for this change to become active.

For Linux systems, I would first see if your Arduino IDE can see the Core2 device before attempting to install any drivers.

You know you are connected to the Core2 if you see something similar to the following status (bottom right) in the Arduino IDE:

<img width="519" height="67" alt="image" src="https://github.com/user-attachments/assets/38677afb-bad5-4221-92e6-b30cb8227f21" />

## How do I build and install the application?

You need to download the code from the GIT repository. This can be done by visiting https://github.com/davygotgit/headsup and downloading, and then extracting, a ZIP file or by running the following terminal command from bash, a Windows Command Prompt or any suitable GIT access tool:

	git clone https://github.com/davygotgit/headsup.git

There are a couple of options to build the application for the first time. Option 1 is:

1. Start the Arduino IDE.
2. Create a new project using the File -> New Sketch menu option.
3. Save the project using the name headsup by using the File -> Save menu option.
4. Open the src/headsup .ino file, from repository, using another editor, and copy/paste the contents over the skeleton project.
       
Option 2 is:
       
1. Start the Arduino IDE.
2. Create a new project using the File -> New Sketch menu option.
3. Save the project using the name headsup by using the File -> Save menu option.
4. Use the Sketch -> Show Sketch Folder menu option to get the location of the project (Sketch location). This will be similar to Home/Arduino/ headsup on Linux.
5. Close the IDE.
6. Copy the headsup.ino file from the src subdirectory of the repository to the Sketch location.
7. Start the Arduino IDE and load the headsup project.
       
Once you have the initial project saved, you can just load it from File -> Open Recent menu option.

With the Sketch loaded, connect the Core2 using the USB A to USB C cable,. Ensure the M5Core2 board is selected and the USB port shows a connected status. Press the Upload button on the toolbar. The Sketch will be compiled and transferred to the Core2. The application will start after the transfer completes.

## Do you have any tips for using M5Stack microcontrollers,  sensors or units?

This is a tip for using M5Stack Units. The Glass2 has technical information and a link to an Ardunio Sketch sample here https://docs.m5stack.com/en/unit/Glass2%20Unit. M5Stack have short, concise code snippets for all their sensors and units.

I tend to use the unit, with a microcontroller, with one of these Sketches first. This tells me that the unit is working as expected.

This can reduce debugging time when you start to add more sensors or units, and your own code. As you know the units/sensors work independently, any issues have to be with your code or the interaction of multiple sensors.

## Were there any challenges creating this project?

I ran into two challenges with this project.

My initial thought for this project was a Cheshire Cat like face that slowly appeared on the HUD. I was able to draw the face using the graphics components provided by the M5Stack library. I thought I could set the screen brightness down very low and slowly ramp it up to display the image. However, the image was pretty clear at screen brightness 5 (the range is 0..255). As the display is monochrome, I was not able to use color shades to reveal the image, so I had to think of a different demo.

The other challenge was with sprite size. For one of the early demo I ideas I had, I created a sprite that was ~50% of the screen. For some reason, this sprite could not be displayed. Small sprites were fine. This particular sprite, once over a certain size, would not display. Changing it to a full screen sprite seemed to work. I am not sure what causes the issue with certain sized sprites.
