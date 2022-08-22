#ifndef STARTWIDGET_H
#define STARTWIDGET_H

#include <QWidget>
#include <QLabel>
#include "level.h"
#include <QPlainTextEdit>
#include <QPushButton>

/**
 * @author Grant Nations
 * @author Blake Van Dyken
 * @author Gabe Bautista
 * @author Sebastian Ramirez
 * @author David Schneider
 * @author Charles Li
 * @brief The start widget class holds all the data for the start menu screen that allows the user to start learning from the app
 **/
class startWidget : public QWidget
{
   Q_OBJECT
public:
   explicit startWidget(Level level, QWidget *parent = nullptr);

   QLabel *startLabel;
   QLabel *imageLabel;

   QPushButton* startButton;

private:
   void paintEvent(QPaintEvent* event);
   QPixmap image;

;

signals:

};

#endif // STARTWIDGET_H
