#include "Role.h"
#include <QRandomGenerator>

Role::Role()
{
    m_roleList << "Детектив" << "Хозяин" << "Путешественник" << "Рыцарь" << "Певец"
               << "Повар" << "Посыльный" << "Учёный";
}

QString Role::getRandomRole() const
{
    if (m_roleList.isEmpty()) return "Неизвестный персонаж";

    int idx = QRandomGenerator::global()->bounded(m_roleList.size());
    return m_roleList[idx];
}