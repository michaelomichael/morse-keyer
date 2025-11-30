#include "keyboard-layouts.h"

char DvorakKeyboardLayout::remap(char original) const {
    switch (original) {
        case 'a':
            return 'a';
        case 'b':
            return 'n';
        case 'c':
            return 'i';
        case 'd':
            return 'h';
        case 'e':
            return 'd';
        case 'f':
            return 'y';
        case 'g':
            return 'u';
        case 'h':
            return 'j';
        case 'i':
            return 'g';
        case 'j':
            return 'c';
        case 'k':
            return 'v';
        case 'l':
            return 'p';
        case 'm':
            return 'm';
        case 'n':
            return 'l';
        case 'o':
            return 's';
        case 'p':
            return 'r';
        case 'q':
            return 'x';
        case 'r':
            return 'o';
        case 's':
            return ';';
        case 't':
            return 'k';
        case 'u':
            return 'f';
        case 'v':
            return '.';
        case 'w':
            return ',';
        case 'x':
            return 'b';
        case 'y':
            return 't';
        case 'z':
            return '/';
        case '.':
            return 'e';
        case '?':
            return '[';
        case '\'':
            return 'q';
        case '!':
            return '!';
        default:
            return original;
    }
}