<table><tr>
<td><img src="./15projects_logo copy.svg" width="360"/></td>
<td><h1>Day 10/15 — samGit</h1></td>
</tr></table>

## Installation (Windows)

### Prerequisites
You need MSYS2 installed. If you don't have it, grab it at https://www.msys2.org and run the installer.

### Step 1 — Open the right terminal
Open **MSYS2 MinGW64** from your Start menu. Not UCRT64, not MSYS2 — specifically MinGW64.

### Step 2 — Install dependencies
Run these two commands:

`pacman -S mingw-w64-x86_64-openssl`

`pacman -S mingw-w64-x86_64-zlib`

### Step 3 — Verify installation
`pkg-config --libs openssl`

`pkg-config --libs zlib`

You should see `-lssl -lcrypto` and `-lz` respectively.

### Step 4 — Compile

`gcc git.c -o mygit -lssl -lcrypto -lz -lgdi32 -lws2_32`

### Step 5 — Run
`./mygit init`

`./mygit add <filename>`

`./mygit commit "<message>"`

`./mygit log`

---

## Installation (Mac)

### Step 1 — Install Homebrew if you don't have it
`/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"`

### Step 2 — Install dependencies
`brew install openssl zlib`

### Step 3 — Compile
`gcc git.c -o mygit -lssl -lcrypto -lz`

---

## Installation (Linux)

### Debian/Ubuntu
`sudo apt install libssl-dev zlib1g-dev`

### Arch

`sudo pacman -S openssl zlib`

### Compile
`gcc git.c -o mygit -lssl -lcrypto -lz`

## Credits

["Write yourself a Git!"](https://wyag.thb.lt/) by Thibault Polge is a Python based tutorial on writing your own Git, and hence it played a significant role in understanding how each Git function works.