#ifndef ROLE_H
#define ROLE_H

#include <QString>
#include <QVector>

class Role
{
public:
    Role();
    QString getRandomRole() const;
    QVector<QString> roleList() const;
    
private:
    QVector<QString> m_roleList;
};

#endif // ROLE_H
