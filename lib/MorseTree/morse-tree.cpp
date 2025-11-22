#include "morse-tree.h"

#include <stdlib.h>
#include <string.h>

#define DOT createNode
#define DASH createNode

struct Node {
    char letter;
    Node* dot;
    Node* dash;
};

Node* createNode(char letter, Node* dot, Node* dash) {
    Node* node = (Node*)malloc(sizeof(Node));
    node->letter = letter;
    node->dot = dot;
    node->dash = dash;
    return node;
}

Node* createNode(char letter) { return createNode(letter, NULL, NULL); }

Node* createMorseTree() {
    // clang-format off
    return createNode(
        NONE,
        DOT('e', // .
            DOT('i', // ..
                DOT('s', // ...
                    DOT('h', // ....
                        DOT('5'), // .....
                        DASH('4')), // ....-
                    DASH('v', // ...-
                        NULL,
                        DASH('3'))), // ...--
                DASH('u', // ..-
                    DOT('f'), // ..-.
                    DASH(NONE, // ..--
                        DOT(NONE, // ..--.
                            DOT('?'), // ..--..
                            NULL),
                        DASH('2')))), // ..---
            DASH('a', // .-
                DOT('r', // .-.
                    DOT('l'), // .-..
                    DASH(NONE, // .-.-
                        DOT(NONE, // .-.-.
                            NULL,
                            DASH('.')), // .-.-.-
                        NULL)),
                DASH('w', // .--
                    DOT('p'), // .--.
                    DASH ('j', // .---
                        NULL,
                        DASH('1', // .----
                            DOT('\''), // .----.
                            NULL))))),
        DASH('t', // -
            DOT('n', // -.
                DOT('d', // -..
                    DOT('b', // -...
                        DOT('6'), // -....
                        NULL),
                    DASH('x')), // -..-
                DASH('k', // -.-,
                    DOT('c', // -.-.
                        NULL,
                        DASH(NONE, // -.-.-
                            NULL,
                            DASH('!'))), // -.-.--
                    DASH('y'))), // -.--
            DASH('m', // --
                DOT('g', // --.
                    DOT('z', // --..
                        DOT('7'), // --...
                        NULL),
                    DASH('q')), // --.-
                DASH('o', // ---
                    DOT(NONE, // ---.
                        DOT('8'), // ---..
                        NULL),
                    DASH(NONE, // ----
                        DOT('9'), // ----.
                        DASH('0')))))); // -----
    // clang-format on
}

Node* root = createMorseTree();

char getLetterForMorseSymbols(const char* morseSymbols) {
    Node* node = root;
    for (int i = 0; i < strlen(morseSymbols); i++) {
        if (node == NULL) {
            return NONE;
        }

        if (morseSymbols[i] == '.') {
            node = node->dot;
        } else if (morseSymbols[i] == '-') {
            node = node->dash;
        } else {
            return ERROR;
        }
    }

    if (node == NULL) {
        return NONE;
    } else {
        return node->letter;
    }
}
