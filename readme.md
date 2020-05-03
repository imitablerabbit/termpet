# Termpet

## Todo

- Create main game screen.
    - Pet art window.
    - Pet interaction menu.
    - Pet stats window.
- Secondary menu options.
    - Load pet form/menu
- Read in general config file which describes the overall setup of the game.
    - Create a complete default config file.
    - Config option to load save files from directory.
    - Config option for directory to find the pet art.
    - Config option to find the pet personality types.
- Create some pet art
    - Simple smiley face
- Design personality file system.
- Create some personality files. These will give base values for the pets.
- Main game play UI.
    - Termpet ascii art window
    - Stats window
- Man page documentation

## Changelog

- Secondary menu options.
    - New pet form
- Start menu to create a new save file, load and existing save file, or edit global options.
- Implement user control functions.
    - Feed pet
    - Play with pet
    - Give pet medicine
- Read in general config file which describes the overall setup of the game.
    - Define API in header file.
    - Write C code to read in config file and return global config struct.
- Implement save file system so pet progress can be retrieved at a later date.
