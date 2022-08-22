#include "gamemodel.h"
#include <algorithm>

using std::find;
GameModel::GameModel(QObject *parent) : QObject{parent} {

    vector<double> levelOneAns = {38.0};
    vector<double> levelTwoAns = {1.45};
    vector<double> levelThreeAns = {26.3};

    QPixmap startMenuImage = QPixmap(":/level_images/img/startMenu.jpg");

    QPixmap levelOneImage = QPixmap(":/level_images/img/level1.jpg");
    QPixmap levelOneInfoImg=QPixmap(":/level_images/img/LawOfReflectionInfo.jpg");
    QPixmap levelOneSolutionImg = QPixmap(":/level_images/img/level1Solution.jpg");

    QPixmap levelTwoInfoImg = QPixmap(":/level_images/img/level2Info.jpg");
    QPixmap levelTwoImage = QPixmap(":/level_images/img/level2.jpg");
    QPixmap levelTwoSolutionImg = QPixmap(":/level_images/img/level2Solution.jpg");

    QPixmap levelThreeInfoImage = QPixmap(":/level_images/img/level3info.jpg");
    QPixmap levelThreeImage = QPixmap(":/level_images/img/level3.jpg");
    QPixmap levelThreeSolutionImg = QPixmap(":/level_images/img/level3Solution.jpg");

    QPixmap laserLevelInfoImage = QPixmap(":/level_images/img/laserLevelInfo.jpg");

    Level startMenu(startMenuImage, levelOneInfoImg, levelOneSolutionImg, "start", levelOneAns);
    Level levelOne(levelOneImage, levelOneInfoImg, levelOneSolutionImg, "x = ", levelOneAns);
    Level levelTwo(levelTwoImage, levelTwoInfoImg, levelTwoSolutionImg,  "n2 = ",  levelTwoAns);
    Level levelThree(levelThreeImage, levelThreeInfoImage, levelThreeSolutionImg, "x = ", levelThreeAns);
    Level levelFour(levelOneImage, laserLevelInfoImage, levelOneSolutionImg, "", levelOneAns);

    levels.push_back(startMenu);
    levels.push_back(levelOne);
    levels.push_back(levelTwo);
    levels.push_back(levelThree);
    levels.push_back(levelFour);
}

void GameModel::iterateLevel() {
    currLevel++;
    emit sendLevel(currLevel);
    emit updateAttempts(3);
    emit hidePassBanner();
}

void GameModel::checkAnswer(QString answer) {
    bool *parsePass = new bool(true);
    double parsedAnswer = answer.toDouble(parsePass);
    if (parsePass) {
        if (std::count(levels.at(currLevel).answers.begin(),
                       levels.at(currLevel).answers.end(), parsedAnswer)) {
            emit passLevel(true);
        } else {
            if (levels.at(currLevel).attemptsRemaining > 1) {
                levels.at(currLevel).attemptsRemaining--;
                emit updateAttempts(levels.at(currLevel).attemptsRemaining);
            } else {
                levels.at(currLevel).attemptsRemaining--;
                emit updateAttempts(levels.at(currLevel).attemptsRemaining);
                emit passLevel(false);
            }
        }
    }

    delete parsePass;
}
