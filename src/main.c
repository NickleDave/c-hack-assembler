#include <stdio.h>


int main(int argc, char *argv[]) {
    if (argc != 2) {
        puts("Invalid number of arguments");
    }

    FILE *fp;
    fp = fopen(argv[1], "r");
    if (fp == NULL) {
        puts("Cannot open file");
    }

    char ch;
    while (1)
    {
        ch = fgetc(fp);
        if (ch == EOF)
            break;
        else 
            printf("%c", ch);
    }
}