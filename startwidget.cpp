#include "startwidget.h"
#include <QPainter>
#include <QColor>

#include <Box2D/Dynamics/b2Body.h>
#include <Box2D/Dynamics/b2Fixture.h>
#include <Box2D/Collision/Shapes/b2PolygonShape.h>
#include <QStyleOption>

/**
 * @brief startWidget::startWidget The constructor for a startWidget object
 * @param level The Level object to use in construction of startWidget parameters
 * @param parent The QWidget parent
 */
startWidget::startWidget(Level level, QWidget *parent)
   : QWidget{parent}
   , image(level.levelImage)
{
   // set the start screen to the widget/screen size
   this->resize(parent->width(), parent->height());
   this->setStyleSheet("background-color: white;");

   imageLabel = new QLabel(this); // add new label that we can draw on

   // resize the label to the start screen image size and position it accordingly
   imageLabel->resize(image.width(), image.height());
   imageLabel->move(this->width()/2 - image.width()/2, this->height()/2 - image.height()/2);
   imageLabel->setPixmap(image);

   // add a stylish looking button that starts the game
   startButton = new QPushButton(this);
   startButton->setText("");
   startButton->resize(140,40);
   startButton->move(this->width()/2 - startButton->width()/2 - 350, this->height()/2 - startButton->height()/2 + 305); // move to middle of screen with offset
   startButton->setFlat(true);
   startButton->setCursor(QCursor(Qt::PointingHandCursor));
}

void startWidget::paintEvent(QPaintEvent *event) {
   QStyleOption opt;
   opt.initFrom(this); // init style of widget
   QPainter p(this);
   style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}
