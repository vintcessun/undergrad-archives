## Dinosaur-Game-Recreation

Recreated Google's Dinosaur Game using SFML.

![](https://media.giphy.com/media/UwXWVbt9wvlSM4P5FV/giphy.gif)

Link to game and more details <b>(please click!)</b>  
<b>https://happyteam.itch.io/dinosaur-game-remake</b>
  
Gameplay of the recreated game  
https://youtu.be/x4zw4AwXctA

## Features
- UI interface (Animated scoreboard, highscore, start/end screen)
- Object Pooling Design Pattern (Clouds, Cactus)
- Sprite Animation (Dinosaur, Scoreboard)
- UV texture manipulation from Sprite Sheet (Scoreboard)

## Build

If you did want to build the application on your own device you would need to install SFML-2.5.1 on your system.
Steps can be found on the SFML website.
You would also need to reroute the make file to the path with the SFML library.

Once a successful build is done with the make command,
you will also need to ensure the openal32.dll library is in the same filepath as the executable.
The openal32 library cannot be statically linked unlike the other .dll files that are in the SFML bin.

#### Created with SFML.
