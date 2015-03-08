#include <QApplication>

#include "gui.h"

int main(int argc, char *argv[])
{
  QApplication app(argc, argv);

  app.setApplicationName(QStringLiteral("Blue Pink Metronome"));
  app.setApplicationVersion(QStringLiteral("0.0.1"));
  app.setOrganizationName(QStringLiteral("cfillion"));

  GUI *gui = new GUI;
  gui->show();

  return app.exec();
}
