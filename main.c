#include <stdio.h>
#include <stdlib.h>

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

int main(int argc, char *argv[])
{
    // TODO: don't hard code the path
    int brightness     = read_integer_from_file("/sys/class/backlight/intel_backlight/brightness");
    int max_brightness = read_integer_from_file("/sys/class/backlight/intel_backlight/max_brightness");
    printf("%d%\n", brightness * 100 / max_brightness);
    return EXIT_SUCCESS;
}
