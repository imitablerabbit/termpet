#include "termpet.h"
#include "termpet_ui.h"

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

char status_line[100];

void print_pet(Pet *pet) {
    printf("\e[1;1H\e[2J");
    printf("Name: %s\n", pet->name);
    printf("Age: %d\n", pet->age);
    printf("Health: %d/%d\n", pet->health, pet->max_health);
    printf("Hunger: %f\n", pet->current_hunger);
    printf("Happiness: %f\n", pet->current_happiness);
    printf("Is Sick: %d\n", pet->is_sick);
    printf("Is Dead: %d\n", pet->is_dead);
    printf("\nStatus: %s\n", status_line);
}

int main() {
    Config *config;
    char *config_file = "/home/imitablerabbit/.termpet/config";

    char *pet_file = "my_pet";
    Pet *pet;

    int menu_option;

    int ch;

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
            if (access(pet_file, R_OK) == 0) {
                pet = load_pet(pet_file);
                break;
            } else {
                printf("Error: Unable to load save %s\n", pet_file);
                free_config(config);
                exit(1);
            }
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

    if (save_pet(pet, pet_file) < 0) {
        printf("Error: Unable to save pet to %s\n", pet_file);
    }

EXIT:
    free_pet(pet);
    free_config(config);

    return 0;
}
