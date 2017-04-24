#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QPixmap>
#include <QRadioButton>
#include <QTableWidget>
#include <QHeaderView>
#include <QScrollBar>
#include <QHBoxLayout>
#include <QDebug>
#include <QApplication>
#include <QThread>
#include <QMessageBox>
#include <QCloseEvent>
#include <cv.h>
#include <opencv.hpp>
#include <highgui.h>
#include <features2d.hpp>
#include <fstream>

#include <face_detection.h>
#define VIDEOWIDTH 960
#define VIDEOHEIGHT 720
#define MAXROWCOUNT 9

class MainWidget : public QWidget
{
    Q_OBJECT

public:
    MainWidget(QWidget *parent = 0);
    ~MainWidget();
    void SetMainWindowLayout();
    void SetConnect();

protected:
    void StartPlayLocalVideo();
    void StartPlayCamera();
    void UpdateImage(QImage &image);
    QImage MatToQImage(cv::Mat &cvImg);
    void DetectFaceCascade(cv::Mat &img);
    void SeetaDetect(cv::Mat &img);
    void ShowInTable(const QString &name, const QString &imagePath, const QImage &shotFace, const float cosSim);
    void closeEvent(QCloseEvent *e);

private:
    QRadioButton *local_video_radio;
    QRadioButton *camera_radio;
    QLineEdit *url_lineedit;
    QPushButton *start_btn;
    QLabel *video_label;
    QTableWidget *face_info_table;
    seeta::FaceDetection *seetaDetector;

    bool stopFlag;

private slots:
    void ChangeLineEditText(bool);
    void StartPlayVideo(bool);
};

#endif // MAINWIDGET_H
