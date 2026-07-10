#include "Role.h"
#include <QRandomGenerator>

Role::Role()
{
    m_roleList
            << "Детектив"
            << "Хозяин"
            << "Путешественник"
            << "Рыцарь"
            << "Певец"
            << "Повар"
            << "Посыльный"
            << "Учёный";
}

QString Role::getRandomRole() const
{
    int index = QRandomGenerator::global()->bounded(m_roleList.size());
    return m_roleList[index];
}

QVector<QString> Role::roleList() const
{
    return m_roleList;
}