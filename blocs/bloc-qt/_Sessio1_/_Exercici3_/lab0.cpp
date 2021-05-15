#include <QApplication>
#include <QFrame>
#include <QLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include "MyForm.h"


int main( int argc, char ** argv)
{
  // Creem tots els components
  QApplication app(argc, argv);
  MyForm form;

  form.show();
  return app.exec();
}
