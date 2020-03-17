#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

FILE* openFile(const char* filename)
{
    FILE* filePointer;

    filePointer = fopen(filename, "r");

    if (NULL == filePointer) {
        printf("Failed to open %s.\n", filename);

        exit(3);
    }

    return filePointer;
}

int getFileSize(const char* filename)
{
    struct stat st;

    stat(filename, &st);

    return st.st_size;
}

char* fillBuffer(FILE* filePointer, int size)
{
    char* buffer = NULL;

    buffer = (char*) malloc(size * sizeof(char));

    fread(buffer, 1, size, filePointer);
    fclose(filePointer);

    return buffer;
}

GLchar* readFromFile(const char* filename)
{
    GLchar* buffer;

    buffer = fillBuffer(openFile(filename), getFileSize(filename));

    return buffer;
}