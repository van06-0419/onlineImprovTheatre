#ifndef PACKET_H
#define PACKET_H

#include <QString>

class Packet
{
public:
    Packet();
    Packet(const QString& cmd, const QString& data);

    QString toString() const;
    static Packet fromString(const QString& str);

    QString command() const;
    QString data() const;

private:
    QString m_cmd;
    QString m_data;
    static const QString SPLIT;
};

#endif // PACKET_H
