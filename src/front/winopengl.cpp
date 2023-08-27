#include "winopengl.h"

#include <iostream>

#include "mainwindow.h"
#include "ui_mainwindow.h"

#ifdef Q_OS_MAC
#include <glu.h>
#elif LINUX
#include <GL/glu.h>
#endif

WinOpenGL::~WinOpenGL() {}

WinOpenGL::WinOpenGL(QWidget* parent) : QOpenGLWidget(parent) {
  setMouseTracking(true);
}

void WinOpenGL::initializeGL() {
  initializeOpenGLFunctions();
  glEnable(GL_DEPTH_TEST);
}

void WinOpenGL::paintGL() {
  glClearColor(this->bg_red, this->bg_green, this->bg_blue, 1.0);

  // очистка буферов цвета и глубины
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glVertexPointer(3, GL_DOUBLE, 0, object.vertexes_arr);

  // перевод текущего режима матрицы в матрицу проекции
  glMatrixMode(GL_PROJECTION);

  // загружаем единичную матрицу, чтобы сбросить все предыдущие преобразования
  glLoadIdentity();

  // выброр преобразования проекции центральная/параллельная
  if (this->projection_type == 1) {  // центральная
    glFrustum(-1 * normalize_coef, 1 * normalize_coef, -1 * normalize_coef,
              1 * normalize_coef, normalize_coef, 1000 * normalize_coef);
    glTranslatef(0, 0, -2 * normalize_coef);
    glRotatef(30, 1, 0, 0);
  } else if (this->projection_type == 0) {  // параллельная
    glOrtho(-1 * normalize_coef, 1 * normalize_coef, -1 * normalize_coef,
            1 * normalize_coef, -1 * normalize_coef, 1000 * normalize_coef);
    glTranslatef(0, -normalize_coef / 2, 0);
  }

  // включаем состояние массива вершин для рисования вершин.
  glEnableClientState(GL_VERTEX_ARRAY);
  glVertexPointer(3, GL_DOUBLE, 0, object.vertexes_arr);

  // вызываем функции для рисования точек модели
  if (vert_type) {
    build_points();
  }

  // вызываем функцию, чтобы нарисовать края модели
  draw_model_edges();

  // отключаем состояние массива вершин, поскольку оно больше не нужно
  glDisableClientState(GL_VERTEX_ARRAY);
}

void WinOpenGL::resetTransformations() {
  rotate_x = 0.0f;
  rotate_y = 0.0f;
  rotate_z = 0.0f;
  move_x = 0.0f;
  move_y = 0.0f;
  move_z = 0.0f;
  scale = 50.0f;
  update();
}

void WinOpenGL::build_points() {
  if (vert_type == 1) {
    glEnable(GL_POINT_SMOOTH);
  }
  vertices_size = (double)vert_size;
  glPointSize(this->vertices_size);
  glColor3f(p_red, p_green, p_blue);
  glDrawArrays(GL_POINTS, 0, object.count_vertexes);
  if (vert_type == 1) {
    glDisable(GL_POINT_SMOOTH);
  }
}

void WinOpenGL::set_normalize_coef() {
  // Инициализируем normalize_coef отрицательным значением, чтобы гарантировать,
  // что он будет корректно обновлен
  normalize_coef = -10;

  for (size_t i = 0; i < object.count_vertexes * 3; i++) {
    // проверяем, больше ли абсолютное значение текущей координаты вершины
    // чем текущий normalize_coef Если это так, обновляем normalize_coef с
    // помощью нового максимальнго абсолютного значения
    if (abs(object.vertexes_arr[i]) > normalize_coef) {
      normalize_coef = abs(object.vertexes_arr[i]);
    }
  }
}

void WinOpenGL::get_data() {
  free_object(&object);
  QByteArray inArr = filename.toUtf8();
  char* cLine = inArr.data();
  parser(cLine, &object);

  set_normalize_coef();
  update();
}

void WinOpenGL::updateProjection() {
  set_normalize_coef();
  update();
}

void WinOpenGL::draw_model_edges() {
  if (this->edge_type == 1) {
    glEnable(GL_LINE_STIPPLE);
    glLineStipple(1, 0x00FF);
  }
  edges_thickness = (double)edge_size;
  glLineWidth(this->edges_thickness);

  // режим многоугольника, чтобы рисовать линии вместо заполненных
  // многоугольников
  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

  // меняем цвет краев на белый
  glColor3f(e_red, e_green, e_blue);
  // умножение на два, потому что мы рисуем
  glDrawElements(GL_LINES, object.count_facets * 2, GL_UNSIGNED_INT,
                 object.vertexes_in_facets);

  if (this->edge_type == 1) {
    glDisable(GL_LINE_STIPPLE);
  }
}
