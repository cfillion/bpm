#ifndef METRONOME_H
#define METRONOME_H

#include <QObject>
#include <QTimer>

class QSound;

class Metronome : public QObject
{
  Q_OBJECT

public:
  Metronome(QObject *parent = 0);

  int bpm() const { return m_bpm; }
  void setBpm(const int newBpm);

  int beats() const { return m_beats; }
  void setBeats(const int newBeats);

  int currentBeat() const { return m_currentBeat; }
  bool isActive() const { return m_timer.isActive(); }

public Q_SLOTS:
  void start();
  void stop();
  void toggle();

Q_SIGNALS:
  void changed();

private Q_SLOTS:
  void performBeat();

private:
  QTimer m_timer;

  int m_bpm;
  int m_currentBeat;
  int m_beats;

  QSound *m_accent;
  QSound *m_tick;
};

#endif
