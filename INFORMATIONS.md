# Informations on the whole topic 

---


## Screen Resolution and Aspect Ratio

**_Screen Resolution_** is the number of pixels on a display. ex: 1920x1080 (1080p), 1280x720 (720p)

**_Aspect Ratio_** is the proportional shape of the screen (it's defined by comparing the width to the height). ex: 4:3 16:9 16:10


Other examples:
- 4:3 aspect ratio with a screen resolution of 640x480
- 16:10 - 1280x800
- 16:9 - 256×144


---

## VGA mode 13h 

- 320x200 pixels
- 256 colors
- Linear framebuffer (mapped at 0xA000)
- Used in MS-DOS

The 2D grid would actually be a frame buffer.

```c
uint32_t framebuffer[320 * 200];
```
But is a simple array of 64000 unsigned integers.

In the past something like:
```c
put_pixel(30,60,0xFF0000);
```
Where in `0xFF0000`, `FF` is Red , `00` is Green, `00` is Blue. (need to see more about those 256 available colors)
30 -> x position
60 -> y position

```
Accessing VGA pixels in the old MS-DOS days was easily done via mapped memory addresses.
```
The start is being mapped at `0xA0000`


However we cannot today write directly to the framebuffer, because is being protected by the operating system.

I searched more about this protection:

In the MS-DOS era, the processors ran in **_Real Mode_**, so software had absolute unrestricted authority over the hardware. Modern OSs run in Protected Mode.
- Every app is isolated in tis own private "virtual" memory space
- The memory addresses the code interacts with are not actual locations on the RAM, they are virtual addresses mapped by the OS
- If a program attempts access the graphics card's phisical memory via a hardcoded address, the CPU's Memory Management Unit (MMU) will flag as an illegal operation and the OS will terminate the program with Segmentation Fault. (there are still things to cover about this)

In the modern systems, the primary hardware framebuffer lives in the dedicated Video RAM (VRAM) of the graphics card (unless a integrated graphics card is being used, which reserves a dedicated, locked piece of system RAM).

Modern systems use a Compositing Window Manager (like the Desktop Window Manager in Windows or Wayland/X11 in Linux)
- the OS gives the application a virtual, off-screen framebuffer
- it draws the pixels into the private memory block
- 60 times a second (fps) (or more), the OS collects these framebuffer details from every running application
- the OS calculated the window posisiton, overlapping rules, transparency and then handles a single write to the hardware framebuffer

The PCIe Bus Bottleneck. Its slow writing pixels one-by-one from the CPU across the motherboard. Modern GPUs are designed to be largely independent. Instead of doing this, modern software uses APIs (DirectX, Vulkan, OpenGL, Metal) to send the instruction and bulk data (like 3D models and textures) to the GPU. Then the GPU calculates using parallel processing power the framebuffer.



Many things today are being protected, thus we have to do other stuff to implement something.



If we want to write something on a screen on a windows machine we would use the available windows libraries from microsoft, but we won't be able to use the exact same code on a linux machine.

These are called **_Native OS APIs/Protocols_**:
- Win32 (Windows ; C ; API)
- Cocoa (MacOS  ; Objective-C/Swift ; API/Framework)
- X11/Wayland (Linux ; these are more a protocol) 

A popular library that translates the code into the other OSs is SDL

When using a library like SDL, upon compiling, it automatically translates the commans into Win32 calls (or the other APis).

SDL, Simple DirectMedia Layer, so it is a layer that stands between the developer code and the OS APIs .

Other libraries for cross-platform are: GLFW, SFML, Allegro, Raylib etc. 

In the past I've personally heard about SFML and Raylib which were really at what they were meant for.


---

## Overview of SDL
- window -> the whole square (window) of the application
- renderer -> the place that render things (the whole windows, without the top bar)
- texture 
- framebuffer

---

## Making SDL3 work

I've followed this tutorial [Setting Up SDL3 w/ Windows, VS Code, and GCC](https://www.youtube.com/watch?v=ZY3jhIQpqjA) by Christopher Medina to make SDL3 work with my current compiler. The tutorial explains everything, from the mingw64 package and what comes with it all the way to running the program.

Brief details for windows:
- get the gcc compiler from [w64devkit](https://www.mingw-w64.org/downloads/#w64devkit) -> to github releases page [w64devkit/releases](https://github.com/skeeto/w64devkit/releases) -> download the .exe (which is just an executable that extracts the needs) -> put the `bin` directory in the environment table path and the compiler part is done
- download the SDL3 latest [release](https://github.com/libsdl-org/SDL/releases/) from github -> unzip 
- when compiling a file use an absolute (or relative) path to the includes and libraries of the SDL3:

```sh
gcc -o executables/main.exe sources/main.c -I "E:\...\sdl\SDL3-devel-3.4.16-mingw\SDL3-3.4.16\x86_64-w64-mingw32\include" -L "E:\...\sdl\SDL3-devel-3.4.16-mingw\SDL3-3.4.16\x86_64-w64-mingw32\lib" -lSDL3 
```
- when running the output executable, make sure that the `.dll` file from the SDL download is in the same location as the executable (dynamic link)

Extra:

In vscode, to link the library to the text editor, go to `.vscode/c_cpp_properties.json` and in the include section put the include location.
```
            "includePath": [
                "${workspaceFolder}/**",
                "E:\\...\\sdl\\SDL3-devel-3.4.16-mingw\\SDL3-3.4.16\\x86_64-w64-mingw32\\include"
            ],
```


---


## Software Rendering vs Hardware Rendering

In sofware rendering, the CPU is responsible for all the calculations related to 3D geometry, lighting, texturing and final color of every pixel on the screen.

In hardware rendering, the GPU is being delegated to do all the graphical calculations. The GPU cores are designed to do parallel matrix mathematics, which imply lighting, color and position of many pixels simultaneously.



