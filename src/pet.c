#include "tamagotchi.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

Pet *create_pet(char *name) {
    Pet *pet;
    
    pet = (Pet*)malloc(sizeof(Pet));

    pet->name = (char*)malloc(sizeof(char)*strlen(name)+1);
    strcpy(pet->name, name);
    pet->age = 0;

    pet->current_hunger=1.0;
    pet->hunger_decrease_chance=0.01;
    pet->hunger_decrease_amount=0.1;

    pet->current_happiness=1.0;
    pet->happiness_decrease_chance=0.01;
    pet->happiness_decrease_amount=0.1;

    pet->current_sickness_chance=0.001;
    pet->sickness_increase_amount=0.001;
    pet->sickness_hunger_threshold=0.4;
    pet->sickness_happiness_threshold=0.3;

    pet->health=10;
    pet->max_health=10;
    pet->health_change=1;
    pet->health_decrease_chance=0.01;

    pet->is_sick=0;
    pet->is_dead=0;

    return pet;
}

int save_pet(Pet *pet, char *filename) {
    FILE *f;

    if ((f = fopen(filename, "w")) == NULL)
        return -1;

    fprintf(f, "name" CONFIG_DELIMITER "%s\n", pet->name); 
    fprintf(f, "age" CONFIG_DELIMITER "%d\n", pet->age); 

    fprintf(f, "current_hunger" CONFIG_DELIMITER "%f\n", pet->current_hunger);
    fprintf(f, "hunger_decrease_chance" CONFIG_DELIMITER "%f\n", pet->hunger_decrease_chance);
    fprintf(f, "hunger_decrease_amount" CONFIG_DELIMITER "%f\n", pet->hunger_decrease_amount);

    fprintf(f, "current_happiness" CONFIG_DELIMITER "%f\n", pet->current_happiness);
    fprintf(f, "happiness_decrease_chance" CONFIG_DELIMITER "%f\n", pet->happiness_decrease_chance);
    fprintf(f, "happiness_decrease_amount" CONFIG_DELIMITER "%f\n", pet->happiness_decrease_amount);

    fprintf(f, "current_sickness_chance" CONFIG_DELIMITER "%f\n", pet->current_sickness_chance);
    fprintf(f, "sickness_increase_amount" CONFIG_DELIMITER "%f\n", pet->sickness_increase_amount);
    fprintf(f, "sickness_hunger_threshold" CONFIG_DELIMITER "%f\n", pet->sickness_hunger_threshold);
    fprintf(f, "sickness_happiness_threshold" CONFIG_DELIMITER "%f\n", pet->sickness_happiness_threshold);

    fprintf(f, "health" CONFIG_DELIMITER "%d\n", pet->health);
    fprintf(f, "max_health" CONFIG_DELIMITER "%d\n", pet->max_health);
    fprintf(f, "health_change" CONFIG_DELIMITER "%d\n", pet->health_change);
    fprintf(f, "health_decrease_chance" CONFIG_DELIMITER "%f\n", pet->health_decrease_chance);

    fprintf(f, "is_sick" CONFIG_DELIMITER "%d\n", pet->is_sick);
    fprintf(f, "is_dead" CONFIG_DELIMITER "%d\n", pet->is_dead);

    if (fclose(f) == EOF)
        return -1;
    return 0;
}

Pet *load_pet(char *filename) {
    FILE *f;
    char buf[CONFIG_BUFFER_SIZE];
    char *key, *value;
    Pet *pet;

    if ((f = fopen(filename, "r")) == NULL)
        return NULL;

    // Create a default pet so no need to init struct.
    pet = create_pet("");

    while(fgets(buf, CONFIG_BUFFER_SIZE, f) != NULL) {
        trim_newlines(buf);
        key = strtok(buf, CONFIG_DELIMITER); 
        value = strtok(NULL, CONFIG_DELIMITER);
        if (key == NULL || value == NULL)
            continue;
        if (set_pet_config(pet, key, value) < 0)
            continue;
    }

    if (fclose(f) == EOF) {
        free_pet(pet);
        return NULL;
    }
    return pet;
}

