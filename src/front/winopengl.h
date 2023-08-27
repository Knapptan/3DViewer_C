#ifndef WINOPENGL_H
#define WINOPENGL_H

#include <math.h>

#include <QColor>
#include <QMessageBox>
#include <QObject>
#include <QOpenGLBuffer>
#include <QOpenGLExtraFunctions>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLWidget>
#include <QTimer>
#include <QWheelEvent>
#include <QWidget>

#include "mainwindow.h"

extern "C" {
#include "../back/viewer_back.h"
}

class WinOpenGL : public QOpenGLWidget, protected QOpenGLFunctions {
  Q_OBJECT

 public:
  explicit WinOpenGL(QWidget *parent = nullptr);
  virtual ~WinOpenGL();

  double p_red = 1, p_green = 1, p_blue = 1;
  double e_red = 1, e_green = 1, e_blue = 1;
  double bg_red = 0, bg_green = 0, bg_blue = 0;

  ObjectData object = {0, 0, NULL, NULL};
  QString filename;
  GLfloat normalize_coef;

  double vertices_size = 1;
  double edges_thickness = 1;
  double scale = 100;

  double move_x = 100;
  double move_y = 100;
  double move_z = 100;
  double rotate_x = 100;
  double rotate_y = 100;
  double rotate_z = 100;

  // 0 параллельное 1 центральное
  int projection_type = 1;

  // 0 скрытые 1 круглые 2 квадратные
  int vert_type = 1;
  int vert_size = 1;

  // 0 сплошные 1 пунктирные
  int edge_type = 0;
  int edge_size = 1;

  void set_normalize_coef();
  void build_points();
  void get_data();
  void draw_model_edges();
  void updateProjection();

 private:
  void initializeGL() override;
  void paintGL() override;
  void resetTransformations();
};

#endif  // WINOPENGL_H
