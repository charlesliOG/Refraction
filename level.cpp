#include "level.h"

Level::Level(QPixmap image, QPixmap infoImage, QPixmap solutionImage, QString answerLabelText, vector<double> answers) :
    levelImage(image), levelInfo(infoImage), levelSolution(solutionImage), answerLabelText(answerLabelText), answers(answers)
{

}
