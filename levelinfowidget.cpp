#include "levelinfowidget.h"
#include <QStyleOption>
#include <QPainter>

/**
 * @brief LevelInfoWidget::LevelInfoWidget The constructor for the LevelInfoWidget.
 * @param image The image that contains the info for the level
 * @param parent The QWidget parent
 */
LevelInfoWidget::LevelInfoWidget(QPixmap image, QWidget* parent) : QWidget{parent}, image(image)
{
    // resize to fit the parent container
    this->resize(parent->width(), parent->height());

    // set background to white
    this->setStyleSheet("background-color: white;");

    int imgWidth = image.width();
    int imgHeight = image.height();

    // set up the image label to show the info image
    imageLabel = new QLabel(this);
    imageLabel->resize(imgWidth, imgHeight);
    imageLabel->move(this->width()/2 - imgWidth/2, this->height()/2 - imgHeight/2 - 60);
    imageLabel->setPixmap(image);

    // set up the button to show the problem
    showProblemButton = new QPushButton(this);
    showProblemButton->setText("Show level");
    showProblemButton->move(this->width()/2 - showProblemButton->width()/2, imageLabel->y() + imageLabel->height() + showProblemButton->height() + 10);
    // set the stylesheet for the button
    showProblemButton->setStyleSheet("QPushButton{"
                                   "background-color: #FFFFFF;"
                                   "border-radius: 4px;"
                                   "border: 1px solid black;"
                                   "font-size: 14px;"
                                   "color: #000000;"
                                   "margin: 0;"
                                   "font-weight: 500;"
                                   "min-width: 10px;"
                                   "outline: none;"
                                   "padding: 4px 8px;}"                                 
                                   );


    // set the cursor to pointer when on the button
    showProblemButton->setCursor(QCursor(Qt::PointingHandCursor));

}

/**
 * @brief LevelInfoWidget::paintEvent The paint event for the widget
 * @param event
 */
void LevelInfoWidget::paintEvent(QPaintEvent *event) {
    // make this widget able to accept a stylesheet
    QStyleOption opt;
    opt.initFrom(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);

}
