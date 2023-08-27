#include "mainwindow.h"

#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);
  settings = new QSettings("21school", "3D_Viewer", this);
  load_settings();
}

MainWindow::~MainWindow() {
  save_settings();
  free_object(&ui->openGLWidget->object);
  delete settings;
  delete ui;
}

void MainWindow::on_pushButton_open_file_clicked() {
  filePath = QFileDialog::getOpenFileName(this, tr("Open file"), ".",
                                          tr("Object files (*.obj)"));
  ui->openGLWidget->filename = filePath;
  ui->spinBox_size->setValue(100);
  ui->verticalSlider_size->setValue(100);
  ui->openGLWidget->get_data();
  QString tmp_str = NULL;
  QString fileName = QString::fromStdString(findName(filePath));
  ui->label_file_name->setText("Имя файла: " + fileName);
  ui->label_file_path->setText("Путь к файлу: " + filePath);
  ui->label_number_peaks->setText(
      "Кол-во вершин: " +
      tmp_str.setNum(ui->openGLWidget->object.count_vertexes));
  ui->label_number_of_ribs->setText(
      "Кол-во ребер: " + tmp_str.setNum(ui->openGLWidget->object.count_facets));
}

std::string MainWindow::findName(QString filePath) {
  std::string utf8_text = filePath.toUtf8().constData();
  std::string file = utf8_text.substr(utf8_text.rfind('/') + 1);
  std::cout << "File name: " << file << std::endl;
  return file;
}

void MainWindow::on_verticalSlider_size_valueChanged(int value) {
  value_scale = (double)value / ui->openGLWidget->scale;
  change_scale(&ui->openGLWidget->object, value_scale);
  ui->openGLWidget->scale = value;
  ui->openGLWidget->update();
  QObject::connect(ui->verticalSlider_size, SIGNAL(valueChanged(int)),
                   ui->spinBox_size, SLOT(setValue(int)));
}

void MainWindow::on_spinBox_size_valueChanged(int value) {
  value_scale = (double)value / ui->openGLWidget->scale;
  change_scale(&ui->openGLWidget->object, value_scale);
  ui->openGLWidget->scale = value;
  ui->openGLWidget->update();
  QObject::connect(ui->spinBox_size, SIGNAL(valueChanged(int)),
                   ui->verticalSlider_size, SLOT(setValue(int)));
}

void MainWindow::scale_default() {
  ui->openGLWidget->scale = 100;
  ui->spinBox_size->setValue(100);
  ui->verticalSlider_size->setValue(100);
}

void MainWindow::on_pushButton_param_clicked() {
  ui->horizontalSlider_spin_x->setValue(100);
  ui->openGLWidget->move_y = 100;
  ui->horizontalSlider_spin_y->setValue(100);
  ui->openGLWidget->move_z = 100;
  ui->horizontalSlider_spin_z->setValue(100);
  ui->openGLWidget->rotate_x = 100;
  ui->horizontalSlider_move_x->setValue(100);
  ui->openGLWidget->rotate_y = 100;
  ui->horizontalSlider_move_y->setValue(100);
  ui->openGLWidget->rotate_z = 100;
  ui->horizontalSlider_move_z->setValue(100);

  ui->openGLWidget->vert_type = 1;
  ui->radioButton_dot_round->setChecked(1);

  ui->openGLWidget->edge_type = 0;
  ui->radioButton_ribs_line->setChecked(1);

  ui->openGLWidget->projection_type = 1;
  ui->radioButton_projection_center->setChecked(1);

  ui->openGLWidget->vert_size = 1;
  ui->spinBox_dot->setValue(1);

  ui->openGLWidget->edge_size = 1;
  ui->spinBox_line->setValue(1);

  ui->openGLWidget->p_red = 1;
  ui->openGLWidget->p_green = 1;
  ui->openGLWidget->p_blue = 1;
  ui->openGLWidget->e_red = 1;
  ui->openGLWidget->e_green = 1;
  ui->openGLWidget->e_blue = 1;
  ui->openGLWidget->bg_red = 0;
  ui->openGLWidget->bg_green = 0;
  ui->openGLWidget->bg_blue = 0;

  scale_default();

  ui->openGLWidget->updateProjection();
}

