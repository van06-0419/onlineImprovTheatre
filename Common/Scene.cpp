#include "Scene.h"
#include <QRandomGenerator>

Scene::Scene()
{
    m_sceneList << "Кафе в маленьком городке" << "Тёмный ночной лес" << "Старинный замок"
                << "Рыночная площадь" << "Морской причал" << "Горный домик";
}

QString Scene::getRandomScene() const
{
    if (m_sceneList.isEmpty()) return "Неизвестная сцена";

    int idx = QRandomGenerator::global()->bounded(m_sceneList.size());
    return m_sceneList[idx];
}