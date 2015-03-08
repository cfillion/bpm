#include "metronome.h"

#include <QAudioOutput>

const int MINUTE_IN_MS = 60 * 1000;

Metronome::Metronome(QObject *parent)
  : QObject(parent)
{
  m_timer.setTimerType(Qt::PreciseTimer);
  QObject::connect(&m_timer, &QTimer::timeout, this, &Metronome::performBeat);

  m_accent.setFileName(QStringLiteral(":/accent"));
  m_accent.open(QIODevice::ReadOnly);

  m_tick.setFileName(QStringLiteral(":/tick"));
  m_tick.open(QIODevice::ReadOnly);

  QAudioFormat format;
  format.setSampleRate(44100);
  format.setChannelCount(2);
  format.setSampleSize(16);
  format.setCodec("audio/pcm");
  format.setByteOrder(QAudioFormat::LittleEndian);
  format.setSampleType(QAudioFormat::SignedInt);

  m_output = new QAudioOutput(format, this);

  setBpm(120);
  setBeats(4);

  // let's wait until the event loop is up and running before starting the timer
  // calling start() right now would cause the very first beat to be shorter
  QMetaObject::invokeMethod(this, "start", Qt::QueuedConnection);
}

void Metronome::setBpm(const int newBpm)
{
  m_bpm = qBound(10, newBpm, 999);
  m_timer.setInterval(MINUTE_IN_MS / m_bpm);

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
  m_timer.start();

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

  m_output->start(m_currentBeat == 1 ? &m_accent : &m_tick);

  m_accent.seek(0);
  m_tick.seek(0);

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
