#include "metronome.h"

#include <QAudioOutput>

const int MINUTE_IN_MS = 60 * 1000;

Metronome::Metronome(QObject *parent)
  : QObject(parent), m_currentSample(0)
{
  m_timer.setTimerType(Qt::PreciseTimer);
  connect(&m_timer, &QTimer::timeout, this, &Metronome::performBeat);

  m_accent.setFileName(QStringLiteral(":/accent"));
  m_accent.open(QIODevice::ReadOnly);

  m_tick.setFileName(QStringLiteral(":/tick"));
  m_tick.open(QIODevice::ReadOnly);

  QAudioFormat format;
  format.setSampleRate(44100);
  format.setChannelCount(1);
  format.setSampleSize(16);
  format.setCodec(QStringLiteral("audio/pcm"));
  format.setByteOrder(QAudioFormat::LittleEndian);
  format.setSampleType(QAudioFormat::SignedInt);

  m_output = new QAudioOutput(format, this);
  connect(m_output, &QAudioOutput::stateChanged,
    this, &Metronome::outputStateChanged);

  setBpm(120);
  setBeats(4);

  start();
}

void Metronome::setBpm(const int newBpm)
{
  m_bpm = qBound(10, newBpm, 350);

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
  m_timer.start(0);

  Q_EMIT changed();
}

void Metronome::stop()
{
  m_timer.stop();

  Q_EMIT changed();
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

  m_currentSample = m_currentBeat == 1 ? &m_accent : &m_tick;

  m_output->suspend();
  m_output->start(m_currentSample);

  const int interval = MINUTE_IN_MS / m_bpm;
  if(m_timer.interval() != interval)
    m_timer.setInterval(interval);

  Q_EMIT changed();
}

void Metronome::outputStateChanged(QAudio::State newState)
{
  switch(newState) {
  case QAudio::IdleState:
  case QAudio::SuspendedState:
    m_output->stop();
    break;
  case QAudio::StoppedState:
    m_currentSample->seek(0);
    break;
  default:
    break;
  }
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
