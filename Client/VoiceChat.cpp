#include "VoiceChat.h"
#include "Client.h"
#include "../Common/Packet.h"
#include <QAudioDevice>
#include <QMediaDevices>
#include <QDebug>

// ============ AudioPlayer ============
AudioPlayer::AudioPlayer(QObject* parent) : QObject(parent)
{
    QAudioFormat format;
    format.setSampleRate(16000);
    format.setChannelCount(1);
    format.setSampleFormat(QAudioFormat::Int16);

    m_audioSink = new QAudioSink(format, this);
    m_ioDevice  = m_audioSink->start();
}

AudioPlayer::~AudioPlayer()
{
    stop();
}

void AudioPlayer::feedData(const QByteArray& pcmData)
{
    if (m_ioDevice && m_audioSink->state() != QAudio::StoppedState)
        m_ioDevice->write(pcmData);
}

void AudioPlayer::stop()
{
    if (m_audioSink)
        m_audioSink->stop();
}

// ============ VoiceChat ============
VoiceChat::VoiceChat(Client* client, QObject* parent)
    : QObject(parent), m_client(client)
{
}

VoiceChat::~VoiceChat()
{
    stopMicrophone();
    qDeleteAll(m_players);
    m_players.clear();
}

void VoiceChat::startMicrophone()
{
    if (m_micOn) return;

    QAudioFormat format;
    format.setSampleRate(16000);
    format.setChannelCount(1);
    format.setSampleFormat(QAudioFormat::Int16);

    QAudioDevice info = QMediaDevices::defaultAudioInput();
    if (!info.isFormatSupported(format))
    {
        qWarning() << "The format is not supported by the microphone";
        return;
    }

    m_audioInput  = new QAudioSource(info, format, this);
    m_audioDevice = m_audioInput->start();

    if (m_audioDevice)
    {
        connect(m_audioDevice, &QIODevice::readyRead,
                this, &VoiceChat::onAudioDataReady);
        m_micOn = true;
        emit microphoneStateChanged(true);
        qDebug() << "Microphone is on";
    }
}

void VoiceChat::stopMicrophone()
{
    if (!m_micOn) return;
    if (m_audioInput)
    {
        m_audioInput->stop();
        m_audioInput->deleteLater();
        m_audioInput  = nullptr;
        m_audioDevice = nullptr;
    }
    m_micOn = false;
    emit microphoneStateChanged(false);
    qDebug() << "Microphone is off";
}

bool VoiceChat::isMicrophoneOn() const
{
    return m_micOn;
}

void VoiceChat::onAudioDataReady()
{
    if (!m_audioDevice) return;

    QByteArray pcmData = m_audioDevice->readAll();
    if (pcmData.isEmpty()) return;

    // Кодируем PCM в base64, чтобы безопасно передать через текстовый протокол
    QString base64 = QString::fromUtf8(pcmData.toBase64());
    QString payload = m_client->userName() + "|" + base64;

    m_client->sendPacket(Packet("VOICE", payload));
}

void VoiceChat::handleVoicePacket(const QString& sender, const QByteArray& base64Pcm)
{
    // Не воспроизводим свой же голос (защита от эха)
    if (sender == m_client->userName()) return;

    AudioPlayer* player = m_players.value(sender, nullptr);
    if (!player)
    {
        player = new AudioPlayer(this);
        m_players.insert(sender, player);
    }

    // Декодируем base64 обратно в PCM
    QByteArray pcmData = QByteArray::fromBase64(base64Pcm);
    player->feedData(pcmData);
}