int set_pet_config(Pet *pet, char *key, char *value) {
    if (strcmp("name", key) == 0) {
        free(pet->name);
        pet->name = (char*)malloc(sizeof(char)*strlen(value)+1);
        strcpy(pet->name, value);
        return 0;
    }
    if (strcmp("age", key) == 0) {
        pet->age = atoi(value);
        return 0;
    }

    if (strcmp("current_hunger", key) == 0) {
        pet->current_hunger = atof(value);
        return 0;
    }
    if (strcmp("hunger_decrease_chance", key) == 0) {
        pet->hunger_decrease_chance = atof(value);
        return 0;
    }
    if (strcmp("hunger_decrease_amount", key) == 0) {
        pet->hunger_decrease_amount = atof(value);
        return 0;
    }

    if (strcmp("current_happiness", key) == 0) {
        pet->current_happiness = atof(value);
        return 0;
    }
    if (strcmp("happiness_decrease_chance", key) == 0) {
        pet->happiness_decrease_chance = atof(value);
        return 0;
    }
    if (strcmp("happiness_decrease_amount", key) == 0) {
        pet->happiness_decrease_amount = atof(value);
        return 0;
    }

    if (strcmp("current_sickness_chance", key) == 0) {
        pet->current_sickness_chance = atof(value);
        return 0;
    }
    if (strcmp("sickness_increase_amount", key) == 0) {
        pet->sickness_increase_amount = atof(value);
        return 0;
    }
    if (strcmp("sickness_hunger_threshold", key) == 0) {
        pet->sickness_hunger_threshold = atof(value);
        return 0;
    }
    if (strcmp("sickness_happiness_threshold", key) == 0) {
        pet->sickness_happiness_threshold = atof(value);
        return 0;
    }

    if (strcmp("health", key) == 0) {
        pet->health = atoi(value);
        return 0;
    }
    if (strcmp("max_health", key) == 0) {
        pet->max_health = atoi(value);
        return 0;
    }
    if (strcmp("health_change", key) == 0) {
        pet->health_change = atoi(value);
        return 0;
    }
    if (strcmp("health_decrease_chance", key) == 0) {
        pet->health_decrease_chance = atof(value);
        return 0;
    }

    if (strcmp("is_sick", key) == 0) {
        pet->is_sick = atoi(value);
        return 0;
    }
    if (strcmp("is_dead", key) == 0) {
        pet->is_sick = atoi(value);
        return 0;
    }
    return -1;
}

int free_pet(Pet *pet) {
    free(pet->name);
    free(pet);
    return 0;
}

float max_float(float x, float y) {
    if (x > y)
        return x;
    return y;
}

float min_float(float x, float y) {
    if (x < y)
        return x;
    return y;
}

int max_int(int x, int y) {
    if (x > y)
        return x;
    return y;
}

int update_pet(Pet *pet) {
    if (pet->is_dead == 1)
        return -1;

    if (random_float(0.0, 1.0) < pet->hunger_decrease_chance) 
        pet->current_hunger -= pet->hunger_decrease_amount;

    if (random_float(0.0, 1.0) < pet->happiness_decrease_chance)
        pet->current_happiness -= pet->happiness_decrease_amount;

    if (!pet->is_sick) {
        if (pet->current_hunger < pet->sickness_hunger_threshold)
            pet->current_sickness_chance += pet->sickness_increase_amount;

        if (pet->current_happiness < pet->sickness_happiness_threshold)
            pet->current_sickness_chance += pet->sickness_increase_amount;
    } else {
        if (random_float(0.0, 1.0) < pet->health_decrease_chance)
            pet->health -= pet->health_change;
    }

    pet->current_hunger = max_float(0.0, pet->current_hunger);
    pet->current_happiness = max_float(0.0, pet->current_happiness);
    pet->current_sickness_chance = min_float(1.0, pet->current_sickness_chance);
    pet->health = max_int(0, pet->health);

    if (random_float(0.0, 1.0) < pet->current_sickness_chance)
        pet->is_sick = 1;

    if (pet->current_hunger <= 0.0 && pet->current_happiness <= 0.0) {
        pet->health = 0;
        pet->is_dead = 1;
        return -1;
    }

    if (pet->health <= 0) {
        pet->is_dead = 1;
        return -1;
    }

    pet->age += 1;

    return 0;
}
