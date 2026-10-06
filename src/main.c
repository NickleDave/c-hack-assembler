#define _GNU_SOURCE

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "assemble.h"


char *extract_filename(const char *filename);


int main(int argc, char *argv[]) {
    /* parse args */
    if (argc < 2) {
        puts("Invalid number of arguments");
    }

    FILE *fp;
    
    fp = fopen(argv[1], "r");
    if (fp == NULL) {
        perror("Assembler");
        return EXIT_FAILURE;
    }

    bool verbose;
    if (argv[2]) {
        verbose = true;
    } else {
        verbose = false;
    }

    /* determine output filename */
    // do this before `assemble`, in case it fails
    char* basename = extract_filename(argv[1]);
    char* out_filename;
    asprintf(&out_filename, "%s%s", basename, ".hack");
    FILE *out_fp;
    out_fp = fopen(out_filename, "w");
    if (out_fp == NULL) {
        perror("NULL when opening `out_filename`");
    }

    /* assemble */
    BinaryStrings binstrings = assemble(fp, verbose);

    /* write to output file */
    for (int i = 0; i < binstrings.num_strings; i++) {
        fprintf(out_fp, "%s", binstrings.strings[i]);
    }

    /* free binary strings */
    for (int i = 0; i < binstrings.num_strings; i++) {
        free(binstrings.strings[i]);
    }
    free(binstrings.strings);

}


/*
`extract_filename` taken from zakuArbor's implementation of the Hack assembler:
https://codeberg.org/zakuArbor/hackAssembler/src/commit/29f1a5f1087a0c394e960aec8056896faab945c0/assembler.c#L19
*/
#define NAME_MAX 250

/*
* Extract the filename without the extension
*
* @param char *file: the file name with the extension
* @return: 
*    * the file name without the extension (allocates memory)
*     * NULL iff file is not a valid assembly file        
*/
char *extract_filename(const char *file) {
    char *file_token;
    char *ext_token;
    char *filename;
    char *bname;
    char file_cpy[NAME_MAX];
    char *path;

    if (!file) {
        return NULL;
    }

    if (strlen(file) > NAME_MAX) {
        fprintf(stderr, "File name exceeds %d characters\n", NAME_MAX);
        return NULL;
    }

    if ( ! (path = strndup(file, strlen(file))) ) {
        return NULL;
    }

    if ( ! (bname = basename(path)) ) {
        free(path);
        return NULL;
    }
    
    strncpy(file_cpy, bname, strlen(bname));
    file_cpy[strlen(file)] = '\0';

    free(path);
    
    file_token = strtok(file_cpy, ".");
    ext_token  = strtok(NULL, ".");

    if (file_token && ext_token && strncmp(ext_token, "asm", 3) == 0) {
        if (!(filename = (char *)malloc(sizeof(char) * strlen(file_token) + 1))) {
            
            return NULL;
        }
        if (!strncpy(filename, file_token, strlen(file_token))) {
            perror("strcpy");
            free(filename);
            
            return NULL;
        }
        filename[strlen(file_token)] = '\0';
        
        return filename;
    }
    fprintf(stderr, "File must have .asm extension\n");
    return NULL;
}

