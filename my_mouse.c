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
} point;

typedef struct s_queue {
    point* point;
    struct s_queue* next;
} queue;

void parse_first_line(maze* maze, char* line) {
    fields* fields = malloc(sizeof(fields));
    int index = 0;
    while (line[index] != 'x') {
        index++;
    }
    char* rows_symbols = malloc(sizeof(char) * (index + 1));
    strncpy(rows_symbols, line, index);
    rows_symbols[++index] = '\n';
    //printf("Rows_symbols: %s\n", rows_symbols);
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
    //printf("Cols_symbols: %s\n", cols_symbols);
    maze -> cols = atoi(cols_symbols);
    fields -> wall = line[index++];
    fields -> empty = line[index++];
    fields -> path = line[index++];
    fields -> entry = line[index++];
    fields -> exit = line[index];
    maze -> fields = fields;
}

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
        parse_first_line(maze, line);
    }

    char** map = malloc(sizeof(char*) * maze->rows);
    int index = 0;
   
    while (fgets(line, MAX_LINE_LENGTH, file) != NULL) {
        map[index] = malloc(sizeof(char) * maze->cols + 1);
        strncpy(map[index],line, maze->cols);
        map[index][maze->cols] = '\0';
        index++;
    }
    maze->map = map;

    fclose(file);

    return maze;
}

void print_maze(maze* maze) {
    for (int i = 0; i < maze->rows; i++) {
        for (int k = 0; k < maze->cols; k++) {
            printf("%c", maze->map[i][k]);
        }
        printf("\n");
    }
}

point* search_for_entry(maze* maze) {
    for (int i = 0; i < maze->rows; i++) {
        for (int k = 0; k < maze->cols; k++) {
            if (maze->map[i][k] == maze->fields->entry) {
                point* entry_point = malloc(sizeof(point));
                entry_point->x = k;
                entry_point->y = i;
                return entry_point;
            }
        }
    }
    printf(ERROR);
    return NULL;
}

point* search_for_exit(maze* maze) {
    for (int i = 0; i < maze->rows; i++) {
        for (int k = 0; k < maze->cols; k++) {
            if (maze->map[i][k] == maze->fields->exit) {
                point* entry_point = malloc(sizeof(point));
                entry_point->x = k;
                entry_point->y = i;
                return entry_point;
            }
        }
    }
    printf(ERROR);
    return NULL;
}

bool is_point_empty(maze* maze, point* point) {
    if (point->x < 0 || point->x > maze->cols || point->y < 0 || point->y > maze-> rows) {
        return false;
    }

    if (maze->map[point->y][point->x] == maze->fields->empty) {
        return true;
    } else if (maze->map[point->y][point->x] == maze->fields->exit) {
        return true;
    } else {
        return false;
    }
}

queue* add_point_to_queue(queue* end_of_queue, point* point) {
    queue* new_end_of_queue = malloc(sizeof(queue));
    new_end_of_queue->point = point;
    new_end_of_queue->next= NULL;
    end_of_queue->next = new_end_of_queue;
    end_of_queue = new_end_of_queue;
    return end_of_queue;
}

queue* add_possible_steps_to_queue(queue* start_of_queue, maze* maze) {
    point* visited_point = start_of_queue->point;
    maze->map[visited_point->y][visited_point->x] = maze->fields->path;

    queue* end_of_queue = start_of_queue;
    while (end_of_queue->next != NULL) {
        end_of_queue = end_of_queue->next;
    }

    point* up = malloc(sizeof(point));
    up->x = visited_point->x;
    up->y = visited_point->y - 1;

    point* left = malloc(sizeof(point));
    left->x = visited_point->x - 1;
    left->y = visited_point->y;

    point* right = malloc(sizeof(point));
    right->x = visited_point->x + 1;
    right->y = visited_point->y;

    point* down = malloc(sizeof(point));
    down->x = visited_point->x;
    down->y = visited_point->y + 1;


    if (is_point_empty(maze, up)) {
        //printf("up\n");
        end_of_queue = add_point_to_queue(end_of_queue, up);
    } 
    if (is_point_empty(maze, left)) {
        //printf("left\n");
        end_of_queue = add_point_to_queue(end_of_queue, left);
    }
    if (is_point_empty(maze, right)) {
        //printf("right\n");
        end_of_queue = add_point_to_queue(end_of_queue, right);
    }if (is_point_empty(maze, down)) {
        //printf("down\n");
        end_of_queue = add_point_to_queue(end_of_queue, down);
    }

    return start_of_queue->next;
}

int main (int argc, char** argv) {
    if (argc != 2) {
        printf(ERROR);
        return 1;
    }

    maze* maze = read_map_file(argv[1]);
    //print_maze(maze);

    point* entry_point = search_for_entry(maze);
    //printf("Entry (%d,%d)\n", entry_point->x, entry_point->y);

    point* exit_point = search_for_exit(maze);
    //printf("Entry (%d,%d)\n", exit_point->x, exit_point->y);

    queue* queue = malloc(sizeof(queue));
    queue->point = entry_point;
    queue->next = NULL;

    while(queue->point->x != exit_point->x || queue->point->y != exit_point->y) {
        queue = add_possible_steps_to_queue(queue, maze);
        print_maze(maze);
        sleep(1);
    }

    maze->map[entry_point->y][entry_point->x] = maze->fields->entry;

    print_maze(maze);
    
    return 0;
}