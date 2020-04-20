#ifndef TAMAGOTCHI_UI_H
#define TAMAGOTCHI_UI_H

#include <ncurses.h>
#include <form.h>
#include <menu.h>

#define MAIN_MENU_OPTION_NEW 0
#define MAIN_MENU_OPTION_LOAD 1
#define MAIN_MENU_OPTION_EXIT 2

#define MAIN_MENU_ITEM_COUNT 3

static const char *choices[] = {
    "New Pet",
    "Load Pet",
    "Exit",
};

static const char *descriptions[] = {
    "Create a new pet",
    "Load an existing pet save file",
    "Exit the game",
};

static const int item_data[] = {
    MAIN_MENU_OPTION_NEW,
    MAIN_MENU_OPTION_LOAD,
    MAIN_MENU_OPTION_EXIT,
};


// Initialises any UI elements ready for displaying to the user.
int init_ui();

// Starts the ncurses main menu for the tamagotchi. This will allow the user to
// create a new pet or load an existing save file. This function will block
// until the user no longer wants to play.
// Returns -1 if there was an error playing the game. Otherwise it will return
// the menu option that was selected. This can be either MAIN_MENU_OPTION_NEW,
// MAIN_MENU_OPTION_LOAD or MAIN_MENU_OPTION_EXIT.
int main_menu();

#endif
