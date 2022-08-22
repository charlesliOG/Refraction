#ifndef LEVEL1_H
#define LEVEL1_H

#include <QObject>
#include "level.h"

#include <Box2D/Dynamics/b2World.h>

class Level1 : public QObject, public Level
{
    Q_OBJECT
public:
    explicit Level1(QObject *parent = nullptr);

signals:

};

#endif // LEVEL1_H
