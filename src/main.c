#include "termpet.h"
#include "termpet_ui.h"

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#include <ncurses.h>

char status_line[100];

void print_pet(Pet *pet) {
    clear();
    printw("Name: %s\n", pet->name);
    printw("Age: %d\n", pet->age);
    printw("Health: %d/%d\n", pet->health, pet->max_health);
    printw("Hunger: %f\n", pet->current_hunger);
    printw("Happiness: %f\n", pet->current_happiness);
    printw("Is Sick: %d\n", pet->is_sick);
    printw("Is Dead: %d\n", pet->is_dead);
    printw("\nStatus: %s\n", status_line);
}

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
    while ((ch = getch()) != 'q') {
        print_pet(pet);
        update_pet(pet);

        if (pet->current_hunger <= 0.5) {
            feed_pet(pet);
            sprintf(status_line, "Fed pet at age %d", pet->age);
        }

        if (pet->current_happiness <= 0.5){
            play_with_pet(pet);
            sprintf(status_line, "Played with pet at age %d", pet->age);
        }

        if (pet->health <= 8){
            give_medicine_to_pet(pet);
            sprintf(status_line, "Gave medicine to pet at age %d", pet->age);
        }
    }

    save_path = save_file_path(config->save_dir, pet->name);
    save = save_pet(pet, save_path);
    if (save == NULL) {
        printf("Error: Unable to save pet to %s\n", save_path);
    }
    free_save(save);

EXIT:
    free_pet(pet);
    free_config(config);
    endwin();
    return 0;
}
