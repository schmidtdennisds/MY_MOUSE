#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define ERROR "MAP ERROR"
#define MAX_LINE_LENGTH 1000

typedef struct s_fields {
    char wall;
    char empty;
    char path;
    char entry;
    char exit;
} fields;

typedef struct s_maze {
    int rows;
    int cols;
    fields* fields;
    char** map;
} maze;


maze* read_map_file(char* filename) {
    FILE *file;
    char line[MAX_LINE_LENGTH];
    maze* maze = malloc(sizeof(maze));

    file = fopen(filename, "r");

    if (file == NULL) {
        printf(ERROR);
        return NULL;
    }

    // Read lines from the file using fgets
    if (fgets(line, MAX_LINE_LENGTH, file) != NULL) {
        fields* fields = malloc(sizeof(fields));
        int index = 0;
        while (line[index] != 'x') {
            index++;
        }
        char* rows_symbols = malloc(sizeof(char) * (index + 1));
        strncpy(rows_symbols, line, index);
        rows_symbols[++index] = '\n';
        printf("Rows_symbols: %s\n", rows_symbols);
        maze -> rows = atoi(rows_symbols);
        int start = index;
        int length = 0;
        while (isdigit(line[index])) {
            length++;
            index++;
        }
        char* cols_symbols = malloc(sizeof(char) * (length + 1));
        strncpy(cols_symbols, line + start, length);
        cols_symbols[length] = '\n';
        printf("Cols_symbols: %s\n", cols_symbols);
        maze -> cols = atoi(cols_symbols);
    }

    // Close the file
    fclose(file);

    return maze;
}

int main (int argc, char** argv) {
    if (argc != 2) {
        printf(ERROR);
        return 1;
    }

    read_map_file(argv[1]);

    return 0;
}