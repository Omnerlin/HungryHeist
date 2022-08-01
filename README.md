# HungryHeist
A small game project created using SFML, inspired by my cat Jynx always trying to sneak that food.

Other libraries used:
- [Perlin Noise](https://github.com/Reputeless/PerlinNoise) (Made by Reputeless)
- [json](https://github.com/nlohmann/json) (By nlohmann)

### Dependencies
SFML 2.5.1 (May work with older versions)

## Build Instructions

NOTE: This project was built using a few C++20 features, namely reverse_view for convenience.
This may cause issues when building for Mac or Linux if your comiler isn't up-to-date.
reverse_view usage can be swapped out for the older reverse_iterator approach if needed.

### Windows (Visual Studio)

1. Place SFML 2.5.1 root directory in ```deps``` folder 
2. Open the project solution
3. Build an x64 Release. A folder under ```x64 > Release``` should be created.

### Linux (Makefile)
Run "make" in the project's root directory. A ```bin``` folder should be created when this is done.

### Mac OS (Makefile)
Follow the build step provided for Linux.

### After Building
Copy over the ```assets``` folder, ```config``` folder, and ```Credits.txt``` file to the build directory so that references are found as expected.

## Known Bugs
- Sometimes the GUI doesn't always resize when it should when maximizing, restoring, and changing to/from fullscreen
- Sometimes the cat's head doesn't position itself correctly when getting caught.. usually only seems to happen the first time which is odd.
