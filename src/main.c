#include "tamagotchi.h"

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
    char *config_file = "/home/imitablerabbit/.tamagotchi/config";
    Pet *pet;
    char *pet_file = "my_pet";
    struct timespec sleep_time, rem;

    srand(time(NULL));

    if ((config = load_config(config_file)) < 0) {
        printf("Error: unable to load config file\n");   
        exit(1);
    }

    if (access(pet_file, R_OK) == 0) {
        pet = load_pet(pet_file);
    } else {
        pet = create_pet("Matthew");
    }

    sleep_time.tv_nsec = 20000000L;
    while (1) {
        print_pet(pet);
        update_pet(pet);
        nanosleep(&sleep_time, &rem);
        
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

    free_pet(pet);
    free_config(config);

    return 0;
}
