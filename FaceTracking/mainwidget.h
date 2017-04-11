#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QPixmap>
#include <QRadioButton>
#include <QTableWidget>
#include <QHBoxLayout>
//#include <cv.h>
//#include <highgui.h>

class MainWidget : public QWidget
{
    Q_OBJECT

public:
    MainWidget(QWidget *parent = 0);
    ~MainWidget();
    void SetMainWindowLayout();
    void SetConnect();

private:
    QRadioButton *local_video_radio;
    QRadioButton *camera_radio;
    QLineEdit *url_lineedit;
    QPushButton *start_btn;
    QLabel *video_label;
    QTableWidget *face_info_table;

private slots:
    void ChangeLineEditText(bool);

};

#endif // MAINWIDGET_H
