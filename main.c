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

int clamp(int value, int min, int max) {
    return (value < min) ? min : (value > max) ? max : value;
}

int parse_input(const char *input, int current_brightness, int max_brightness)
{
    char *end;
    long value = strtol(input, &end, 10);

    if (*end != '\0') {
        if (*end == '%' && *(end + 1) == '\0') {
            value = max_brightness * value / 100;
        } else {
            fprintf(stderr, "Failed to parse input '%s'\n", input);
            exit(EXIT_FAILURE);
        }
    }

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
        printf("%d/%d -> %s -> %d/%d\n", brightness, max_brightness, argv[1], parse_input(argv[1], brightness, max_brightness), max_brightness);
    } else {
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
