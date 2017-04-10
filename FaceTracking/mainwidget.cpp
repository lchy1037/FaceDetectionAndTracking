#include "mainwidget.h"

MainWidget::MainWidget(QWidget *parent)
    : QWidget(parent)
{
    setWindowTitle("Face Detection And Tracking");
//    showMaximized();

    local_video_radio = new QRadioButton("本地视频");
    camera_radio = new QRadioButton("摄像头");
    url_lineedit = new QLineEdit();
    video_label = new QLabel();
//    video_label->setPixmap(QPixmap(":/bg/bg1"));
//    video_label->setScaledContents(true);
    face_info_table = new QTableWidget();

    SetMainWindowLayout();
}

void MainWidget::SetMainWindowLayout()
{
    QHBoxLayout *url_layout = new QHBoxLayout();
    url_layout->addWidget(local_video_radio);
    url_layout->addStretch();
    url_layout->addWidget(camera_radio);
    url_layout->addStretch();
    url_layout->addWidget(url_lineedit);

    QVBoxLayout *left_layout = new QVBoxLayout();
    left_layout->addLayout(url_layout);
    left_layout->addWidget(video_label);

    QHBoxLayout *main_layout = new QHBoxLayout();
    main_layout->addLayout(left_layout);
    main_layout->setSpacing(20);
    main_layout->addWidget(face_info_table);

    setLayout(main_layout);
}

MainWidget::~MainWidget()
{

}
