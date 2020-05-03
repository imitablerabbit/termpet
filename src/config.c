#include "termpet.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

Config *load_config(const char *filename) {
    FILE *f;
    char buf[CONFIG_BUFFER_SIZE];
    char *key, *value;
    Config *config;

    if ((f = fopen(filename, "r")) == NULL)
        return NULL;

    config = (Config*)malloc(sizeof(Config));

    while(fgets(buf, CONFIG_BUFFER_SIZE, f) != NULL) {
        trim_newlines(buf);
        key = strtok(buf, CONFIG_DELIMITER);
        value = strtok(NULL, CONFIG_DELIMITER);
        if (key == NULL || value == NULL)
            continue;
        if (set_config(config, key, value) < 0)
            continue;
    }

    if (fclose(f) == EOF) {
        free_config(config);
        return NULL;
    }
    return config;
}

int set_config(Config *config, char *key, char *value) {
    if (strcmp("pets_dir", key) == 0) {
        config->pets_dir = (char*)malloc(sizeof(char)*strlen(value)+1);
        strcpy(config->pets_dir, value);
        return 0;
    }
    if (strcmp("save_dir", key) == 0) {
        config->save_dir = (char*)malloc(sizeof(char)*strlen(value)+1);
        strcpy(config->save_dir, value);
        return 0;
    }
    return -1;
}

int free_config(Config *config) {
    free(config->pets_dir);
    free(config->save_dir);
    free(config);
    return 0;
}
