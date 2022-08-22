#ifndef GAMEWIDGET_H
#define GAMEWIDGET_H

#include "level.h"
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QWidget>

/**
 * @author Grant Nations
 * @author Blake Van Dyken
 * @author Gabe Bautista
 * @author Sebastian Ramirez
 * @author David Schneider
 * @author Charles Li
 * @brief The GameWidget class is the widget that holds each main game level.
 * This includes the labels, inputs, buttons, etc, that are conatined in the
 * main level screen. Note, this does not include the info screen for a level.
 */
class GameWidget : public QWidget {
    Q_OBJECT
public:
    explicit GameWidget(Level level, QWidget *parent = nullptr);
    ~GameWidget();
    QLabel *answerLabel;
    QLabel *imageLabel;
    QLineEdit *inputText;
    QPushButton *submitAnswerButton;
    QLabel *remainingAttempts;
    QLabel *passFailBanner;
    QPushButton *nextLevelButton;
    QPushButton *showInfoButton;

private:
    void paintEvent(QPaintEvent *event);
    QPixmap image;
    QPixmap imageSolution;
    QString answerLabelText;

signals:
    void sendText(QString);

public slots:
    void getInputText();
    void showRemainingAttempts(int);
    void passFailLevel(bool);
    void onHidePassBanner();
};

#endif // GAMEWIDGET_H
