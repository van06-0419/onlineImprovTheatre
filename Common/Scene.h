#ifndef SCENE_H
#define SCENE_H

#include <QString>
#include <QVector>

class Scene
{
public:
    Scene();
    QString getRandomScene() const;

private:
    QVector<QString> m_sceneList;
};

#endif // SCENE_H
