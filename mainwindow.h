#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "gamemodel.h"
#include "model.h"
#include "gamewidget.h"
#include "startwidget.h"
#include "levelinfowidget.h"
#include "laserwidget.h"
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

/**
 * @author Grant Nations
 * @author Blake Van Dyken
 * @author Gabe Bautista
 * @author Sebastian Ramirez
 * @author David Schneider
 * @author Charles Li
 * @brief The MainWindow class changes the current widget based on the level
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(GameModel &model, QWidget *parent = nullptr);
    ~MainWindow();


public slots:
    void updateLevel(int level);

private:
    Ui::MainWindow *ui;
    startWidget *startMenu;
    GameWidget *levelOne;
    GameWidget *levelTwo;
    GameWidget *levelThree;
    LevelInfoWidget *levelOneInfoWidget;
    LevelInfoWidget *levelTwoInfoWidget;
    LevelInfoWidget *levelThreeInfoWidget;

    GameWidget *levelOneWidget;
    GameWidget *levelTwoWidget;
    GameWidget *levelThreeWidget;

    LevelInfoWidget *laserInfoWidget;
    LaserWidget *laserWidget;
    int currLevel = 0;


public slots:
    void showLevel();
    void showInfo();
};
#endif // MAINWINDOW_H



