#ifndef TERMPET_UI_H
#define TERMPET_UI_H

#include <ncurses.h>
#include <form.h>
#include <menu.h>

#define TITLE_TEXT "termpet - Tamagotchi for the terminal"

#define MAIN_MENU_OPTION_NEW 0
#define MAIN_MENU_OPTION_LOAD 1
#define MAIN_MENU_OPTION_EXIT 2

#define MAIN_MENU_ITEM_COUNT 3

static const char *main_menu_choices[] = {
    "New Pet",
    "Load Pet",
    "Exit",
};

static const char *main_menu_descriptions[] = {
    "Create a new pet",
    "Load an existing pet save file",
    "Exit the game",
};

static const int main_menu_item_data[] = {
    MAIN_MENU_OPTION_NEW,
    MAIN_MENU_OPTION_LOAD,
    MAIN_MENU_OPTION_EXIT,
};


// Initialises any UI elements ready for displaying to the user.
int init_ui();

// Starts the ncurses main menu for the termpet. This will allow the user to
// create a new pet or load an existing save file. This function will block
// until the user no longer wants to play.
// Returns -1 if there was an error playing the game. Otherwise it will return
// the menu option that was selected. This can be either MAIN_MENU_OPTION_NEW,
// MAIN_MENU_OPTION_LOAD or MAIN_MENU_OPTION_EXIT.
int main_menu();

#define NEW_GAME_MENU_TEXT "termpet - New Game"

#define NEW_GAME_MENU_OPTION_START 0
#define NEW_GAME_MENU_OPTION_EXIT 1

#define NEW_GAME_MENU_ITEM_COUNT 2

static const char *new_game_menu_choices[] = {
    "Start",
    "Exit",
};

static const char *new_game_menu_descriptions[] = {
    "Start the game with the current options",
    "Exit the game",
};

static const int new_game_menu_item_data[] = {
    NEW_GAME_MENU_OPTION_START,
    NEW_GAME_MENU_OPTION_EXIT,
};

int new_game_menu(char **name);

#endif
