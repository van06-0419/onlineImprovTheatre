#include "Message.h"

Message::Message()
{}

Message::Message(const QString& sender, const QString& content)
    : m_sender(sender), m_content(content)
{
    m_time = QDateTime::currentDateTime().toString("hh:mm");
}

QString Message::sender() const
{
    return m_sender;
}

QString Message::content() const
{
    return m_content;
}

QString Message::time() const
{
    return m_time;
}
