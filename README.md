# About this projet
## INFORMATION
This project is a university project created to discover C++ and the SFML library on a more or less proffesionnel level. It was made in a two months time so it isn't yet finished.<br />
This project is still under developement, and will be continued even after the project dead line at a slower path.

# Dependencies
SFML -> get it here : https://www.sfml-dev.org/download/sfml/2.5.1/

# HOW TO RUN THE GAME 
## LINUX
1) Use the command : make, to compile<br />
2) To create the map use the command : ./carRacing edit<br />
REFER TO THE "HOW TO EDIT THE MAP" THAT EXPLAINS HOW TO EDIT THE MAP.<br />
3) Run the game normally using the command : ./carRacing<br />
PS: You don't need to compile one more time after editing the map, just once at the start is good enough.<br />
## WHAT'S NEEDED TO RUN THE GAME
# Linux
On debian systems : sudo apt-get install libsfml-dev<br/>
On other systems, it will much likely be something similar.
# Windows
The easiest way to do that is using chocolatey https://chocolatey.org/
choco install make
choco install msys2
choco install mingw --version 7.3.0
Download SFML from https://www.sfml-dev.org/download/sfml/2.5.1/
Put includes from SFML into mingw-include folder
Put libs from SFML into mingw-lib folder
Put dlls from SFML into mingw-bin folder

## WINDOWS 
1) Compile the program using the file : compiler.bat<br />
2) To edit the map run (from cmd) : carRacing.exe edit<br />
REFER TO THE "HOW TO EDIT THE MAP" THAT EXPLAINS HOW TO EDIT THE MAP.<br />
3) Run the game using the file : carRacing.exe

# HOW TO EDIT THE MAP
IMP : When the map editor is open, you MUST place the first block of the road at the top left of the screen, as this is where the car spawns at the start!<br />
1) Click on the left button of your mouse to change the two forms of the map either straight blocks on corner blocks.<br />
2) Click on the right button of your mouse to change the angle of each chosen block.<br />
3) Click on "spacebar" to place it when the cursor is inside the top left corner.<br />
4) You can move the screen afterwards using the right/left/up/down key to place the other blocks anywhere on the map.<br />
5) When you finish creating the map click on esc to close.<br />
PS: Please create a logic map.
