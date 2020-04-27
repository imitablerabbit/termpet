#ifndef TERMPET_UI_H
#define TERMPET_UI_H

#include "termpet.h"

#include <ncurses.h>
#include <form.h>
#include <menu.h>

#define BACKSPACE_CHAR 127
#define ENTER_CHAR 10

// Initialises any UI elements/API's ready for displaying to the user.
int init_ui();

// Initialises a menu object with some common attributes.
int init_menu(MENU *m, WINDOW *menu_window);

// Initialises a form object with some common attributes.
int init_form(FORM *f, WINDOW *form_window);

// Change the coloring of the menu so that it appears to be the activate UI
// element that the user should interact with. Active menu elements will have
// their backgrounds highlighted.
int activate_menu(MENU *m);

// Change the coloring of the menu so that it appears to be deactivated and
// would not respond to any user interaction.
int deactivate_menu(MENU *m);

// Change the coloring of the form elements so that it appears to be active
// and would take user input.
int activate_form(FORM *f);

// Change the coloring of the form elements so that it appears to be deactivated
// and unable to respond to any user interaction.
int deactivate_form(FORM *f);

#define MENU_DRIVER_EXIT_UP 1
#define MENU_DRIVER_EXIT_DOWN 2
#define MENU_DRIVER_SELECTED 3

// Handles menu interaction based on the characters pressed by the user.
//
// Returns 0 if the users action was completely contained within the menu driver
// and was successful.
//
// Returns MENU_DRIVER_EXIT_UP if the user wanted to leave
// the menu upwards, likewise MENU_DRIVER_EXIT_DOWN is when the user wanted to
// leave from the bottom. The handler is unable to so swapping between any ui
// elements, this is merely a method of informing the caller of the users
// intent.
//
// MENU_DRIVER_SELECTED is returned if the user pressed  enter on a valid
// option. This will cause the ITEM** current to be set to the menu's currently
// selected item.
//
// -1 is returned if there was an error.
int menu_handler(MENU *m, int ch, ITEM** current);

#define FORM_DRIVER_EXIT_UP 1
#define FORM_DRIVER_EXIT_DOWN 2

// Handles form interaction based on the characters pressed by the user.
//
// Returns 0 if the users action was completly contained within the form driver
// and was successful.
//
// Returns FORM_DRIVER_EXIT_UP if the user wanted to leave the form upwards,
// likewise FORM_DRIVER_EXIT_DOWN is when the user wanted to leave from the
// bottom.
//
// -1 is returned if there was an error.
int form_handler(FORM *f, int ch);

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

// Starts the ncurses main menu for the termpet. This will allow the user to
// create a new pet or load an existing save file. This function will block
// until the user no longer wants to play.
//
// Returns -1 if there was an error playing the game. Otherwise it will return
// the menu option that was selected. This can be either MAIN_MENU_OPTION_NEW,
// MAIN_MENU_OPTION_LOAD or MAIN_MENU_OPTION_EXIT.
int main_menu();

// New Game Menu

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

int new_game_menu(Pet **pet);

#endif
