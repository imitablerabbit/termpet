#ifndef TERMPET_H
#define TERMPET_H

// General util functions

// trim_newlines will remove all new lines at the end of a string. The string
// will be edited directly by moving the '\0' byte to the first '\n' byte found
// at the end of the string. The function returns the number of new lines that
// has been removed.
int trim_newlines(char* s);

// Returns a random floating point value in between min and max. The random
// seed should be initialised at the start of the program by calling
// srand(time(0));
float random_float(float min, float max);

// Return the highest of the 2 floating point values.
float max_float(float x, float y);

// Return the smallest of the 2 floating point values.
float min_float(float x, float y);

// Return the highest of the integer values.
int max_int(int x, int y);

// Return the smallest of the integer values.
int min_int(int x, int y);

// Config related functions.

#define CONFIG_BUFFER_SIZE 1000
#define CONFIG_DELIMITER "="

struct config {
    char *pets_dir;
};

typedef struct config Config;

// Loads a config file into a Config struct and returns a pointer to it. A
// config file should be a new line separated list of config elements.
// Typically each config element will be a key value pair delimited by an '='
// character. This function takes in a file path to the config file.
// Returns a pointer to a Config object on success or NULL if there were any
// errors. Any config objects created by this function should be freed when
// no longer needed by calling free_config.
Config *load_config(const char *filename);

// Releases memory for a config struct. This should be called whenever the
// config item is not longer needed. Returns 0 on success and -1 if any
// errors occurred.
int free_config(Config *config);

// Sets a specific config entry based on string encodings of the values. This
// function is used by the load_config function to populate each config field
// in the config struct. Returns a 0 if the config value was set correctly
// and -1 if any errors occurred. If a key is not recognised then a -1 is
// returned.
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

// Create a new pet with the specified name. The pet will be initialised to
// it's default values. Any pets created with this function should be freed
// with the free_pet function when no longer needed.
Pet *create_pet(char *name);

// Save a pet struct to a file. The format is the same as the general config.
// See load_config. The function takes in a pointer to a Pet object p which is
// the pet that should be saved. It also takes in a file path which is the save
// file destination. No checks are performed when saving to the file. Any
// existing data will be overwritten. If the file does not exist then it will be
// created automatically. Returns 0 if the save was successful, returns -1 if an
// error occurred.
int save_pet(Pet *p, char *filename);

// Load a pet from a save file. The format is the same as the general config.
// see load_config. Any pets loaded via this method should be freed when they
// are no longer needed. Use free_pet to free the dymanically allocated memory
// from this function. This function takes in a file path to the save file that
// should be loaded and returns a pointer to a Pet object. If the save file
// could not be loaded then this function will return NULL.
Pet *load_pet(char *filename);

// Sets a specific pet config value based on string inputs. This functions is
// intended to be used with the load_pet function.
// Returns 0 if the config value was correctly set. Returns -1 if an error
// occured. An unknown pet config key would mean this function returns -1.
int set_pet_config(Pet *pet, char *key, char *value);

// Free any dynamically allocated memory used for the passed in Pet. This
// function should be called whenever the pet object is no longer needed.
// Returns 0 if successful
int free_pet(Pet *pet);

// Updates the pets internal values. This function signifies the passage of
// time. It should be called inside of a game loop.
// Returns 0 if the pet is still alive after the update. Returns -1 if the
// pet has dies during the update. If the pet is deceased then it's values
// will no longer be updated.
int update_pet(Pet *pet);

// Feeds the pet in order to relieve some hunger. This function should be called
// as part of a user action in order to keep the pet alive. If the pet is
// deceased then the hunger values are not changed.
// Returns 0 if the pet was able to be fed. Returns -1 if the pet could not be
// fed. The pet cannot be fed if it is already deceased.
int feed_pet(Pet *pet);

// Play with the pet to give the pet some happiness. This function should be
// called as part of a user action in order to keep the pet alive. If the pet
// is deceased then the happiness values are not changed.
// Returns 0 if the pet was able to be played with. Returns -1 if the pet
// could not be played with. The pet cannot be played with if it is already
// deceased.
int play_with_pet(Pet *pet);

// Give the pet some medicine in order to remove its sickness attribute and to
// reset the current_sickness_chance stat. Returns 0 if giving the medicine was
// successful. Returns -1 if giving the medicine failed. The user is not able
// to give the pet medicine if it is already deceased.
int give_medicine_to_pet(Pet *pet);

#endif