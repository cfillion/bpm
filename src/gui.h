#ifndef GUI_H
#define GUI_H

#include <QWidget>

#include "metronome.h"

class GUI : public QWidget
{
  Q_OBJECT

public:
  GUI(QWidget *parent = 0);

protected:
  void paintEvent(QPaintEvent *);
  void keyPressEvent(QKeyEvent *);

private:
  void drawBackground(QPainter &painter) const;
  void drawForeground(QPainter &painter) const;

  Metronome *m_metronome;
};

#endif
