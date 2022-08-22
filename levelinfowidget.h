#ifndef LEVELINFOWIDGET_H
#define LEVELINFOWIDGET_H

#include <QLabel>
#include <QPushButton>
#include <QWidget>

/**
 * @author Grant Nations
 * @author Blake Van Dyken
 * @author Gabe Bautista
 * @author Sebastian Ramirez
 * @author David Schneider
 * @author Charles Li
 * @brief The LevelInfoWidget class is the widget that displays the info for a
 * level. This gives the user the info necessary for a level and the option to
 * show the level.
 */
class LevelInfoWidget : public QWidget {
    Q_OBJECT
private:
    QPixmap image;

public:
    explicit LevelInfoWidget(QPixmap, QWidget *parent = nullptr);
    QLabel *imageLabel;
    QPushButton *showProblemButton;
    void paintEvent(QPaintEvent *event);

signals:
};

#endif // LEVELINFOWIDGET_H
