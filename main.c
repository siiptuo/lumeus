// Lumeus - adjust backlight brightness
// Copyright (c) 2015 Tuomas Siipola
// See LICENSE file for license details.

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

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
    // TODO: don't hard code the path
    int brightness     = read_integer_from_file("/sys/class/backlight/intel_backlight/brightness");
    int max_brightness = read_integer_from_file("/sys/class/backlight/intel_backlight/max_brightness");
    if (argc == 1) {
        printf("%d%\n", brightness * 100 / max_brightness);
    } else if (argc == 2) {
        write_integer_to_file("/sys/class/backlight/intel_backlight/brightness", parse_input(argv[1], brightness, max_brightness));
    } else {
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
