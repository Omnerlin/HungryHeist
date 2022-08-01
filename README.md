# HungryHeist
A small game project created using SFML, inspired by my cat Jynx always trying to sneak that food.

Other libraries used:
- [Perlin Noise](https://github.com/Reputeless/PerlinNoise) (Made by Reputeless)
- [json](https://github.com/nlohmann/json) (By nlohmann)

### Dependencies
[SFML 2.5.1](https://www.sfml-dev.org/download.php) (May work with older versions)

## Build Instructions  

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

## Attributions
UI SFX set by Kenney Vleugels (www.kenney.nl)

jump.wav - used from [12 Player Movement SFX](https://opengameart.org/content/12-player-movement-sfx) by Leohpaz on OpenGameArt.org  
[(CC BY 4.0)](https://creativecommons.org/licenses/by/4.0/legalcode)

recordScratch.wav sound effect created by Paul Virostek from Airborne Sound: [Variety 2 pack](https://soundcloud.com/airbornesound/variety-2-sound-effects-1)

gameplayMusic.wav - [Tactical Pursuit](https://opengameart.org/content/tactical-pursuit) by Matthew Pablo on OpenGameArt.org [(CC BY 3.0)](https://creativecommons.org/licenses/by/3.0/legalcode)  
This was shortened by me for better looping in the game.

forestLoop.ogg - [Loop - House In A Forest](https://opengameart.org/content/loop-house-in-a-forest) by HorrorPen on OpenGameArt.org [(CC BY 3.0)](https://creativecommons.org/licenses/by/3.0/legalcode)

preEatMusic.wav - [Insrumental Symphony (2 Tracks)](https://opengameart.org/content/instrumental-symphony-2-tracks) by Alexandr Zhelanov on OpenGameArt.org  
https://soundcloud.com/alexandr-zhelanov  
[(CC BY 3.0)](https://creativecommons.org/licenses/by/3.0/legalcode)  
