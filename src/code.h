/* 
adapted from 
https://github.com/ianmurfinxyz/hackass_hack_assembler_c/blob/master/src/decoder.c
*/
#ifndef CODE_H
#define CODE_H

#include <inttypes.h>

// no language mnemonic in 'Hack' assembly is longer than this.
#define MAX_MNEMONIC_CHAR_LENGTH 4

typedef const struct MnemonicBitsPair {
  const char mnemonic[MAX_MNEMONIC_CHAR_LENGTH];
  uint16_t bits;
} MnemonicBitsPair;

extern const MnemonicBitsPair DestStrBitsMap[];
extern const MnemonicBitsPair CompStrBitsMap[];
extern const MnemonicBitsPair JumpStrBitsMap[];

bool is_valid_dest(char * token);
bool is_valid_comp(char * token);
bool is_valid_jump(char * token);

#endif
