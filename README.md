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

Render Time (8 logical cores, 3.0GHz): 80 seconds

# Screenshots
### Rendered frame with diffuse lighting on reflected objects:
![image](https://github.com/NiverMK/RayTracingWithPBR/blob/master/Screenshots/screenshot_1.png?raw=true)
![image](https://github.com/NiverMK/RayTracingWithPBR/blob/master/Screenshots/screenshot_2.png?raw=true)

### Rendered frame with Blinn-Phong lighting on reflected objects:
![image](https://github.com/NiverMK/RayTracingWithPBR/blob/master/Screenshots/screenshot_3.png?raw=true)

It looks better, but there is a problem with white noise from smoothness objects in reflections.
![image](https://github.com/NiverMK/RayTracingWithPBR/blob/master/Screenshots/screenshot_4.png?raw=true)
