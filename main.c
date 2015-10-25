// Lumeus - adjust backlight brightness
// Copyright (c) 2015 Tuomas Siipola
// See LICENSE file for license details.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <dirent.h>
#include <limits.h>

int read_integer_from_file(const char *filename)
{
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        fprintf(stderr, "Failed to open file '%s'", filename);
        exit(EXIT_FAILURE);
    }
    int value;
    int ret = fscanf(fp, "%d\n", &value);
    if (ret != 1) {
        fprintf(stderr, "Failed to parse integer from '%s'\n", filename);
        exit(EXIT_FAILURE);
    }
    fclose(fp);
    return value;
}

void write_integer_to_file(const char *filename, int value)
{
    FILE *fp = fopen(filename, "w");
    if (!fp) {
        fprintf(stderr, "Failed to open file '%s'", filename);
        exit(EXIT_FAILURE);
    }
    int ret = fprintf(fp, "%d\n", value);
    if (ret <= 0) {
        fprintf(stderr, "Failed to write to file '%s'\n", filename);
        exit(EXIT_FAILURE);
    }
    fclose(fp);
}

// Keep the value between min and max.
int clamp(int value, int min, int max) {
    return (value < min) ? min : (value > max) ? max : value;
}

// This function returns new brightness based on user input string. The input
// can either be absolute or relative to current brightness indicated by a plus
// or minus sign or the lack of it. It can also be a raw brightness value or
// percentage of maximum brightness.
int parse_input(const char *input, int current_brightness, int max_brightness)
{
    // Try to parse input string as an integer.
    char *end;
    long value = strtol(input, &end, 10);

    // Check are there any extra characters after the integer.
    // If so, the input can only valid if it's a percentage.
    if (*end != '\0') {
        if (*end == '%' && *(end + 1) == '\0') {
            value = max_brightness * value / 100;
        } else {
            fprintf(stderr, "Failed to parse input '%s'\n", input);
            exit(EXIT_FAILURE);
        }
    }

    // Determine if the value is relative or absolute.
    if (input[0] == '+' || input[0] == '-') {
        return clamp(current_brightness + value, 0, max_brightness);
    } else {
        return clamp(value, 0, max_brightness);
    }
}

int main(int argc, char *argv[])
{
    char controller[NAME_MAX] = { '\0' };

    // TODO: allow specifying wanted controller
    DIR *dp = opendir("/sys/class/backlight");
    if (!dp) {
        fprintf(stderr, "Failed to open directory '/sys/class/backlight'\n");
        exit(EXIT_FAILURE);
    }
    struct dirent *entry;
    while ((entry = readdir(dp)) != NULL) {
        if (entry->d_name[0] == '.') {
            continue;
        }
        memcpy(controller, entry->d_name, NAME_MAX);
        break;
    }
    closedir(dp);

    if (controller[0] == '\0') {
        fprintf(stderr, "No controller found\n");
        exit(EXIT_FAILURE);
    }

    char brightness_path[PATH_MAX];
    snprintf(brightness_path, PATH_MAX, "/sys/class/backlight/%s/brightness", controller);
    int brightness = read_integer_from_file(brightness_path);

    char max_brightness_path[PATH_MAX];
    snprintf(max_brightness_path, PATH_MAX, "/sys/class/backlight/%s/max_brightness", controller);
    int max_brightness = read_integer_from_file(max_brightness_path);

    if (argc == 1) {
        printf("%d%\n", brightness * 100 / max_brightness);
    } else if (argc == 2) {
        write_integer_to_file(brightness_path, parse_input(argv[1], brightness, max_brightness));
    } else {
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
