#ifndef TAMAGOTCHI_H
#define TAMAGOTCHI_H

// General util functions

// trim_newlines will remove all new lines at the end of a string. The function
// returns the number of new lines that has been removed.
int trim_newlines(char* s);

float random_float(float min, float max);

// Config related functions.

#define CONFIG_BUFFER_SIZE 1000
#define CONFIG_DELIMITER "="

struct config {
    char *pets_dir;
};

typedef struct config Config;

// Loads a config file into a Config struct and returns a pointer to it. A
// config file should be a new line separeted list of config elements.
// Typically each config element will be a key value pair delimited by an '='
// character.
Config *load_config(const char *filename);

// Releases memory for a config struct. This should be called whenever the
// config item is not longer needed.
int free_config(Config *config);

// Sets a specific config entry based on string encodings of the values. This
// function is used by the load_config function to populate each config field
// in the config struct.
int set_config(Config *config, char *key, char *value);

// Pet related functions.

struct pet {
    char *name;
    int age;

    float current_hunger;
    float hunger_decrease_chance;
    float hunger_decrease_amount;

    float current_happiness;
    float happiness_decrease_chance;
    float happiness_decrease_amount;

    float current_sickness_chance;
    float sickness_increase_amount;
    float sickness_hunger_threshold;
    float sickness_happiness_threshold;

    int health;
    int max_health;
    int health_change;
    float health_decrease_chance;

    int is_sick;
    int is_dead;
};

typedef struct pet Pet;

// Create a new pet. The pet will be initialised to it's default values.
Pet *create_pet(char *name);

// Save a pet struct to a file. The format is the same as the general config.
// See load_config.
int save_pet(Pet *p, char *filename);

// Load a pet from a save file. The format is the same as the general config.
// see load_config.
Pet *load_pet(char *filename);

// Sets a specific pet config value based on string inputs. This functions is
// intended to be used with the load_pet function.
int set_pet_config(Pet *pet, char *key, char *value);

// Free any dynamically allocated memory used for the passed in Pet. This
// function should be called whenever the pet object is no longer needed.
int free_pet(Pet *pet);

// Updates the pets internal values. This function signifies the passage of
// time. It should be called inside of a game loop.
int update_pet(Pet *pet);

int feed_pet(Pet *pet);

int play_with_pet(Pet *pet);

int give_medicine_to_pet(Pet *pet);

#endif
