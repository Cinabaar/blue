# Particle simulation using SPH algorithm

## Presentation

Click to view full video:

http://www.youtube.com/watch?v=a44fNPBgibE

## Description

This is an implementation of the Smoothed Particle Hydrodynamics algorithm using OpenGL and OpenCL to simulate up to 
~250k particles in realtime on my average PC. The simulation implemented is the dam-break test where a fixed amount
of water is released in a container and its behaviour is examined. 

The rendering of particles is done using the impostor technique to render circles in the fragment shader instead of spheres. 
A metaball-based implementation of water is on the way, however due to lack of time I couldn't roll it out yet.

Apart from the simulation itself I built a framework (too soon to call it an engine) and extensible entity-component system. The framework itself is a playground where a plan to test algorithms and ideas in the future. So far I have implemented a set of componets handling the camera, rendering of simple objects and water simulation.

## Running

The code should compile and was tested on both Windows and Linux using cmake, gcc and mingw-x64. 

### Requirements

As the code was only tested on my computer I can only vouch for a setup identical to mine. As such, in theory a C++11
compliant compiler and an OpenCL SDK should be enough to run the simulation (I used the NVida Cuda SDK as it provides
the necessary libraries and headers).

### Instructions

On Linux:
~~~
git clone
git submodules update --init
mkdir build && cd build
cmake ..
make
cd bin
./blue
~~~

Use equivalents on Windows:
~~~
cmake -G "MingGW Makefiles" ..
mingw32-make.exe (or similar)
~~~
## Future improvements

- Implement visualisation using metaballs
- Add better lighting
