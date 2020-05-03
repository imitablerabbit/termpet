#include "termpet.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <dirent.h>

char *save_file_path(char *dir, char *save_name) {
    char *s, *s_init;
    int dir_len, name_len, len;

    if (strcmp(dir, "") == 0)
        return NULL;
    if (strcmp(save_name, "") == 0)
        return NULL;

    dir_len = strlen(dir);
    name_len = strlen(save_name);
    len = dir_len + name_len;
    s = (char*)calloc(len+2, sizeof(char));
    s_init = s;

    strcpy(s, dir);
    s += dir_len;
    *s = (char)'/';
    s++;
    strcpy(s, save_name);
    s += name_len;
    s = '\0';

    return s_init;
}

Save **save_files(char *dir) {
    Save **saves, **saves_iter, *s;
    DIR *d;
    struct dirent *de;
    int i;

    if ((d = opendir(dir)) == NULL)
        return NULL;

    i = 0;
    saves = (Save**)calloc(MAX_SAVE_FILES, sizeof(Save*));
    while ((de = readdir(d)) != NULL && i < MAX_SAVE_FILES-1) {
        s = (Save*)malloc(sizeof(Save*));
        s->path = (char*)calloc(strlen(de->d_name), sizeof(char));
        s->pet = NULL;
        strcpy(s->path, de->d_name);
        *saves_iter = s;
        saves_iter++;
        i++;
    }
    saves_iter = NULL;

    if (!closedir(d)) {
        free_save_files(saves);
        return NULL;
    }
    return saves;
}

int free_save_files(Save **saves) {
    Save **save_iter;
    Save *s;

    while(++save_iter) {
        s = *save_iter;
        free(s->path);
        if (s->pet)
            free_pet(s->pet);
        free(s);
    }
    free(saves);
    return 0;
}

Save *save_pet(Pet *pet, char *save_path) {
    FILE *f;
    Save *s;

    s = (Save*)malloc(sizeof(Save*));
    s->path = save_path;
    s->pet = pet;

    if ((f = fopen(s->path, "w")) == NULL) {
        free(s->path);
        free(s);
        return NULL;
    }

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
        return NULL;

    return s;
}

int load_pet(Save *save) {
    FILE *f;
    char buf[CONFIG_BUFFER_SIZE];
    char *key, *value;
    Pet *pet;

    if (!save)
        return -1;

    if ((f = fopen(save->path, "r")) == NULL)
        return -1;

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
        return -1;
    }

    save->pet = pet;
    return 0;
}


