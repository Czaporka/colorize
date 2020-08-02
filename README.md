![](https://github.com/Czaporka/colorize/workflows/build/badge.svg)

# colorize

## Description
`colorize` is a simple utility that employs color-coding in order to aid humans in parsing console output that contains a lot of similarly-looking words, especially numerals.  

## Examples
![](docs/examples.jpeg)

## Installation
```bash
git clone https://github.com/Czaporka/colorize.git
cd colorize
make install
```
Alternatively, download a pre-built executable (compiled on Debian):
```bash
curl --create-dirs -Lo ~/.local/bin/colorize https://github.com/Czaporka/colorize/releases/download/v1.1.0/colorize
chmod +x ~/.local/bin/colorize
```
Make sure `~/.local/bin` is on your `PATH`, e.g.:
```bash
export PATH=${PATH}:~/.local/bin
echo 'export PATH=${PATH}:~/.local/bin' >> ~/.bashrc
```

## Usage
```
Usage: colorize [-a | -h]
Colorize text (by default just numerals).
Example: df | colorize
Example: colorize -a < file.txt
Options:
  -a, --all   : colorize all words, not just numerals
  -h, --help  : display this help message and exit
```
