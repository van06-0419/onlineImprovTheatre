#ifndef COMMONDEF_H
#define COMMONDEF_H

#include <QtGlobal>

// Идентификатор пользователя
enum UserType
{
    ACTOR,     // Актер
    AUDIENCE   // Зритель
};

// Ограничения по количеству участников
const int MAX_ACTOR = 5;
const int MAX_AUDIENCE = 10;

// Порт сервера
const quint16 SERVER_PORT = 8888;

#endif // COMMONDEF_H