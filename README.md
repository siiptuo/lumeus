# Lumeus

Lumeus is a simple command line utility for Linux to adjust screen backlight
brightness. It's useful for binding to a hotkey but it can also be used
manually. It doesn't rely on X11 so it can be used in Wayland and system
console.

## Usage

Running the command `lumeus` outputs the current brightness as percentage.

Adjusting the brightness can be done by giving the command a value. For example
`lumeus +10%` increases the brightness by 10 percent. To decrease the
brightness naturally use a minus sign instead. By omitting the sign altogether,
the brightness is set to percentage of maximum brightness.

It's also possible to use an exact hardware brightness value simply by dropping
the percentage sign and using plain integers. This is useful if the backlight
brightness range is small like 0-15.

## License

MIT
