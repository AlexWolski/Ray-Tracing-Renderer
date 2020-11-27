# Ray Tracing Renderer

![Ray Traced Render](https://i.imgur.com/7qhgNXi.png)

- This ray tracer was built using C++ and runs on the CPU.
- It uses the Phong shading algorithm and supports reflections and soft shadows.
- The [openFrameworks](https://openframeworks.cc/) library is used for multithreading and drawing the rendered image to the screen.


# Set Up:

1. Clone or download this repository
2. Navigate to the folder:<br/>
`Ray-Tracing-Renderer\openFrameworks\apps\Homework 5 - Ray Tracing`
3. Open the **Homework 5 - Ray Tracing** solution file with Visual Studio 2017
4. Set the target architecture to **x64** and the project configuration to **Release**
5. Click **Local Windows Debugger** to compile and run the program

# To-Do:

  - Remove the dependency on openFrameworks and switch to pure OpenGL
  - Implement the rtNode class to handle matrix transforms
  - Add an acceleration structure (probably a BVH)
  - Add interactive controls for navigating the scene and toggling features
  - Add anti-aliasing (probably FXAA)
  - Move the workload to the GPU using HLSL