# colorize
[![build](https://github.com/Czaporka/colorize/workflows/build/badge.svg)](https://github.com/Czaporka/colorize/actions)
[![codecov](https://codecov.io/gh/Czaporka/colorize/branch/master/graph/badge.svg)](https://codecov.io/gh/Czaporka/colorize)

## Description
`colorize` is a simple utility that employs color-coding in order to aid humans in parsing console output that contains a lot of similarly-looking words, especially numerals.

## Examples
![](docs/examples.jpeg)

## Installation
### Pre-built executable
You can download & install a pre-built executable (compiled on Debian/Ubuntu, may not work on other platforms):
```bash
curl --create-dirs -fLo ~/.local/bin/colorize https://github.com/Czaporka/colorize/releases/download/v1.5.0/colorize
chmod +x ~/.local/bin/colorize
```
### Compiling from sources
If the pre-built binary doesn't work for you, you can try compiling from sources:
```bash
git clone https://github.com/Czaporka/colorize.git
cd colorize
make install
```
#### Fun fact:
The source code contains two alternative implementations of the method
for finding words to colorize: one of them utilizes C++/`<regex>`, the
other one utilizes C/`<regex.h>`. By default, the C one is enabled, as
it should be more cross-platform. If you're feeling adventurous, you
may try compiling with the C++ implementation enabled, or with both of
them (in which case you will be able to switch to the C one by providing the
`--posix` command line option to `colorize`):
```bash
make install IMPL=CXX   # compile & install with the C++ implementation enabled
make install IMPL=both  # compile & install with both the C++ and the C implementations enabled
```
If you're using `gcc`, the C++ implementation requires version 4.9.0+ (with regex support).
### PATH
Make sure `~/.local/bin` is on your `PATH`, e.g.:
```bash
export PATH=${PATH}:~/.local/bin
echo 'export PATH=${PATH}:~/.local/bin' >> ~/.bashrc
```

## Usage
```
Usage: colorize [--help|-h] [--all|-a] [--comprise|-c CHARS] [--embedded|-e] [--ignore-case|-i] [--print-regex|-p] [--regex|-r REGEX] [--salt|-s SALT] [--version|-V] [--hex|-x]

Colorize text (by default just numerals).

Example: df | colorize
Example: colorize -a < file.txt

Optional arguments:
  -h, --help           show this help message and exit
  -a, --all            colorize all words, not just numerals
  -c, --comprise=CHARS specify additional characters to be interpreted as part
                       of the same token; for example, use --comprise=,: for
                       text containing numbers with comma for decimal separator
                       (e.g. 3,141) and timestamps such as 23:59:59
  -e, --embedded       also catch numbers embedded within larger, not purely
                       numerical strings, e.g. in "Word01", "01" will be
                       colorized (normally it would not)
  -i, --ignore-case    make sure "token", "TOKEN", "ToKeN", etc., are the same
                       color
  -p, --print-regex    print the final regex to STDERR; this can be used to
                       fine-tune the regex manually, then provide the result
                       with the --regex option
  -r, --regex=REGEX    use an arbitrary REGEX for finding the tokens; -a and -c
                       are then ignored
  -s, --salt=SALT      append a salt to every token before hashing, effectively
                       shuffling the colors
  -V, --version        print version and exit
  -x, --hex            force catching hexadecimal digits even when not prefixed
                       with "0x"
```
