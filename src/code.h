/* 
adapted from 
https://github.com/ianmurfinxyz/hackass_hack_assembler_c/blob/master/src/decoder.c
*/
#ifndef CODE_H
#define CODE_H

#include <inttypes.h>

// no language mnemonic in 'Hack' assembly is longer than this.
#define MAX_MNEMONIC_CHAR_LENGTH 4
#define MAX_BITS_LENGTH 7

typedef const struct MnemonicBitsPair {
  const char mnemonic[MAX_MNEMONIC_CHAR_LENGTH];
  const char bits[MAX_BITS_LENGTH];
} MnemonicBitsPair;

extern const MnemonicBitsPair DestStrBitsMap[];
extern const MnemonicBitsPair CompStrBitsMap[];
extern const MnemonicBitsPair JumpStrBitsMap[];

bool is_valid_dest(char * token);
bool is_valid_comp(char * token);
bool is_valid_jump(char * token);

char* dest_bits_str_from_mnemonic(char* mnemonic);
char* comp_bits_str_from_mnemonic(char* mnemonic);
char* jump_bits_str_from_mnemonic(char* mnemonic);

#endif
