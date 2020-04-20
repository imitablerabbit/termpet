# Terminal Tamagotchi

## Todo

- Read in general config file which describes the overall setup of the game.
    - Create a complete default config file.
- Start menu to create a new save file, load and existing save file, or edit global options.
- Main game play UI.
    - Tamagotchi ascii art window
    - User control menu
        - Maybe need some method of keyboard interrupt for sleep. I remember seeing some half sleep function that could be used with this type of UI.
        - Maybe just create proper time based game loop which splits user input and game updates in same loop.
    - Stats window
- Man page documentation

## Changelog

- Implement user control functions.
    - Feed pet
    - Play with pet
    - Give pet medicine
- Read in general config file which describes the overall setup of the game.
    - Define API in header file.
    - Write C code to read in config file and return global config struct.
- Implement save file system so pet progress can be retrieved at a later date.
