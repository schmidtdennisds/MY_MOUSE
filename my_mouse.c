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

queue_end_points* add_point_to_queue(queue_end_points* queue_end_points, point* point) {
    queue* new_end_of_queue = malloc(sizeof(queue));
    new_end_of_queue->point = point;
    new_end_of_queue->next= NULL;
    queue_end_points->last->next = new_end_of_queue;
    queue_end_points->last = new_end_of_queue;
    return queue_end_points;
}

queue_end_points* add_possible_steps_to_queue(queue_end_points* queue_end_points, maze* maze) {
    point* visited_point = queue_end_points->first->point;
    //maze->map[visited_point->y][visited_point->x] = maze->fields->path;

    point* up = malloc(sizeof(point));
    up->x = visited_point->x;
    up->y = visited_point->y - 1;
    up->previous = visited_point;

    point* left = malloc(sizeof(point));
    left->x = visited_point->x - 1;
    left->y = visited_point->y;
    left->previous = visited_point;

    point* right = malloc(sizeof(point));
    right->x = visited_point->x + 1;
    right->y = visited_point->y;
    right->previous = visited_point;

    point* down = malloc(sizeof(point));
    down->x = visited_point->x;
    down->y = visited_point->y + 1;
    down->previous = visited_point;


    if (is_point_empty(maze, up)) {
        //printf("up\n");
        queue_end_points = add_point_to_queue(queue_end_points, up);
    } 
    if (is_point_empty(maze, left)) {
        //printf("left\n");
        queue_end_points = add_point_to_queue(queue_end_points, left);
    }
    if (is_point_empty(maze, right)) {
        //printf("right\n");
        queue_end_points = add_point_to_queue(queue_end_points, right);
    }if (is_point_empty(maze, down)) {
        //printf("down\n");
        queue_end_points = add_point_to_queue(queue_end_points, down);
    }

    queue_end_points->first = queue_end_points->first->next;
    return queue_end_points;
}

int print_solution_path(maze* maze, point* end_point) {
    int count = 0;
    while (end_point->previous != NULL) {
        maze->map[end_point->y][end_point->x] = maze->fields->path;
        end_point = end_point->previous;
        count++;
    }
    return count - 1;
}

int main (int argc, char** argv) {
    if (argc != 2) {
        printf(ERROR);
        return 1;
    }

    maze* maze = read_map_file(argv[1]);
    //print_maze(maze);

    point* entry_point = search_for_entry(maze);
    entry_point->previous = NULL;
    //printf("Entry (%d,%d)\n", entry_point->x, entry_point->y);

    point* exit_point = search_for_exit(maze);
    //printf("Entry (%d,%d)\n", exit_point->x, exit_point->y);

    queue* queue = malloc(sizeof(queue));
    queue->point = entry_point;
    queue->next = NULL;

    queue_end_points* queue_end_points = malloc(sizeof(queue_end_points));
    queue_end_points->first = queue;
    queue_end_points->last = queue;


    while(queue_end_points->first->point->x != exit_point->x || queue_end_points->first->point->y != exit_point->y) {
        queue_end_points = add_possible_steps_to_queue(queue_end_points, maze);
        //print_maze(maze);
        //sleep(1);
    }

    int solution_count = print_solution_path(maze, queue_end_points->first->point);

    while (queue_end_points->first->next != NULL) {
        free(queue_end_points->first->point);
        queue_end_points->first = queue_end_points->first->next;
    }
    free (queue_end_points);

    maze->map[entry_point->y][entry_point->x] = maze->fields->entry;
    maze->map[exit_point->y][exit_point->x] = maze->fields->exit;

    fields* fields = maze->fields;
    printf("%dx%d%c%c%c%c%c\n", maze->rows, maze->cols, fields->wall, fields->empty, fields->path, fields->entry, fields->exit);
    print_maze(maze);
    printf("%d STEPS!\n", solution_count);

    free(maze->map);
    free(maze->fields);
    free(maze);
    
    return 0;
}