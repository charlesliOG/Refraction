#include "mainwindow.h"
#include "levelinfowidget.h"
#include "ui_mainwindow.h"
#include <QTimer>
#include <gamewidget.h>

MainWindow::MainWindow(GameModel &model, QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    levelOneWidget = new GameWidget(model.levels.at(1), this);
    levelTwoWidget = new GameWidget(model.levels.at(2), this);
    levelThreeWidget = new GameWidget(model.levels.at(3), this);
    levelOneInfoWidget = new LevelInfoWidget(model.levels.at(1).levelInfo, this);
    levelTwoInfoWidget = new LevelInfoWidget(model.levels.at(2).levelInfo, this);
    levelThreeInfoWidget = new LevelInfoWidget(model.levels.at(3).levelInfo, this);

    laserInfoWidget = new LevelInfoWidget(model.levels.at(4).levelInfo, this);
    laserWidget = new LaserWidget(this);

    levelTwoWidget->setVisible(false);
    levelOneWidget->setVisible(false);
    levelThreeWidget->setVisible(false);
    laserWidget->setVisible(false);

    levelOneInfoWidget->setVisible(false);
    levelTwoInfoWidget->setVisible(false);
    levelThreeInfoWidget->setVisible(false);
    laserInfoWidget->setVisible(false);


    startMenu = new startWidget(model.levels.at(0), this);

    connect(levelOneWidget->nextLevelButton, &QPushButton::clicked, &model, &GameModel::iterateLevel);

    connect(levelTwoWidget->nextLevelButton, &QPushButton::clicked, &model, &GameModel::iterateLevel);

    connect(levelThreeWidget->nextLevelButton, &QPushButton::clicked, &model, &GameModel::iterateLevel);

    connect(startMenu->startButton, &QPushButton::clicked, &model,
            &GameModel::iterateLevel);



    connect(&model, &GameModel::sendLevel, this, &MainWindow::updateLevel);

    // Get the input text when the sumbit answer button is pressed
    connect(levelOneWidget->submitAnswerButton, &QPushButton::clicked, levelOneWidget,
            &GameWidget::getInputText);

    connect(levelTwoWidget->submitAnswerButton, &QPushButton::clicked, levelTwoWidget,
            &GameWidget::getInputText);

    connect(levelThreeWidget->submitAnswerButton, &QPushButton::clicked, levelThreeWidget,
            &GameWidget::getInputText);

    // Check submitted answer from user
    connect(levelOneWidget, &GameWidget::sendText, &model, &GameModel::checkAnswer);

    connect(levelTwoWidget, &GameWidget::sendText, &model, &GameModel::checkAnswer);

    connect(levelThreeWidget, &GameWidget::sendText, &model, &GameModel::checkAnswer);

    // Update remaining attempts when user submits an answer
    connect(&model, &GameModel::updateAttempts, levelOneWidget,
            &GameWidget::showRemainingAttempts);

    connect(&model, &GameModel::updateAttempts, levelTwoWidget,
            &GameWidget::showRemainingAttempts);

    connect(&model, &GameModel::updateAttempts, levelThreeWidget,
            &GameWidget::showRemainingAttempts);

    // Handle when a level is passed or failed
    connect(&model, &GameModel::passLevel, levelOneWidget,
            &GameWidget::passFailLevel);

    connect(&model, &GameModel::passLevel, levelTwoWidget,
            &GameWidget::passFailLevel);

    connect(&model, &GameModel::passLevel, levelThreeWidget,
            &GameWidget::passFailLevel);

    connect(&model, &GameModel::hidePassBanner, levelTwoWidget,
            &GameWidget::onHidePassBanner);

    connect(&model, &GameModel::hidePassBanner, levelThreeWidget,
            &GameWidget::onHidePassBanner);

    connect(levelOneInfoWidget->showProblemButton, &QPushButton::clicked, this,
            &MainWindow::showLevel);
    connect(levelTwoInfoWidget->showProblemButton, &QPushButton::clicked, this,
            &MainWindow::showLevel);
    connect(levelThreeInfoWidget->showProblemButton, &QPushButton::clicked, this,
            &MainWindow::showLevel);
    connect(laserInfoWidget->showProblemButton, &QPushButton::clicked, this,
            &MainWindow::showLevel);

    connect(levelOneWidget->showInfoButton, &QPushButton::clicked, this,
            &MainWindow::showInfo);
    connect(levelTwoWidget->showInfoButton, &QPushButton::clicked, this,
            &MainWindow::showInfo);
    connect(levelThreeWidget->showInfoButton, &QPushButton::clicked, this,
            &MainWindow::showInfo);
    connect(laserWidget->infoButton, &QPushButton::clicked, this,
            &MainWindow::showInfo);

}

void MainWindow::updateLevel(int level) {
    currLevel = level;
    if (level == 1) {
        startMenu->setVisible(false);
        levelOneInfoWidget->setVisible(true);

    }
    if (level == 2) {
        levelOneWidget->setVisible(false);
        levelTwoInfoWidget->setVisible(true);

    }
    if (level == 3) {
        levelTwoWidget->setVisible(false);
        levelThreeInfoWidget->setVisible(true);
    }
    if (level == 4){
        levelThreeWidget->setVisible(false);
        laserInfoWidget->setVisible(true);
    }
}

void MainWindow::showLevel() {
    if(currLevel == 1)
    {
        levelOneInfoWidget->setVisible(false);
        levelOneWidget->setVisible(true);
    }
    if(currLevel == 2)
    {
        levelTwoInfoWidget->setVisible(false);
        levelTwoWidget->setVisible(true);
    }
    if(currLevel == 3)
    {
        levelThreeInfoWidget->setVisible(false);
        levelThreeWidget->setVisible(true);
    }
    if(currLevel == 4){
        laserInfoWidget->setVisible(false);
        laserWidget->setVisible(true);
    }
}

void MainWindow::showInfo() {
    if(currLevel == 1)
    {
        levelOneWidget->setVisible(false);
        levelOneInfoWidget->setVisible(true);
    }
    if(currLevel == 2)
    {
        levelTwoWidget->setVisible(false);
        levelTwoInfoWidget->setVisible(true);
    }
    if(currLevel == 3){
        levelThreeWidget->setVisible(false);
        levelThreeInfoWidget->setVisible(true);
    }
    if(currLevel == 4){
        laserWidget->setVisible(false);
        laserInfoWidget->setVisible(true);
    }
}

MainWindow::~MainWindow() { delete ui; }
