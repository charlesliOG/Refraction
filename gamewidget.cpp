#include "gamewidget.h"
#include <QPainter>
#include <QColor>

#include <Box2D/Dynamics/b2Body.h>
#include <Box2D/Dynamics/b2Fixture.h>
#include <Box2D/Collision/Shapes/b2PolygonShape.h>
#include <QStyleOption>

/**
 * @brief GameWidget::GameWidget Constructor for GameWidget object
 * @param level The Level object to construct the widget parameters with
 * @param parent The QWidget parent
 */
GameWidget::GameWidget(Level level, QWidget *parent)
    : QWidget{parent}, image(level.levelImage), imageSolution(level.levelSolution), answerLabelText(level.answerLabelText)
{
    this->resize(parent->width(), parent->height());
    this->setStyleSheet("background-color: white;");

    // Setup level image
    imageLabel = new QLabel(this);
    imageLabel->resize(image.width(), image.height());
    imageLabel->move(this->width()/2 - image.width()/2, this->height()/2 - image.height()/2);
    imageLabel->setPixmap(image);

    // Setup answer label
    answerLabel = new QLabel(this);
    answerLabel->setText(answerLabelText);
    answerLabel->resize(20,10);
    answerLabel->move(imageLabel->x() + imageLabel->width() + 10, imageLabel->y()+5); // TODO

    // Setup answer input box
    inputText = new QLineEdit(this);
    inputText->resize(50,30);
    inputText->move(imageLabel->x() + imageLabel->width() + answerLabel->width()+10, imageLabel->y());

    // Setup submit answer button
    submitAnswerButton = new QPushButton(this);
    submitAnswerButton->setText("Check Answer");
    submitAnswerButton->move(answerLabel->x(), inputText->y()+inputText->height() + 10);
    submitAnswerButton->setStyleSheet("QPushButton{"
                                      "background-color: #13aa52;"
                                      "border-radius: 4px;"
                                      "border-style: none;"
                                      "font-size: 14px;"
                                      "font-weight: 600;"
                                      "color: #fff;"
                                      "margin: 0;"
                                      "min-width: 10px;"
                                      "outline: none;"
                                      "padding: 4px 8px;}"
                                      );


    // set cursor to pointer for submit answer button
    submitAnswerButton->setCursor(QCursor(Qt::PointingHandCursor));


    // Setup remaining attempts label
    remainingAttempts = new QLabel(this);
    remainingAttempts->setText("Remaining attempts: 3");
    remainingAttempts->move(submitAnswerButton->x(), submitAnswerButton->y()+submitAnswerButton->height()+10);
    remainingAttempts->setStyleSheet("font-size: 14px;");

    // Setup pass/fail banner (label)
    passFailBanner = new QLabel(this);
    passFailBanner->setHidden(true);
    passFailBanner->resize(imageLabel->width(), passFailBanner->height());
    passFailBanner->setAlignment(Qt::AlignCenter);
    passFailBanner->move(imageLabel->x(), imageLabel->y() - passFailBanner->height() - 10);

    // Setup next level button
    nextLevelButton = new QPushButton(this);
    nextLevelButton->setText("Next level");
    nextLevelButton->move(this->width()/2 - nextLevelButton->width()/2, imageLabel->y() + imageLabel->height() + nextLevelButton->height() + 10);
    nextLevelButton->setHidden(true);
    nextLevelButton->setStyleSheet("QPushButton{"
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
                                   "QPushButton:hover{"
                                   "}"
                                   );


    // set cursor to pointer
    nextLevelButton->setCursor(QCursor(Qt::PointingHandCursor));

    //Setup show info button
    showInfoButton = new QPushButton(this);
    showInfoButton->setText("Show info");
    showInfoButton->move(showInfoButton->x()+10, showInfoButton->y()+10);
    showInfoButton->setStyleSheet("QPushButton{"
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
                                  "QPushButton:hover{"
                                  "}"
                                  );


    // set cursor to pointer
    showInfoButton->setCursor(QCursor(Qt::PointingHandCursor));
}

/**
 * @brief GameWidget::paintEvent Triggers on widget paint
 * @param event
 */
void GameWidget::paintEvent(QPaintEvent *event) {
    // make this widget able to accept stylsheet
    QStyleOption opt;
    opt.initFrom(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);

}

/**
 * @brief GameWidget::passFailLevel A slot that is triggered when the user runs out of attempts
 * or gets the correct answer for a level.
 * @param passed True if the user entered correct answer, false if otherwise.
 */
void GameWidget::passFailLevel(bool passed){
    inputText->setDisabled(true);
    submitAnswerButton->setDisabled(true);
    imageLabel->setPixmap(imageSolution);
    if(passed){ // level passed, show label and next level button
        // set text to reflect level completed
        passFailBanner->setStyleSheet("color: green; font-size: 24px;");
        passFailBanner->setText("Level completed!");
        passFailBanner->show();
    } else {  // level failed, show label and next level button
        // set text to reflect level failed
        passFailBanner->setStyleSheet("color: red; font-size: 24px;");
        passFailBanner->setText("Level failed!");
        passFailBanner->show();
    }
    nextLevelButton->show();
}

/**
 * @brief GameWidget::onHidePassBanner A slot that runs when the pass/fail banner of a level needs
 * to be hidden
 */
void GameWidget::onHidePassBanner()
{
    passFailBanner->hide();
    nextLevelButton->hide();
    inputText->setDisabled(false);
    submitAnswerButton->setDisabled(false);
    imageLabel->setPixmap(image);
}

/**
 * @brief GameWidget::getInputText A slot that when triggered emits the input text in the answer
 * input box.
 */
void GameWidget::getInputText(){
    emit sendText(inputText->text());
}

/**
 * @brief GameWidget::showRemainingAttempts Updates the label that shows the remaining answer
 * attempts that a user has.
 * @param attempts The attempts remaining
 */
void GameWidget::showRemainingAttempts(int attempts){
    remainingAttempts->setText("Remaining attempts: " + QString::number(attempts));
}

GameWidget::~GameWidget(){
}