void MainWindow::load_settings() {
  if (settings->value("are_settings").toInt()) {
    char rgba_color[40];
    int projection_mode = settings->value("projection_type").toInt();
    if (projection_mode == 0) {
      ui->radioButton_projection_parallel->setChecked(true);
      ui->openGLWidget->projection_type = 0;
    } else if (projection_mode == 1) {
      ui->radioButton_projection_center->setChecked(true);
      ui->openGLWidget->projection_type = 1;
    }

    int v_mode = settings->value("v_display_method").toInt();
    if (v_mode == 0) {
      ui->radioButton_dot_invis->setChecked(true);
      ui->openGLWidget->vert_type = 0;
    } else if (v_mode == 1) {
      ui->radioButton_dot_round->setChecked(true);
      ui->openGLWidget->vert_type = 1;
    } else if (v_mode == 2) {
      ui->radioButton_dot_square->setChecked(true);
      ui->openGLWidget->vert_type = 2;
    }
    ui->spinBox_dot->setValue(settings->value("vert_size").toDouble());
    ui->openGLWidget->p_red = settings->value("v_red").toDouble();
    ui->openGLWidget->p_green = settings->value("v_green").toDouble();
    ui->openGLWidget->p_blue = settings->value("v_blue").toDouble();
    sprintf(rgba_color, "vertex-color: rgb(%d,%d,%d)",
            (int)(ui->openGLWidget->p_red * 255),
            (int)(ui->openGLWidget->p_green * 255),
            (int)(ui->openGLWidget->p_blue * 255));

    int edges_mode = settings->value("edges_type").toInt();
    if (edges_mode == 0) {
      ui->radioButton_ribs_line->setChecked(true);
      ui->openGLWidget->edge_type = 0;
    } else if (edges_mode == 1) {
      ui->radioButton_ribs_dotted->setChecked(true);
      ui->openGLWidget->edge_type = 1;
    }
    int vert_mode = settings->value("vert_type").toInt();
    if (vert_mode == 0) {
      ui->radioButton_dot_invis->setChecked(true);
      ui->openGLWidget->vert_type = 0;
    } else if (vert_mode == 1) {
      ui->radioButton_dot_round->setChecked(true);
      ui->openGLWidget->vert_type = 1;
    } else if (vert_mode == 2) {
      ui->radioButton_dot_square->setChecked(true);
      ui->openGLWidget->vert_type = 2;
    }
    ui->spinBox_line->setValue(settings->value("edges_size").toInt());
    ui->openGLWidget->e_red = settings->value("e_red").toDouble();
    ui->openGLWidget->e_green = settings->value("e_green").toDouble();
    ui->openGLWidget->e_blue = settings->value("e_blue").toDouble();
    sprintf(rgba_color, "edges-color: rgb(%d,%d,%d)",
            (int)(ui->openGLWidget->e_red * 255),
            (int)(ui->openGLWidget->e_green * 255),
            (int)(ui->openGLWidget->e_blue * 255));
    ui->openGLWidget->bg_red = settings->value("bg_red").toDouble();
    ui->openGLWidget->bg_green = settings->value("bg_green").toDouble();
    ui->openGLWidget->bg_blue = settings->value("bg_blue").toDouble();
    sprintf(rgba_color, "background-color: rgb(%d,%d,%d)",
            (int)(ui->openGLWidget->bg_red * 255),
            (int)(ui->openGLWidget->bg_green * 255),
            (int)(ui->openGLWidget->bg_blue * 255));
  }
}

void MainWindow::save_settings() {
  settings->setValue("are_settings", 1);
  settings->setValue("projection_type", ui->openGLWidget->projection_type);
  settings->setValue("vert_type", ui->openGLWidget->vert_type);
  settings->setValue("vert_size", ui->openGLWidget->vert_size);
  settings->setValue("v_red", ui->openGLWidget->p_red);
  settings->setValue("v_green", ui->openGLWidget->p_green);
  settings->setValue("v_blue", ui->openGLWidget->p_blue);
  settings->setValue("edges_type", ui->openGLWidget->edge_type);
  settings->setValue("edges_size", ui->openGLWidget->edge_size);
  settings->setValue("e_red", ui->openGLWidget->e_red);
  settings->setValue("e_green", ui->openGLWidget->e_green);
  settings->setValue("e_blue", ui->openGLWidget->e_blue);
  settings->setValue("bg_red", ui->openGLWidget->bg_red);
  settings->setValue("bg_green", ui->openGLWidget->bg_green);
  settings->setValue("bg_blue", ui->openGLWidget->bg_blue);
}

void MainWindow::on_radioButton_projection_center_clicked() {
  ui->openGLWidget->projection_type = 1;
  scale_default();
  ui->openGLWidget->updateProjection();
}

