#ifndef AUDIOMANAGER_H
#define AUDIOMANAGER_H
#include <QObject>
#include <QAudioSource>
#include <QAudioSink>
#include <QIODevice>

class AudioManager : public QObject
{
    Q_OBJECT
public:
    explicit AudioManager(QObject *parent = nullptr);
    ~AudioManager();
    void startMicrophone();
    void stopMicrophone();
    bool isMicrophoneOn() const;

public slots:
    void playAudio(const QByteArray &data);

signals:
    void audioCaptured(const QByteArray &data);

private slots:
    void captureAudio();

private:
    QAudioSource *m_audioSource;
    QAudioSink *m_audioSink;
    QIODevice *m_audioInput;
    QIODevice *m_audioOutput;
    bool m_micOn;
    void setupAudio();
};

#endif