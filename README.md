<table><tr>
<td><img src="./10projects_logo copy.svg" width="360"/></td>
<td><h1>Day 10/10 — samGit</h1></td>
</tr></table>

## Introduction

samGit is a recreation of Git done by myself to understand how Git functions while also being another C-based project after I started learning the language. At the moment it only has three functions, init, add, and commit, which makes it technically functional! It's also locally hosted, so no you cannot use it to push changes to Github.

Everything here is compressed to a zlib object, stored in .samgit/objects/, identified by the SHA-1 hash of its contents. The same content will always produce the same hash, so repeat files won't need to be stored twice and whatnot. If you want to see a completed run, check inside `samGit/Example of a full repository/`. The following instructions were ran:

```
gcc samgit.c -o samgit -lssl -lcrypto -lz -lgdi32 -lws2_32
./samgit init
echo "Test" > test.txt
./samgit add test.txt
./samgit commit "Test successful!"
```

Do I expect anyone ever to make samGit a replacement to Git? Absolutely not! I will also continue to use Git itself, as it is a perfect creation, but then again what's the point in using a tool if you don't undersand how it works? For somebody whose only been using C for about a month, it's been pretty fun to work on this project, albeit stressful at times as even though I have a reference for how each function works, I still have to learn a lot about the intricacies of how C functions. 

I think I'm ready for my Programming in C class next semester, as that is meant to be when I learn what C is. 😂

## Usage

Ensure you have followed compilation instructions below. This is how you'd use it with GCC, so if you have a seperate compiler ask Claude or something, or just figure it out yourself.

```./samgit init``` - Initializes your repository.
```./samgit add <filename>``` - Adds a file to path.
```./samgit commit "Message"``` - Commit, with a message.

At the moment if you wish to decrypt the hexadecimal messages, you're going to have to do that yourself with SHA-1. I should fix that in the near future.

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

`gcc git.c -o samgit -lssl -lcrypto -lz -lgdi32 -lws2_32`

### Step 5 — Run
`./samgit init`

`./samgit add <filename>`

`./samgit commit "<message>"`

---

## Installation (Mac)

### Step 1 — Install Homebrew if you don't have it
`/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"`

### Step 2 — Install dependencies
`brew install openssl zlib`

### Step 3 — Compile
`gcc git.c -o samgit -lssl -lcrypto -lz`

---

## Installation (Linux)

### Debian/Ubuntu
`sudo apt install libssl-dev zlib1g-dev`

### Arch

`sudo pacman -S openssl zlib`

### Compile
`gcc git.c -o samgit -lssl -lcrypto -lz`

## Credits

**["Write yourself a Git!"](https://wyag.thb.lt/)** by Thibault Polge - A Python based tutorial on writing your own Git, and hence it played a significant role in understanding how each Git function works.

**OpenSSL** - SHA-1 hashing via `<openssl/sha.h>` (https://www.openssl.org)

**zlib** - Object compression via `<zlib.h>` (https://zlib.net)
