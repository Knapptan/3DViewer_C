#include "viewer_back.h"

int parser(const char *address, data *object) {
  FILE *file;
  int error = 0;

  if (address == NULL || object == NULL) {
    error = 1;
  } else if (error == 0) {
    file = fopen(address, "r");
    if (file) {
      count_vert_and_facet(file, object);
      error = alloc_mem(object);
      if (error != 1) {
        fseek(file, 0, SEEK_SET);
        error = parse_vert_and_facet(file, object);
      }
      fclose(file);
    } else {
      error = 1;
    }
  }
  return error;
}

void count_vert_and_facet(FILE *file, data *object) {
  char *line = NULL;
  size_t len = 0;

  while (getline(&line, &len, file) != EOF) {
    if (strncmp(line, "v ", 2) == 0) {
      object->count_vertexes++;
    } else if (strncmp(line, "f ", 2) == 0) {
      char *facets_char_value = strtok(line + 2, " ");
      while (facets_char_value != NULL) {
        if (atoi(facets_char_value)) {
          object->count_facets++;
        }
        facets_char_value = strtok(NULL, " ");
      }
    }
  }
  if (line) {
    free(line);
    line = NULL;
  }
}

int alloc_mem(data *object) {
  int allocate_status = 0;

  if (object->count_vertexes) {
    object->vertexes_arr = calloc(object->count_vertexes * 3, sizeof(double));
  }
  if (object->count_facets) {
    // *2 потому что для отрисовки линии в glDrawElements нужно 2 вершины
    object->vertexes_in_facets = calloc(object->count_facets * 2, sizeof(int));
  }
  if (object->vertexes_arr == NULL || object->vertexes_in_facets == NULL) {
    allocate_status = 1;
  }

  return allocate_status;
}

int parse_vert_and_facet(FILE *file, data *object) {
  int error = 0;
  char *line = NULL;
  size_t len = 0;
  size_t i = 0;
  size_t vertexes_count = 0;
  size_t facets_index_count = 0;

  while (getline(&line, &len, file) != EOF) {
    if (strncmp(line, "v ", 2) == 0) {
      vertexes_count++;
      double x, y, z;
      sscanf(line, "v %lf %lf %lf", &x, &y, &z);
      object->vertexes_arr[i++] = x;
      object->vertexes_arr[i++] = y;
      object->vertexes_arr[i++] = z;
    } else if (strncmp(line, "f ", 2) == 0) {
      int first_index = 0;
      int is_first_index = 0;
      char *facets_char_value = strtok(line + 2, " ");
      while (facets_char_value != NULL) {
        int facets_int_value = atoi(facets_char_value);
        if (facets_int_value) {
          // Если индекс отрицательный, то он относится к концу списка вершин.
          // Например, -1 относится к последнему элементу.
          if (facets_int_value < 0) {
            facets_int_value += vertexes_count + 1;
          }
          object->vertexes_in_facets[facets_index_count] = facets_int_value - 1;
          if (!is_first_index) {
            first_index = facets_int_value - 1;
            is_first_index = 1;
          } else {
            object->vertexes_in_facets[++facets_index_count] =
                facets_int_value - 1;
          }
          facets_index_count++;
        }
        facets_char_value = strtok(NULL, " ");
      }
      // loop lines
      object->vertexes_in_facets[facets_index_count] = first_index;
      facets_index_count++;
    }
  }
  if (line) {
    free(line);
    line = NULL;
  }

  if (!object->vertexes_arr[object->count_vertexes * 3 - 1] ||
      !object->vertexes_in_facets[object->count_facets * 2 - 1]) {
    error = 1;
  }

  return error;
}

void free_object(data *object) {
  if (object != NULL) {
    if (object->vertexes_arr != NULL) {
      free(object->vertexes_arr);
      object->vertexes_arr = NULL;
    }
    if (object->vertexes_in_facets != NULL) {
      free(object->vertexes_in_facets);
      object->vertexes_in_facets = NULL;
    }
  }
  if (object->count_vertexes != 0) object->count_vertexes = 0;
  if (object->count_facets != 0) object->count_facets = 0;
}

void print_object(data *object) {
  //  printing vertices
  printf("count_vertexes: %ld\n", object->count_vertexes);
  for (int i = 0; i < (int)object->count_vertexes * 3; i++) {
    printf("%lf ", object->vertexes_arr[i]);
    if ((i + 1) % 3 == 0) {
      printf("\n");
    }
  }
  printf("\n");
  // printing vertex_indices
  printf("count_facets: %ld\n", object->count_facets);
  for (int i = 0; i < (int)object->count_facets * 2; i++) {
    printf("%d ", object->vertexes_in_facets[i]);
    if ((i + 1) % 6 == 0) {
      printf("\n");
    }
  }
}
