#ifndef CONFIGSERVICE_H
#define CONFIGSERVICE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Config {
    FILE *configfile;
    char *CONFIGFILENAME;
    // Define a simple linked list for key-value properties.
    struct ConfigProperty *properties;
} Config;

// Key-value pair structure for properties.
typedef struct ConfigProperty {
    char *key;
    char *value;
    struct ConfigProperty *next;
} ConfigProperty;

Config *Config_getInstance();
void Config_init(Config *config);
void Config_destroy(Config *config);
char *Config_getProperty(Config *config, const char *key);
void Config_loadProperties(Config *config);
void Config_close(Config *config);
char *Config_toString(Config *config);

#endif
