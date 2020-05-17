#include "termpet.h"
#include "termpet_ui.h"

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#include <ncurses.h>

int main() {
    Config *config;
    char *config_file = "/home/imitablerabbit/.termpet/config";
    Pet *pet = NULL;
    int menu_option;
    int ch;
    char *save_path;
    Save *save = NULL;

    srand(time(NULL));

    if ((config = load_config(config_file)) == NULL) {
        printf("Error: unable to load config file\n");
        exit(1);
    }

    init_ui();
    menu_option = main_menu();
    switch (menu_option) {
        case MAIN_MENU_OPTION_NEW:
            menu_option = new_game_menu(&pet);
            if (menu_option == NEW_GAME_MENU_OPTION_EXIT
                    || menu_option == -1) {
                free_config(config);
                exit(0);
            }
            break;
        case MAIN_MENU_OPTION_LOAD:
            menu_option = load_game_menu(config, &save);
            if (menu_option == LOAD_GAME_MENU_OPTION_EXIT
                    || menu_option == -1) {
                free_config(config);
                exit(0);
            }
            copy_pet(&pet, save->pet);
            free_save(save);
            break;
        case MAIN_MENU_OPTION_EXIT:
        default:
            free_config(config);
            exit(0);
    }

    timeout(10);
    game_loop(config, pet);

    save_path = save_file_path(config->save_dir, pet->name);
    save = save_pet(pet, save_path);
    free(save_path);
    if (save == NULL) {
        printf("Error: Unable to save pet to %s\n", save_path);
        goto EXIT;
    }
    free_save(save);

EXIT:
    free_pet(pet);
    free_config(config);
    endwin();
    return 0;
}
