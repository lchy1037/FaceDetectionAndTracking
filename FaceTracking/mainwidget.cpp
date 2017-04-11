#include "mainwidget.h"

MainWidget::MainWidget(QWidget *parent)
    : QWidget(parent)
{
    setWindowTitle("Face Detection And Tracking");

    local_video_radio = new QRadioButton("本地视频");
    camera_radio = new QRadioButton("摄像头");
    url_lineedit = new QLineEdit();
    url_lineedit->setPlaceholderText("请选择视频来源");
    start_btn = new QPushButton("开始");
    video_label = new QLabel();
    QPixmap pixmap1(":/bg/bg1");
    pixmap1 = pixmap1.scaled(QSize(720, 1080), Qt::KeepAspectRatio);
//    video_label->setGeometry(10, 20, 64, 64);
    video_label->setPixmap(pixmap1);
    face_info_table = new QTableWidget();

    SetMainWindowLayout();
    SetConnect();
}

void MainWidget::SetMainWindowLayout()
{
    QHBoxLayout *radio_layout = new QHBoxLayout();
    radio_layout->addStretch();
    radio_layout->addWidget(local_video_radio);
    radio_layout->addStretch();
    radio_layout->addWidget(camera_radio);
    radio_layout->addStretch();

    QHBoxLayout *url_layout = new QHBoxLayout();
    url_layout->addSpacing(20);
    url_layout->addWidget(url_lineedit);
    url_layout->addSpacing(20);
    url_layout->addWidget(start_btn);
    url_layout->addSpacing(20);

    QVBoxLayout *left_layout = new QVBoxLayout();
    left_layout->addLayout(radio_layout);
    left_layout->addLayout(url_layout);
    left_layout->addWidget(video_label);

    QHBoxLayout *main_layout = new QHBoxLayout();
    main_layout->addLayout(left_layout);
    main_layout->setSpacing(20);
    main_layout->addWidget(face_info_table);

    setLayout(main_layout);
}

void MainWidget::SetConnect()
{
    connect(local_video_radio, SIGNAL(toggled(bool)), this, SLOT(ChangeLineEditText(bool)));
    connect(camera_radio, SIGNAL(toggled(bool)), this, SLOT(ChangeLineEditText(bool)));
}

void MainWidget::ChangeLineEditText(bool)
{
    if(local_video_radio->isChecked()) url_lineedit->setPlaceholderText("请输入本地视频路径");
    if(camera_radio->isChecked()) url_lineedit->setPlaceholderText("请输入摄像头的地址");
    if(!local_video_radio->isChecked() && !camera_radio->isChecked())
        url_lineedit->setPlaceholderText("请选择视频来源");
}

MainWidget::~MainWidget()
{

}
