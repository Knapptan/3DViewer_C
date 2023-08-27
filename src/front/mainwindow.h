#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QApplication>
#include <QButtonGroup>
#include <QColorDialog>
#include <QDateTime>
#include <QFileDialog>
#include <QFont>
#include <QImage>
#include <QImageWriter>
#include <QMainWindow>
#include <QMessageBox>
#include <QString>
#include <QWheelEvent>
#include <QtOpenGL/QtOpenGL>
#include <iostream>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();
  Ui::MainWindow *ui;

  double value_scale = 1;

 private slots:
  void on_pushButton_open_file_clicked();
  std::string findName(QString filePath);

  void on_verticalSlider_size_valueChanged(int value);
  void on_spinBox_size_valueChanged(int arg1);
  void on_pushButton_param_clicked();
  void load_settings();
  void save_settings();
  void scale_default();
  void on_radioButton_projection_center_clicked();
  void on_radioButton_projection_parallel_clicked();

  void on_radioButton_ribs_line_clicked();
  void on_radioButton_ribs_dotted_clicked();
  void on_radioButton_dot_invis_clicked();
  void on_radioButton_dot_round_clicked();
  void on_radioButton_dot_square_clicked();

  void on_spinBox_dot_valueChanged(int value);
  void on_spinBox_line_valueChanged(int value);

  void on_horizontalSlider_move_x_valueChanged(int value);
  void on_horizontalSlider_move_y_valueChanged(int value);
  void on_horizontalSlider_move_z_valueChanged(int value);

  void on_horizontalSlider_spin_x_valueChanged(int value);
  void on_horizontalSlider_spin_y_valueChanged(int value);
  void on_horizontalSlider_spin_z_valueChanged(int value);

  void on_pushButton_change_dot_color_clicked();
  void on_pushButton_change_line_color_clicked();
  void on_pushButton_change_background_color_clicked();
  void on_pushButton_screenshot_clicked();
  void on_pushButton_gif_clicked();
  void on_pushButton_info_clicked();

  void wheelEvent(QWheelEvent *event);
  void mouseMoveEvent(QMouseEvent *event);

 private:
  QString filePath;
  QString file_name;
  QButtonGroup *vert_type_buttons;
  QButtonGroup *edge_type_buttons;
  QButtonGroup *projection_type_buttons;
  QSettings *settings;
  QPoint cur_pos;
  QPoint new_pos;
};
#endif  // MAINWINDOW_H
