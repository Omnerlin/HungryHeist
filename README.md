# HungryHeist
A small game project created using SFML, inspired by my cat Jynx always trying to sneak that food.

Other libraries used:
- [Perlin Noise](https://github.com/Reputeless/PerlinNoise) (Made by Reputeless)
- [json](https://github.com/nlohmann/json) (By nlohmann)

## Build Instructions

### Dependencies
SFML 2.5.1 (May work with older versions)

### Windows
1. Place SFML 2.5.1 in ```deps``` folder 
2. Open the project solution and build an x64 Release

### Linux
I've created a Makefile that seems to work with my local Linux setup:
1. Simply run "make" 

### Mac OS
I've 

### After Building
Copy over the ```assets``` folder, ```config``` folder, and ```Credits.txt``` file to the build directory so that references are found as expected. 

## Known Bugs
- Sometimes the GUI doesn't always resize when it should when maximizing, restoring, and changing to/from fullscreen
- Sometimes the cat's head doesn't position itself correctly when getting caught.. usually only seems to happen the first time which is odd.