void MainWindow::on_radioButton_projection_parallel_clicked() {
  ui->openGLWidget->projection_type = 0;
  scale_default();
  ui->openGLWidget->updateProjection();
}

void MainWindow::on_radioButton_ribs_line_clicked() {
  ui->openGLWidget->edge_type = 0;
  ui->openGLWidget->update();
}

void MainWindow::on_radioButton_ribs_dotted_clicked() {
  ui->openGLWidget->edge_type = 1;
  ui->openGLWidget->update();
}

void MainWindow::on_radioButton_dot_invis_clicked() {
  ui->openGLWidget->vert_type = 0;
  ui->openGLWidget->update();
}

void MainWindow::on_radioButton_dot_round_clicked() {
  ui->openGLWidget->vert_type = 1;
  ui->openGLWidget->update();
}

void MainWindow::on_radioButton_dot_square_clicked() {
  ui->openGLWidget->vert_type = 2;
  ui->openGLWidget->update();
}

void MainWindow::on_spinBox_dot_valueChanged(int value) {
  ui->openGLWidget->vert_size = value;
  ui->openGLWidget->update();
}

void MainWindow::on_spinBox_line_valueChanged(int value) {
  ui->openGLWidget->edge_size = value;
  ui->openGLWidget->update();
}

void MainWindow::on_horizontalSlider_move_x_valueChanged(int value) {
  move_X(&ui->openGLWidget->object, (value - ui->openGLWidget->move_x) *
                                        ui->openGLWidget->normalize_coef / 100);
  ui->openGLWidget->move_x = value;
  ui->openGLWidget->update();
}

void MainWindow::on_horizontalSlider_move_y_valueChanged(int value) {
  move_Y(&ui->openGLWidget->object, (value - ui->openGLWidget->move_y) *
                                        ui->openGLWidget->normalize_coef / 100);
  ui->openGLWidget->move_y = value;
  ui->openGLWidget->update();
}

void MainWindow::on_horizontalSlider_move_z_valueChanged(int value) {
  move_Z(&ui->openGLWidget->object, (value - ui->openGLWidget->move_z) *
                                        ui->openGLWidget->normalize_coef / 100);
  ui->openGLWidget->move_z = value;
  ui->openGLWidget->update();
}

void MainWindow::on_horizontalSlider_spin_x_valueChanged(int value) {
  rotate_x(&ui->openGLWidget->object, (value - ui->openGLWidget->rotate_x));
  ui->openGLWidget->rotate_x = value;
  ui->openGLWidget->update();
}

void MainWindow::on_horizontalSlider_spin_y_valueChanged(int value) {
  rotate_y(&ui->openGLWidget->object, (value - ui->openGLWidget->rotate_y));
  ui->openGLWidget->rotate_y = value;
  ui->openGLWidget->update();
}

void MainWindow::on_horizontalSlider_spin_z_valueChanged(int value) {
  rotate_z(&ui->openGLWidget->object, (value - ui->openGLWidget->rotate_z));
  ui->openGLWidget->rotate_z = value;
  ui->openGLWidget->update();
}

void MainWindow::on_pushButton_change_dot_color_clicked() {
  QColor points_color = QColorDialog::getColor(QColor(38, 38, 38, 255));
  if (points_color.isValid()) {
    ui->openGLWidget->p_red = points_color.redF();
    ui->openGLWidget->p_green = points_color.greenF();
    ui->openGLWidget->p_blue = points_color.blueF();
    char rgba_color[40];
    sprintf(rgba_color, "points: rgb(%d,%d,%d)", points_color.red(),
            points_color.green(), points_color.blue());
    ui->openGLWidget->update();
  }
}

void MainWindow::on_pushButton_change_line_color_clicked() {
  QColor line_color = QColorDialog::getColor(QColor(38, 38, 38, 255));
  if (line_color.isValid()) {
    ui->openGLWidget->e_red = line_color.redF();
    ui->openGLWidget->e_green = line_color.greenF();
    ui->openGLWidget->e_blue = line_color.blueF();
    char rgba_color[40];
    sprintf(rgba_color, "edge: rgb(%d,%d,%d)", line_color.red(),
            line_color.green(), line_color.blue());
    ui->openGLWidget->update();
  }
}

