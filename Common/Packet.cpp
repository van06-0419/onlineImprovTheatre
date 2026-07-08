#include "Packet.h"

const QString Packet::SPLIT = "|";

Packet::Packet()
{}

Packet::Packet(const QString& cmd, const QString& data)
    : m_cmd(cmd), m_data(data)
{}

QString Packet::toString() const
{
    return m_cmd + SPLIT + m_data;
}

Packet Packet::fromString(const QString& str)
{
    int pos = str.indexOf(SPLIT);
    if (pos == -1) return Packet();
    return Packet(str.left(pos), str.mid(pos + 1));
}

QString Packet::command() const
{
    return m_cmd;
}

QString Packet::data() const
{
    return m_data;
}
