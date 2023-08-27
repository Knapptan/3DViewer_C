#include "viewer_back.h"

void move_X(data *object, const double coefficient) {
  for (size_t i = 0; i < object->count_vertexes * 3; i += 3) {
    object->vertexes_arr[i] += coefficient;
  }
}

void move_Y(data *object, const double coefficient) {
  for (size_t i = 1; i < object->count_vertexes * 3; i += 3) {
    object->vertexes_arr[i] += coefficient;
  }
}

void move_Z(data *object, const double coefficient) {
  for (size_t i = 2; i < object->count_vertexes * 3; i += 3) {
    object->vertexes_arr[i] += coefficient;
  }
}

void change_scale(data *object, const double coefficient) {
  if (coefficient != 0) {
    for (size_t i = 0; i < object->count_vertexes * 3; i++) {
      object->vertexes_arr[i] *= coefficient;
    }
  }
}

void rotate_x(data *object, double angle) {
  double old_y, old_z;
  angle *= M_PI / 180;
  for (size_t i = 1; i < object->count_vertexes * 3; i += 3) {
    old_y = object->vertexes_arr[i];
    old_z = object->vertexes_arr[i + 1];
    object->vertexes_arr[i] = old_y * cos(angle) + old_z * sin(angle);
    object->vertexes_arr[i + 1] = -old_y * sin(angle) + old_z * cos(angle);
  }
}

void rotate_y(data *object, double angle) {
  double old_x, old_z;
  angle *= M_PI / 180;
  for (size_t i = 0; i < object->count_vertexes * 3; i += 3) {
    old_x = object->vertexes_arr[i];
    old_z = object->vertexes_arr[i + 2];
    object->vertexes_arr[i] = old_x * cos(angle) + old_z * sin(angle);
    object->vertexes_arr[i + 2] = -old_x * sin(angle) + old_z * cos(angle);
  }
}

void rotate_z(data *object, double angle) {
  double old_x, old_y;
  angle *= M_PI / 180;
  for (size_t i = 0; i < object->count_vertexes * 3; i += 3) {
    old_x = object->vertexes_arr[i];
    old_y = object->vertexes_arr[i + 1];
    object->vertexes_arr[i] = old_x * cos(angle) + old_y * sin(angle);
    object->vertexes_arr[i + 1] = -old_x * sin(angle) + old_y * cos(angle);
  }
}
