#ifndef GAMEMODEL_H
#define GAMEMODEL_H

#include <QObject>

#include<Box2D/Box2D.h>
#include <Box2D/Dynamics/b2World.h>
#include "level.h"
#include <QString>
#include <QPixmap>

#include <vector>

using std::vector;

/**
 * @author Grant Nations
 * @author Blake Van Dyken
 * @author Gabe Bautista
 * @author Sebastian Ramirez
 * @author David Schneider
 * @author Charles Li
* @brief The GameModel class holds the levels and keeps track of the current level. It also recieves
* data to update levels (e.g. check answers)
*/
class GameModel : public QObject
{
    Q_OBJECT
public:
    explicit GameModel(QObject *parent = nullptr);
    vector<Level>levels;
private:

    int currLevel = 0;

signals:
    void passLevel(bool);
    void updateAttempts(int);
    void sendLevel(int);
    void hidePassBanner();

public slots:
    void checkAnswer(QString answer);
    void iterateLevel();

};

#endif // GAMEMODEL_H
