#pragma once

/// @brief Handles mapping a sequence of dots and dashes into an alphanumeric character.
class MorseTree {
   public:
    static constexpr char Incomplete = 0;
    static constexpr char Error = 1;

    MorseTree();
    virtual char getLetterForMorseSymbols(const char* morseSymbols);

   private:
    struct Node {
        char letter;
        Node* dot;
        Node* dash;
    };

    Node* _root = createMorseTree();
    Node* createNode(char letter);
    Node* createNode(char letter, Node* dot, Node* dash);
    Node* createMorseTree();
};