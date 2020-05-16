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
    sprintf(s, "%s/%s", dir, save_name);
    return s;
}

Save **save_files(char *dir) {
    Save **saves, **saves_iter, *s;
    DIR *d;
    struct dirent *de;
    int i;
    int name_len, dir_len, path_len;

    if ((d = opendir(dir)) == NULL)
        return NULL;

    i = 0;
    dir_len = strlen(dir);
    saves = (Save**)calloc(MAX_SAVE_FILES, sizeof(Save*));
    saves_iter = saves;
    while ((de = readdir(d)) != NULL && i < MAX_SAVE_FILES-1) {
        if (strcmp(de->d_name, ".") == 0 || strcmp(de->d_name, "..") == 0) {
            continue;
        }
        s = (Save*)malloc(sizeof(Save));
        name_len = strlen(de->d_name);
        path_len = dir_len + 1 + name_len;
        s->name = (char*)calloc(name_len+1, sizeof(char));
        s->path = (char*)calloc(path_len+1, sizeof(char));
        s->pet = NULL;
        strcpy(s->name, de->d_name);
        sprintf(s->path, "%s/%s", dir, s->name);
        *saves_iter = s;
        saves_iter++;
        i++;
    }
    *saves_iter = NULL;

    if (closedir(d) == -1) {
        free_save_files(saves);
        return NULL;
    }
    return saves;
}

int copy_save(Save **dst, Save *src) {
    Save *dst_temp;

    if (src == NULL)
        return -1;

    dst_temp = (Save*)malloc(sizeof(Save));
    if (!dst_temp)
        return -1;
    memset(dst_temp, 0, sizeof(Save));

    if (src->name) {
        dst_temp->name = (char*)calloc(strlen(src->name)+1, sizeof(char));
        if (dst_temp->name == NULL) {
            free_save(dst_temp);
            return -1;
        }
        strcpy(dst_temp->name, src->name);
    }
    if (src->path) {
        dst_temp->path = (char*)calloc(strlen(src->path)+1, sizeof(char));
        if (dst_temp->path == NULL) {
            free_save(dst_temp);
            return -1;
        }
        strcpy(dst_temp->path, src->path);
    }
    if (src->pet) {
        dst_temp->pet = (Pet*)malloc(sizeof(Pet*));
        if (dst_temp->pet == NULL) {
            free_save(dst_temp);
            return -1;
        }
        copy_pet(&dst_temp->pet, src->pet);
    }

    *dst = dst_temp;
    return 0;
}

void free_save(Save *s) {
    if (!s)
        return;

    if (s->name != NULL)
        free(s->name);
    if (s->path != NULL)
        free(s->path);
    if (s->pet != NULL)
        free_pet(s->pet);
    free(s);
}

void free_save_files(Save **saves) {
    Save **save_iter;

    if (!saves)
        return;

    save_iter = saves;
    for (save_iter = saves; *save_iter != NULL; save_iter++) {
        free_save(*save_iter);
    }
    free(saves);
}

Save *save_pet(Pet *pet, char *save_path) {
    FILE *f = NULL;
    Save *s = NULL;

    s = (Save*)malloc(sizeof(Save));
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


