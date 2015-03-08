#include "metronome.h"

#include <QSound>

const int MINUTE_IN_MS = 60 * 1000;

Metronome::Metronome(QObject *parent)
  : QObject(parent)
{
  m_timer.setSingleShot(true);
  QObject::connect(&m_timer, &QTimer::timeout, this, &Metronome::performBeat);

  m_accent = new QSound(QStringLiteral(":/accent"), this);
  m_tick = new QSound(QStringLiteral(":/tick"), this);

  setBpm(120);
  setBeats(4);

  // let's wait until the event loop is up and running before starting the timer
  // calling start() right now would cause the very first beat to be shorter
  QMetaObject::invokeMethod(this, "start", Qt::QueuedConnection);
}

void Metronome::setBpm(const int newBpm)
{
  m_bpm = qBound(30, newBpm, 300);

  Q_EMIT changed();
}

void Metronome::setBeats(const int newBeats)
{
  m_beats = qBound(1, newBeats, 9);

  Q_EMIT changed();
}

void Metronome::start()
{
  m_currentBeat = 0;
  performBeat();
}

void Metronome::stop()
{
  m_timer.stop();
}

void Metronome::toggle()
{
  if(isActive())
    stop();
  else
    start();
}

void Metronome::performBeat()
{
  if(m_currentBeat++ >= m_beats)
    m_currentBeat = 1;

  if(m_currentBeat == 1)
    m_accent->play();
  else
    m_tick->play();

  m_timer.start(MINUTE_IN_MS / m_bpm);
  Q_EMIT changed();
}

bool Metronome::tap()
{
  if(m_tap.isValid()) {
    setBpm(MINUTE_IN_MS / m_tap.elapsed());
    m_tap.invalidate();

    return true;
  }

  m_tap.start();
  return false;
}
