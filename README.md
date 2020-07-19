![](https://github.com/Czaporka/colorize-numerals/workflows/build/badge.svg)
# colorize-numerals

## Description
`colorize-numerals` is a simple utility that employs color-coding in order to aid humans in parsing console output that contains a lot of numbers.
By applying the same distinct color to every occurrence of a number, hopefully it should make it easier to figure out what is what.

For example,
It is difficult to understand what is happening here:
![](docs/without.png)

This looks much better:
![](docs/with.png)

## How to install?
```bash
git clone https://github.com/Czaporka/colorize-numerals.git
cd colorize-numerals
make install
```
Alternatively, download a pre-built executable (compiled on Debian):
```bash
curl --create-dirs -Lo ~/.local/bin/colorize-numerals https://github.com/Czaporka/colorize-numerals/releases/download/v1.0.1/colorize-numerals
chmod +x ~/.local/bin/colorize-numerals
```
Make sure `~/.local/bin` is on your `PATH`, e.g.:
```bash
echo 'export PATH=${PATH}:~/.local/bin' >> ~/.bashrc
```
