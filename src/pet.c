#include "termpet.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

Pet *create_pet(char *name) {
    Pet *pet = NULL;

    pet = (Pet*)malloc(sizeof(Pet));

    pet->name = (char*)malloc(sizeof(char)*strlen(name)+1);
    strcpy(pet->name, name);
    pet->age = 0;

    pet->current_hunger=1.0;
    pet->max_hunger=1.0;
    pet->hunger_decrease_chance=0.01;
    pet->hunger_decrease_amount=0.1;

    pet->current_happiness=1.0;
    pet->max_happiness=1.0;
    pet->happiness_decrease_chance=0.01;
    pet->happiness_decrease_amount=0.1;

    pet->current_sickness_chance=0.001;
    pet->max_sickness_chance=0.2;
    pet->min_sickness_chance=0.001;
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

int copy_pet(Pet **dst, Pet *src) {
    Pet *dst_temp;

    if (src == NULL)
        return -1;

    dst_temp = (Pet*)malloc(sizeof(Pet));
    if (dst_temp == NULL)
        return -1;

    memcpy(dst_temp, src, sizeof(Pet));

    if (src->name != NULL) {
        dst_temp->name = (char*)calloc(strlen(src->name)+1, sizeof(char));
        if (!dst_temp->name) {
            free(dst_temp);
            return -1;
        }
        strcpy(dst_temp->name, src->name);
    }

    *dst = dst_temp;
    return 0;
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
    if (strcmp("max_hunger", key) == 0) {
        pet->max_hunger = atof(value);
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
    if (strcmp("max_happiness", key) == 0) {
        pet->max_happiness = atof(value);
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
    if (strcmp("max_sickness_chance", key) == 0) {
        pet->max_sickness_chance = atof(value);
        return 0;
    }
    if (strcmp("min_sickness_chance", key) == 0) {
        pet->min_sickness_chance = atof(value);
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
    if (pet == NULL)
        return -1;

    if (pet->name != NULL)
        free(pet->name);
    free(pet);
    return 0;
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
    pet->current_sickness_chance = min_float(pet->max_sickness_chance,
            pet->current_sickness_chance);
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

int feed_pet(Pet *pet) {
    if (pet->is_dead)
        return -1;

    pet->current_hunger += 0.5;
    pet->current_hunger = min_float(pet->max_hunger, pet->current_hunger);
    return 0;
}

int play_with_pet(Pet *pet) {
    if (pet->is_dead)
        return -1;

    pet->current_happiness += 0.5;
    pet->current_happiness = min_float(pet->max_happiness, pet->current_happiness);
    return 0;
}

int give_medicine_to_pet(Pet *pet) {
    if (pet->is_dead)
        return -1;

    pet->is_sick = 0;
    pet->current_sickness_chance = pet->min_sickness_chance;
    pet->health += 1;
    pet->health = min_int(pet->max_health, pet->health);
    return 0;
}
