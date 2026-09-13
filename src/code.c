#include <stdbool.h>
#include <string.h>

#include "code.h"


const MnemonicBitsPair DestStrBitsMap[] = {
    {.mnemonic="",    .bits=0b0000000000000000},
    {.mnemonic="M",   .bits=0b0000000000001000},
    {.mnemonic="D",   .bits=0b0000000000010000},
    {.mnemonic="MD",  .bits=0b0000000000011000},
    {.mnemonic="A",   .bits=0b0000000000100000},
    {.mnemonic="AM",  .bits=0b0000000000101000},
    {.mnemonic="AD",  .bits=0b0000000000110000},
    {.mnemonic="AMD", .bits=0b0000000000111000},
};

const MnemonicBitsPair CompStrBitsMap[] = {
    {.mnemonic="0",   .bits=0b0000101010000000},
    {.mnemonic="1",   .bits=0b0000111111000000},
    {.mnemonic="-1",  .bits=0b0000111010000000},
    {.mnemonic="D",   .bits=0b0000001100000000},
    {.mnemonic="A",   .bits=0b0000110000000000},
    {.mnemonic="!D",  .bits=0b0000001101000000},
    {.mnemonic="!A",  .bits=0b0000110001000000},
    {.mnemonic="-D",  .bits=0b0000001111000000},
    {.mnemonic="-A",  .bits=0b0000110011000000},
    {.mnemonic="D+1", .bits=0b0000011111000000},
    {.mnemonic="A+1", .bits=0b0000110111000000},
    {.mnemonic="D-1", .bits=0b0000001110000000},
    {.mnemonic="A-1", .bits=0b0000110010000000},
    {.mnemonic="D+A", .bits=0b0000000010000000},
    {.mnemonic="D-A", .bits=0b0000010011000000},
    {.mnemonic="A-D", .bits=0b0000000111000000},
    {.mnemonic="D&A", .bits=0b0000000000000000},
    {.mnemonic="D|A", .bits=0b0000010101000000},
    {.mnemonic="M",   .bits=0b0001110000000000},
    {.mnemonic="!M",  .bits=0b0001110001000000},
    {.mnemonic="-M",  .bits=0b0001110011000000},
    {.mnemonic="M+1", .bits=0b0001110111000000},
    {.mnemonic="M-1", .bits=0b0001110010000000},
    {.mnemonic="D+M", .bits=0b0001000010000000},
    {.mnemonic="D-M", .bits=0b0001010011000000},
    {.mnemonic="M-D", .bits=0b0001000111000000},
    {.mnemonic="D&M", .bits=0b0001000000000000},
    {.mnemonic="D|M", .bits=0b0001010101000000},
};


const MnemonicBitsPair JumpStrBitsMap[] = {
    {.mnemonic="",    .bits=0b0000000000000000},
    {.mnemonic="JGT", .bits=0b0000000000000001},
    {.mnemonic="JEQ", .bits=0b0000000000000010},
    {.mnemonic="JGE", .bits=0b0000000000000011},
    {.mnemonic="JLT", .bits=0b0000000000000100},
    {.mnemonic="JNE", .bits=0b0000000000000101},
    {.mnemonic="JLE", .bits=0b0000000000000110},
    {.mnemonic="JMP", .bits=0b0000000000000111},
};


bool is_valid_dest(char * token) {
    bool matched = false;
    for (int i = 0; i<8; i++) {
        if (!strcmp(token, DestStrBitsMap[i].mnemonic)) {
            matched = true;
        }
    }
    return matched;
}


bool is_valid_comp(char * token) {
    bool matched = false;
    for (int i = 0; i<28; i++) {
        if (!strcmp(token, CompStrBitsMap[i].mnemonic)) {
            matched = true;
        }
    }
    return matched;
}


bool is_valid_jump(char * token) {
    bool matched = false;
    for (int i = 0; i<8; i++) {
        if (!strcmp(token, JumpStrBitsMap[i].mnemonic)) {
            matched = true;
        }
    }
    return matched;
}
