#ifndef VIEWER_H
#define VIEWER_H
#define _GNU_SOURCE

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct ObjectData {
  size_t count_vertexes;  // вершины
  size_t count_facets;    // грани
  double *vertexes_arr;  // масиив для записи вершин палигонов
  int *vertexes_in_facets;  // массив индесков вершин в гранях
} data;

// parsing object data
int parser(const char *address, data *object);
void count_vert_and_facet(FILE *file, data *object);
int alloc_mem(data *object);
int parse_vert_and_facet(FILE *file, data *object);
void free_object(data *object);
void print_object(data *object);

// transformations
void change_scale(data *object, const double coefficient);
// move
void move_X(data *object, const double coefficient);
void move_Y(data *object, const double coefficient);
void move_Z(data *object, const double coefficient);
// rotate
void rotate_x(data *object, double angle);
void rotate_y(data *object, double angle);
void rotate_z(data *object, double angle);

#endif  // VIEWER_H
