#ifndef MY_MOUSE
#define MY_MOUSE


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <unistd.h>

#define ERROR "MAP ERROR"
#define MAX_LINE_LENGTH 1000

typedef struct s_fields {
    char wall;
    char empty;
    char path;
    char entry;
    char exit;
    char visited;
    char in_queue;
} fields;

typedef struct s_maze {
    int rows;
    int cols;
    fields* fields;
    char** map;
} maze;

typedef struct s_point {
    int x;
    int y;
    struct s_point* previous;
} point;

typedef struct s_queue {
    point* point;
    struct s_queue* next;
} queue;

typedef struct s_queue_end_points {
    queue* first;
    queue* last;
} queue_end_points;

#endif