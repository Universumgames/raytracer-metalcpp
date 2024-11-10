#include "../include/configservice.h"

// Static instance of Config to maintain singleton pattern
static Config *instance = NULL;

/**
 * @brief Get the singleton instance of the Config struct.
 * If the instance doesn't exist, initialize it.
 * 
 * @return Config* Pointer to the singleton Config instance.
 */
Config *Config_getInstance() {
    if (instance == NULL) {
        instance = (Config *)malloc(sizeof(Config));
        Config_init(instance);  // Initialize the singleton instance
    }
    return instance;
}

/**
 * @brief Initialize a Config struct with default values.
 * Sets the config file name and loads properties from the file.
 * 
 * @param config Pointer to the Config struct to initialize.
 */
void Config_init(Config *config) {
    config->CONFIGFILENAME = "config.ini";  // Default config file name
    config->properties = NULL;              // Initialize properties to NULL
    Config_loadProperties(config);          // Load properties from file
}

/**
 * @brief Clean up and free resources associated with a Config struct.
 * Closes the config file if open, frees all properties, and frees config.
 * 
 * @param config Pointer to the Config struct to destroy.
 */
void Config_destroy(Config *config) {
    Config_close(config);  // Close config file if open

    // Free each property in the linked list
    ConfigProperty *prop = config->properties;
    while (prop) {
        ConfigProperty *next = prop->next;
        free(prop->key);   // Free property key
        free(prop->value); // Free property value
        free(prop);        // Free the property struct itself
        prop = next;
    }

    free(config);  // Free the Config struct itself
}

/**
 * @brief Load properties from the configuration file into the Config struct.
 * Parses the file line by line, splitting each line by '=' into key-value pairs,
 * removes any line breaks from the values, and stores them in the Config's properties list.
 * Properties are stored in the order they appear in the file.
 *
 * @param config Pointer to the Config struct where properties will be stored.
 */
void Config_loadProperties(Config *config) {
    config->configfile = fopen(config->CONFIGFILENAME, "r");
    if (!config->configfile) {
        perror("Could not open config file");
        exit(EXIT_FAILURE);
    }

    char line[255];
    ConfigProperty *last_prop = NULL; // Pointer to track the last property in the list

    // Read file line by line
    while (fgets(line, sizeof(line), config->configfile) != NULL) {
        if (line[0] == ';') continue; // Ignore comment lines

        // Find the delimiter ('=') between key and value
        char *delimiter = strchr(line, '=');
        if (!delimiter) {
            perror("Config file format error");
            exit(EXIT_FAILURE);
        }

        *delimiter = '\0';          // Null-terminate key part
        char *key = line;           // Key starts at beginning of line
        char *value = delimiter + 1; // Value starts after '='

        // Remove newline and carriage return characters from the value
        char *newline = strchr(value, '\n');
        if (newline) *newline = '\0';
        char *carriage_return = strchr(value, '\r');
        if (carriage_return) *carriage_return = '\0';

        // Allocate and initialize new property
        ConfigProperty *prop = (ConfigProperty *)malloc(sizeof(ConfigProperty));
        prop->key = strdup(key);
        prop->value = strdup(value);
        prop->next = NULL;

        // Add to the end of the list to maintain order
        if (last_prop == NULL) {
            config->properties = prop; // First element in the list
        } else {
            last_prop->next = prop; // Append to the end
        }
        last_prop = prop; // Update last_prop to the new end of the list
    }
}

/**
 * @brief Retrieve the value of a property given its key.
 * Searches through the properties linked list for a matching key.
 * 
 * @param config Pointer to the Config struct containing properties.
 * @param key The property key to look up.
 * @return char* The property value if found; NULL otherwise.
 */
char *Config_getProperty(Config *config, const char *key) {
    ConfigProperty *prop = config->properties;
    // Traverse the list to find the matching key
    while (prop) {
        if (strcmp(prop->key, key) == 0) {
            return prop->value; // Return value if key matches
        }
        prop = prop->next;
    }
    return NULL; // Return NULL if key not found
}

/**
 * @brief Close the configuration file if it is open.
 * 
 * @param config Pointer to the Config struct containing the file pointer.
 */
void Config_close(Config *config) {
    if (config->configfile) {
        fclose(config->configfile); // Close the file
        config->configfile = NULL;
    }
}

/**
 * @brief Generate a string representation of the Config properties.
 * Iterates through the properties list and concatenates each key-value pair
 * into a single formatted string.
 * 
 * @param config Pointer to the Config struct containing properties.
 * @return char* A string representing all properties (caller must free).
 */
char *Config_toString(Config *config) {

    ConfigProperty *prop = config->properties;
    size_t buffer_size = 256;
    char *buffer = (char *)malloc(buffer_size);
    snprintf(buffer, buffer_size, "Configuration:{");

    // Append each key-value pair to the output string
    while (prop) {
        size_t needed_size = strlen(buffer) + strlen(prop->key) + strlen(prop->value) + 1;
        if (needed_size >= buffer_size) {
            buffer_size *= 2;
            buffer = (char *)realloc(buffer, buffer_size); // Expand buffer as needed
        }
        strcat(buffer, prop->key);
        strcat(buffer, "=");
        strcat(buffer, prop->value);
        strcat(buffer, ";");
        prop = prop->next;
    }
    strcat(buffer, "}"); // End of configuration string
    return buffer;
}
