# Ment0r
A (probably) broken chess engine I never bothered to finish.

Hello users, the project that you are about to be shown is probably the worst piece of code you have ever seen.
It contains more memory leaks than Joe Biden.
I don't really recommend to look into the code accualy, but if you really want to, here is how to build this monstrosity.
It uses raylib and PESTO's eval function. thx to https://www.chessprogramming.org <3

# Step 1 - install raylib (and gcc)
https://www.raylib.com/
# Step 2 - compile
in the main directory do

```gcc src/*.c -lraylib```

*I will probably add cmake support(but it is a distant future)

# Usage
this program takes two agruments , first 'B' or 'W' specifing the side to move

second, the fen string

It should look roughly like:

```./your_binary [move] [fen string]```

then the program will ask for the depth, just input 5 or 4, it should (probably) work
