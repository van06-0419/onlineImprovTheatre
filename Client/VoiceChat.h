#ifndef VOICECHAT_H
#define VOICECHAT_H

#include <QObject>
#include <QAudioSource>
#include <QAudioSink>
#include <QIODevice>
#include <QMap>
#include <QByteArray>

class Client;

// Аудиоплеер для одного собеседника
class AudioPlayer : public QObject
{
    Q_OBJECT
public:
    AudioPlayer(QObject* parent = nullptr);
    ~AudioPlayer();
    void feedData(const QByteArray& pcmData);
    void stop();

private:
    QAudioSink* m_audioSink = nullptr;
    QIODevice*  m_ioDevice  = nullptr;
};

// Основной класс голосового чата
class VoiceChat : public QObject
{
    Q_OBJECT
public:
    explicit VoiceChat(Client* client, QObject* parent = nullptr);
    ~VoiceChat();

    void startMicrophone();
    void stopMicrophone();
    bool isMicrophoneOn() const;

    // Вызывается из ChatWindow при получении VOICE-пакета
    void handleVoicePacket(const QString& sender, const QByteArray& base64Pcm);

signals:
    void microphoneStateChanged(bool on);

private slots:
    void onAudioDataReady();

private:
    Client*       m_client;
    QAudioSource* m_audioInput  = nullptr;
    QIODevice*    m_audioDevice = nullptr;
    bool          m_micOn       = false;

    // Один аудиоплеер на каждого собеседника
    QMap<QString, AudioPlayer*> m_players;
};

#endif // VOICECHAT_H