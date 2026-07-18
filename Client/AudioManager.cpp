#include "AudioManager.h"
#include <QMediaDevices>

AudioManager::AudioManager(QObject *parent)
    :
    QObject(parent),
    m_audioSource(nullptr),
    m_audioSink(nullptr),
    m_audioInput(nullptr),
    m_audioOutput(nullptr),
    m_micOn(false)

{
    setupAudio();
}

AudioManager::~AudioManager()
{
    if(m_audioSource)
    {
        m_audioSource->stop();
    }

    if(m_audioSink)
    {
        m_audioSink->stop();
    }
}

void AudioManager::setupAudio()
{
    QAudioFormat format;
    format.setSampleRate(16000);
    format.setChannelCount(1);
    format.setSampleFormat(QAudioFormat::Int16);

    m_audioSource = new QAudioSource(
            QMediaDevices::defaultAudioInput(),
            format,
            this
        );

    m_audioSink = new QAudioSink(
            QMediaDevices::defaultAudioOutput(),
            format,
            this
        );

    // 输出设备启动
    m_audioOutput =
        m_audioSink->start();
}

void AudioManager::startMicrophone()
{
    if(!m_audioSource) return;
    if(m_micOn) return;

    m_audioInput = m_audioSource->start();

    if(m_audioInput)
    {
        connect(
            m_audioInput,
            &QIODevice::readyRead,
            this,
            &AudioManager::captureAudio
        );
    }
    m_micOn = true;
}

void AudioManager::stopMicrophone()
{
    if(!m_audioSource) return;
    m_audioSource->stop();
    m_audioInput = nullptr;
    m_micOn = false;
}

bool AudioManager::isMicrophoneOn() const
{
    return m_micOn;
}

void AudioManager::captureAudio()
{
    if(!m_audioInput) return;

    QByteArray audio = m_audioInput->readAll();

    if(!audio.isEmpty())
    {
        emit audioCaptured(audio);
    }
}

void AudioManager::playAudio(const QByteArray &data)
{
    if(!m_audioOutput) return;
    m_audioOutput->write(data);
}