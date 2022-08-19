#ifndef CFGFILE_H_
#define CFGFILE_H_

#include <stdbool.h>

/**
 * @brief Configuration for the program.
 */
typedef struct {
    char *logfile_path;
    // ...
} cfgfile_config_t;

/**
 * @brief Parses the text of a configuration file.
 */
bool cfgfile_parse(char *filetext, cfgfile_config_t **result);

/**
 * @brief Prints a demonstration message.
 */
void cfgfile_print_message(void);

/**
 * @brief Adds one to an integer.
 *
 * @param a The starting integer.
 * @return int The argument plus one.
 */
int cfgfile_add_one(int a);

#endif