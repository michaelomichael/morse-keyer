#include "morse-tree.h"

#include <stdlib.h>
#include <string.h>

#define DOT createNode
#define DASH createNode

constexpr char MorseTree::Incomplete;
constexpr char MorseTree::Error;

MorseTree::MorseTree(void) {}

MorseTree::Node* MorseTree::createNode(char letter, Node* dot, Node* dash) {
    Node* node = (Node*)malloc(sizeof(Node));
    node->letter = letter;
    node->dot = dot;
    node->dash = dash;
    return node;
}

MorseTree::Node* MorseTree::createNode(char letter) { return createNode(letter, NULL, NULL); }

MorseTree::Node* MorseTree::createMorseTree() {
    // clang-format off
    return createNode(
        Incomplete,
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
                    DASH(Incomplete, // ..--
                        DOT(Incomplete, // ..--.
                            DOT('?'), // ..--..
                            NULL),
                        DASH('2')))), // ..---
            DASH('a', // .-
                DOT('r', // .-.
                    DOT('l'), // .-..
                    DASH(Incomplete, // .-.-
                        DOT(Incomplete, // .-.-.
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
                        DASH(Incomplete, // -.-.-
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
                    DOT(Incomplete, // ---.
                        DOT('8'), // ---..
                        NULL),
                    DASH(Incomplete, // ----
                        DOT('9'), // ----.
                        DASH('0')))))); // -----
    // clang-format on
}

char MorseTree::getLetterForMorseSymbols(const char* morseSymbols) {
    Node* node = _root;
    for (unsigned int i = 0; i < strlen(morseSymbols); i++) {
        if (node == NULL) {
            return Incomplete;
        }

        if (morseSymbols[i] == '.') {
            node = node->dot;
        } else if (morseSymbols[i] == '-') {
            node = node->dash;
        } else {
            return Error;
        }
    }

    if (node == NULL) {
        return Incomplete;
    } else {
        return node->letter;
    }
}
