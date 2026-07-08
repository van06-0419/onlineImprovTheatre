#ifndef MESSAGE_H
#define MESSAGE_H

#include <QString>
#include <QDateTime>

class Message
{
public:
    Message();
    Message(const QString& sender, const QString& content);

    QString sender() const;
    QString content() const;
    QString time() const;

private:
    QString m_sender;
    QString m_content;
    QString m_time;
};

#endif // MESSAGE_H
