#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

char* readFromFile(char* filename);
char* fillBuffer(FILE* filePointer, int size);
FILE* openFile(char* filename);
int getFileSize(char* filename);

char* readFromFile(char* filename)
{
    return fillBuffer(openFile(filename), getFileSize(filename));
}

char* fillBuffer(FILE* filePointer, int size)
{
    char* buffer = NULL;

    buffer = (char*) malloc(size * sizeof(char));

    fread(buffer, 1, size, filePointer);
    fclose(filePointer);

    return buffer;
}

FILE* openFile(char* filename)
{
    FILE* filePointer;

    filePointer = fopen(filename, "r");

    if (NULL == filePointer) {
        printf("Failed to open %s.\n", filename);

        exit(5);
    }

    return filePointer;
}

int getFileSize(char* filename)
{
    struct stat st;

    stat(filename, &st);

    return st.st_size;
}