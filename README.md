# Lumeus

Lumeus is a simple command line tool that adjusts backlight brightness using
ACPI.

## Usage

Running the command `lumeus` outputs the current brightness as percentage.

Adjusting the brightness can be done by giving the command a value. For example
`lumeus +10%` increases the brightness by 10 percent. To decrease the
brightness naturally use a minus sign instead. By omitting the sign altogether,
the brightness is set to percentage of maximum brightness.

It's also possible to use the raw brightness values simply by dropping the
percentage sign and using plain integers.

## License

MIT
