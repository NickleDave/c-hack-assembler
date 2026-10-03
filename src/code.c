#include <stdbool.h>
#include <string.h>

#include "code.h"


const MnemonicBitsPair DestStrBitsMap[] = {
    {.mnemonic="",    .bits="000"},
    {.mnemonic="M",   .bits="001"},
    {.mnemonic="D",   .bits="010"},
    {.mnemonic="MD",  .bits="011"},
    {.mnemonic="A",   .bits="100"},
    {.mnemonic="AM",  .bits="101"},
    {.mnemonic="AD",  .bits="110"},
    {.mnemonic="AMD", .bits="111"},
};


const MnemonicBitsPair CompStrBitsMap[] = {
    {.mnemonic="0",   .bits="0101010"},
    {.mnemonic="1",   .bits="0111111"},
    {.mnemonic="-1",  .bits="0111010"},
    {.mnemonic="D",   .bits="0001100"},
    {.mnemonic="A",   .bits="0110000"},
    {.mnemonic="!D",  .bits="0001101"},
    {.mnemonic="!A",  .bits="0110001"},
    {.mnemonic="-D",  .bits="0001111"},
    {.mnemonic="-A",  .bits="0110011"},
    {.mnemonic="D+1", .bits="0011111"},
    {.mnemonic="A+1", .bits="0110111"},
    {.mnemonic="D-1", .bits="0001110"},
    {.mnemonic="A-1", .bits="0110010"},
    {.mnemonic="D+A", .bits="0000010"},
    {.mnemonic="D-A", .bits="0010011"},
    {.mnemonic="A-D", .bits="0000111"},
    {.mnemonic="D&A", .bits="0000000"},
    {.mnemonic="D|A", .bits="0010101"},
    {.mnemonic="M",   .bits="1110000"},
    {.mnemonic="!M",  .bits="1110001"},
    {.mnemonic="-M",  .bits="1110011"},
    {.mnemonic="M+1", .bits="1110010"},
    {.mnemonic="M-1", .bits="1110010"},
    {.mnemonic="D+M", .bits="1000010"},
    {.mnemonic="D-M", .bits="1010011"},
    {.mnemonic="M-D", .bits="1000111"},
    {.mnemonic="D&M", .bits="1000000"},
    {.mnemonic="D|M", .bits="1010101"},
};


const MnemonicBitsPair JumpStrBitsMap[] = {
    {.mnemonic="",    .bits="000"},
    {.mnemonic="JGT", .bits="001"},
    {.mnemonic="JEQ", .bits="010"},
    {.mnemonic="JGE", .bits="011"},
    {.mnemonic="JLT", .bits="100"},
    {.mnemonic="JNE", .bits="101"},
    {.mnemonic="JLE", .bits="110"},
    {.mnemonic="JMP", .bits="111"},
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
