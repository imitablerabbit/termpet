# Termpet

## Building

To build `termpet` you will need the following pre-installed on your machine.

- `gcc` C compiler.
    - The code is written in ANSI C so any C compiler would work but you will
    need to change the settings in the makefile/compile the game manually.
- `ncurses` C library.
- `ncurses form` C library that extends `ncurses` with form functionality.
- `ncurses menu` C library that extends `ncurses` with menu functionality.

Once you have the pre-requisites installed, then you can compile `termpet`
using the Makefile.

```bash
make compile
```

## Playing the Game

Currently the main gameplay loop has not been fully implmented. Instead the
game will run the user actions automatically, e.g. it will play itself. In the
future there will be an action menu where you can choose to feed, play, or give
medicine to the pet.

### Main Menu

When you first open the game you will be greeted with the main menu. From here
you can decide whether you would like to start a new game, load a previous save
file, or exit the game.

### New Game Menu

The new game menu lets you create a new pet to play with. It allows you to edit
the pet attributes displayed in the input form before you start playing.

### Load Game Menu

The load game menu displays a list of all the save files that exist in the
saves directory. Selecting a save file from this list will load the pet so you
can continue from where you left off.

### Gameplay

The current version of termpet will display a contantly updating list of the
pets stats to you. In this version the game will play itself by running user
actions. You can save and quit the game byh pressing 'q'.

## Config

There are a few config files that `termpet` relies on. Firstly there is the
global config file. The global config is located at `~/.termpet/config`.

The pet save files contain all of the save game data for each new pet created.
These are located in `~/.termpet/saves/` where each file in this directory
represents a pet save file.
