#include <stdio.h>


int main(int argc, char *argv[]) {
    if (argc != 2) {
        puts("Invalid number of arguments");
    }

    FILE *fp;
    char *line = NULL;
    size_t len = 0;
    size_t read;
    
    fp = fopen(argv[1], "r");
    if (fp == NULL) {
        puts("Cannot open file");
    }

    while ((read = getline(&line, &len, fp)) != -1)
    {
        printf("%s", line);
    }
}