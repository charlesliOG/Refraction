#ifndef LEVEL_H
#define LEVEL_H

#include <vector>
#include <QString>
#include <QPixmap>

using std::vector;

/**
 * @author Grant Nations
 * @author Blake Van Dyken
 * @author Gabe Bautista
 * @author Sebastian Ramirez
 * @author David Schneider
 * @author Charles Li
 * @brief The Level class defines the basic content of a level, i.e. the image, answer label text,
 * the number of attempts for the level, and the correct answer(s) for the level.
*/
class Level
{
public:

    QPixmap levelImage;
    QPixmap levelInfo;
    QPixmap levelSolution;
    QString answerLabelText; // This is the text that goes in front of the answer text box (e.g. "x =" or
    // "Enter the value of x in degrees: ")
    vector<double> answers;
    int attemptsRemaining = 3;

    Level(QPixmap image, QPixmap infoImage, QPixmap,  QString answerLabelText, vector<double> answers);
};

#endif // LEVEL_H
