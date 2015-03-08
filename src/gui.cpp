#include "gui.h"

#include <QApplication>
#include <QKeyEvent>
#include <QPainter>
#include <QFontMetrics>

#include "metronome.h"

const int BIG_INCREMENT = 10;
const int SMALL_INCREMENT = 1;

const QColor BACKGROUND_COLOR = QColor(Qt::blue).lighter(130);
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
  case Qt::Key_Q:
  case Qt::Key_Enter:
  case Qt::Key_Return:
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
  rightCorner.lineTo(width() - cornerX, height());
  rightCorner.lineTo(width(), height() - cornerY);

  painter.fillPath(leftCorner, CORNER_COLOR);
  painter.fillPath(rightCorner, CORNER_COLOR);

  painter.restore();
}

void GUI::drawForeground(QPainter &painter) const
{
  painter.save();
  painter.translate(60, 52);

  painter.setPen(Qt::black);

  QFont font = painter.font();
  font.setPixelSize(28);
  font.setWeight(QFont::Bold);
  painter.setFont(font);

  QString leftPadding = "\x20";
  if(QFontMetrics(font).width("♩") > QFontMetrics(font).width("="))
    leftPadding.clear();

  painter.drawText(0, 0, QString("♩%1= %2").arg(leftPadding)
    .arg(m_metronome->bpm()));

  font.setPixelSize(18);
  font.setWeight(QFont::Normal);
  painter.setFont(font);

  painter.drawText(115, 0, QString("%1 beats")
    .arg(m_metronome->beats()));

  font.setPixelSize(10);
  font.setWeight(QFont::Bold);
  painter.setFont(font);

  if(m_metronome->isActive()) {
    painter.drawText(51, 17, QString("Current Beat: %1")
      .arg(m_metronome->currentBeat()));
  }
  else
    painter.drawText(41, 17, "Metronome Stopped");

  painter.restore();
}
