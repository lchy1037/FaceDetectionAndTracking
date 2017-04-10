#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QPixmap>
#include <QRadioButton>
#include <QTableWidget>
#include <QHBoxLayout>

class MainWidget : public QWidget
{
    Q_OBJECT

public:
    MainWidget(QWidget *parent = 0);
    ~MainWidget();
    void SetMainWindowLayout();

private:
    QRadioButton *local_video_radio;
    QRadioButton *camera_radio;
    QLineEdit *url_lineedit;
    QLabel *video_label;
    QTableWidget *face_info_table;
};

#endif // MAINWIDGET_H
