# RayTracingWithPBR
Simple implementation of Ray Tracing with physically based rendering, where you can rotate the camera to select the desired angle.
All calculations are performed on the processor.

# Technical information
Platform: Windows

Used C++ Version: C++17

Used libraries: STL, WinAPI

Resolution: 1280x720

Number of reflections: 2

Maximum number of rays at the first reflection: 500

Render Time (8 logical cores, 3.0GHz): 110 seconds

# Screenshots
### Rendered frame with Blinn-Phong lighting on reflected objects:
![image](https://github.com/NiverMK/RayTracingWithPBR/blob/master/Screenshots/screenshot_1.png?raw=true)
![image](https://github.com/NiverMK/RayTracingWithPBR/blob/master/Screenshots/screenshot_2.png?raw=true)

# Solution to the white noise problem
### Let object B be reflected in object A. Then the new roughness of object B, depending on the roughness of object A, can be calculated by the formula
![image](https://github.com/NiverMK/RayTracingWithPBR/blob/master/Screenshots/screenshot_3.png?raw=true)
### Rendered frame with white noise
![image](https://github.com/NiverMK/RayTracingWithPBR/blob/master/Screenshots/screenshot_4.png?raw=true)
### Rendered frame without white noise
![image](https://github.com/NiverMK/RayTracingWithPBR/blob/master/Screenshots/screenshot_5.png?raw=true)
