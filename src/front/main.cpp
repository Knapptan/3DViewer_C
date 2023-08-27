#include <QApplication>

#include "mainwindow.h"

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);
  QFont font;
  font.setFamily("Arial");
  QApplication::setFont(font);
  MainWindow w;
  w.show();
  return a.exec();
}