void MainWindow::on_pushButton_change_background_color_clicked() {
  QColor bg_color = QColorDialog::getColor(QColor(38, 38, 38, 255));
  if (bg_color.isValid()) {
    ui->openGLWidget->bg_red = bg_color.redF();
    ui->openGLWidget->bg_green = bg_color.greenF();
    ui->openGLWidget->bg_blue = bg_color.blueF();
    char rgba_color[40];
    sprintf(rgba_color, "background-color: rgb(%d,%d,%d)", bg_color.red(),
            bg_color.green(), bg_color.blue());
    ui->openGLWidget->update();
  }
}

void MainWindow::wheelEvent(QWheelEvent *event) {
  QPoint numDegrees = event->angleDelta() / 120;
  ui->spinBox_size->setValue(ui->spinBox_size->value() + numDegrees.y());
}

void MainWindow::mouseMoveEvent(QMouseEvent *event) {
  new_pos = QPoint(event->globalPosition().toPoint() - cur_pos);

  if (event->buttons() & Qt::LeftButton) {
    ui->horizontalSlider_move_x->setSliderPosition(new_pos.x() / 10);
    ui->horizontalSlider_move_y->setSliderPosition(200 - new_pos.y() / 10);
    update();
  } else if (event->buttons() & Qt::RightButton) {
    ui->horizontalSlider_spin_y->setSliderPosition(200 - new_pos.x() / 10);
    ui->horizontalSlider_spin_x->setSliderPosition(new_pos.y() / 10);
    update();
  }
}

void MainWindow::on_pushButton_screenshot_clicked() {
  QFileDialog dialogConnectImage(this);
  QDateTime date = QDateTime::currentDateTime();
  QString format_date = date.toString("yyyy-MM-dd hh.mm.ss");
  QString name_screen_shot = "Screen Shot " + format_date + ".jpeg";
  QString name_img = dialogConnectImage.getSaveFileName(
      this, tr("Save a screenshot"), name_screen_shot,
      tr("image (*.jpeg *.bmp)"));
  if (name_img.length() >= 1) {
    QImage img = ui->openGLWidget->grabFramebuffer();
    img.save(name_img);
    QMessageBox messageBoxImage;
    messageBoxImage.information(0, "", "Screenshot сохранен");
  }
}

void MainWindow::on_pushButton_gif_clicked() {
  QDateTime date = QDateTime::currentDateTime();
  QString format_date = date.toString("yyyy-MM-dd_hh.mm.ss");
  QString name_gif = "Screen Shot " + format_date + ".gif";

  QString saveDirectory = QFileDialog::getExistingDirectory(
      this, tr("Save Directory"), QDir::homePath());

  if (!saveDirectory.isEmpty()) {
    QString gifFilePath = QDir(saveDirectory).filePath(name_gif);

    QList<QImage> frames;

    for (int i = 0; i < 20; ++i) {
      QImage img = ui->openGLWidget->grabFramebuffer();
      frames.append(img);
    }

    QMessageBox messageBoxImage;
    messageBoxImage.information(0, "",
                                "GIF сохранен\n это заглушка, не доделано");
  }
}

void MainWindow::on_pushButton_info_clicked() {
  QMessageBox messageBoxImage;
  messageBoxImage.information(
      0, "Info",
      "\n\nЭто учебный проект Sber School 213DViewer v1.0 что может программа:"
      "\n\nЗагружать каркасную модель из файла формата obj (поддержка только "
      "списка вершин и поверхностей)"
      "\n\nПеремещать модель на заданное расстояние относительно осей X, Y, Z"
      "\n\nПоворачивать модель на заданный угол относительно своих осей X, Y, Z"
      "\n\nМасштабировать модель на заданное значение"
      "\n\nПрограмма позволяет настраивать тип проекции (параллельная и "
      "центральная)"
      "\n\nПрограмма позволяет настраивать тип (сплошная, пунктирная), цвет и "
      "толщинуребер, способотображения (отсутствует, круг, квадрат), цвет и "
      "размервершин"
      "\n\nПрограмма позволяет выбиратьцветфона"
      "\n\nНастройки сохранятся между перезапусками программы"
      "\n\nНастойки можно сбросить до стандартных"
      "\n\nПрограмма позволяет сохранять полученные ('отрендеренные') "
      "изображения в файл в форматах bmp и jpeg"
      "\n\nПрограмма позволяет по кнопке 'Создание git' записывать небольшие "
      "'скринкасты' - текущие пользовательские аффинные преобразования "
      "загруженного объекта в gif-анимацию (640x480, 10fps, 5s)"
      "\n\nАвторы: Knapptan Styxdune"
      "\n\nhttps://github.com/Knapptan");
}
