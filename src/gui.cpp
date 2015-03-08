#include "gui.h"

#include <QApplication>
#include <QPainter>
#include <QKeyEvent>

#include "metronome.h"

const int BIG_INCREMENT = 10;
const int SMALL_INCREMENT = 1;

const QColor BACKGROUND_COLOR = QColor(Qt::blue).lighter(125);
const QColor CORNER_COLOR = QColor(Qt::magenta);

GUI::GUI(QWidget *parent)
  : QWidget(parent)
{
  m_metronome = new Metronome(this);

  connect(m_metronome, SIGNAL(changed()), this, SLOT(update()));

  setFixedSize(300, 100);
  setWindowTitle(qApp->applicationName());
  setWindowFlags(Qt::WindowStaysOnTopHint);
}

void GUI::paintEvent(QPaintEvent *)
{
  QPainter painter(this);
  painter.setRenderHint(QPainter::HighQualityAntialiasing);

  drawBackground(painter);
  drawForeground(painter);
}

void GUI::keyPressEvent(QKeyEvent *event)
{
  const int key = event->key();

  switch(key) {
  case Qt::Key_Space:
    m_metronome->toggle();
    return;
  case Qt::Key_Up:
    m_metronome->setBpm(m_metronome->bpm() + BIG_INCREMENT);
    return;
  case Qt::Key_Down:
    m_metronome->setBpm(m_metronome->bpm() - BIG_INCREMENT);
    return;
  case Qt::Key_Right:
    m_metronome->setBpm(m_metronome->bpm() + SMALL_INCREMENT);
    return;
  case Qt::Key_Left:
    m_metronome->setBpm(m_metronome->bpm() - SMALL_INCREMENT);
    return;
  case Qt::Key_T:
    m_metronome->tap();
    return;
  case Qt::Key_Escape:
    close();
    return;
  }

  if(key > Qt::Key_0 && key <= Qt::Key_9)
    m_metronome->setBeats(key - Qt::Key_0);
}

void GUI::drawBackground(QPainter &painter) const
{
  painter.save();
  painter.fillRect(0, 0, width(), height(), BACKGROUND_COLOR);

  const int cornerX = height();
  const int cornerY = height() / 1.5;

  QPainterPath leftCorner;
  leftCorner.lineTo(cornerX, 0);
  leftCorner.lineTo(0, cornerY);

  QPainterPath rightCorner;
  rightCorner.moveTo(width(), height());
  rightCorner.lineTo(width() - cornerX, height()); // horizontal
  rightCorner.lineTo(width(), height() - cornerY); // vertical

  painter.fillPath(leftCorner, CORNER_COLOR);
  painter.fillPath(rightCorner, CORNER_COLOR);

  painter.restore();
}

void GUI::drawForeground(QPainter &painter) const
{
  painter.save();
  painter.translate(60, 52);

  QFont font = painter.font();
  font.setPointSize(28);
  font.setWeight(QFont::Bold);
  painter.setFont(font);

  painter.drawText(0, 0, QString("♩= %1")
    .arg(m_metronome->bpm()));

  font.setPointSize(18);
  font.setWeight(QFont::Normal);
  painter.setFont(font);

  painter.drawText(110, 0, QString("%1 beats")
    .arg(m_metronome->beats()));

  font.setPointSize(10);
  font.setWeight(QFont::Bold);
  painter.setFont(font);

  painter.drawText(11, 15, QString("Current Beat: %1 | Volume: 100%").arg(m_metronome->currentBeat()));

  painter.restore();
}
