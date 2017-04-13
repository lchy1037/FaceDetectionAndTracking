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
    video_label->resize(QSize(VIDEOWIDTH, VIDEOHEIGHT));
    QPixmap pixmap1(":/bg/bg1");
    pixmap1 = pixmap1.scaled(video_label->size());
    video_label->setPixmap(pixmap1);
    video_label->setScaledContents(true);
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
    connect(start_btn, SIGNAL(clicked(bool)), this, SLOT(StartPlayVideo(bool)));
}

void MainWidget::ChangeLineEditText(bool)
{
    url_lineedit->clear();
//    if(local_video_radio->isChecked()) url_lineedit->setPlaceholderText("请输入本地视频路径");
    if(local_video_radio->isChecked()) url_lineedit->setText("C:/Users/luan/Desktop/movie.mp4");
    if(camera_radio->isChecked()) url_lineedit->setPlaceholderText("请输入摄像头的地址");
    if(!local_video_radio->isChecked() && !camera_radio->isChecked())
        url_lineedit->setPlaceholderText("请选择视频来源");
}

void MainWidget::StartPlayVideo(bool)
{
    if(local_video_radio->isChecked()) StartPlayLocalVideo();
    if(camera_radio->isChecked()) StartPlayCamera();
}

void MainWidget::UpdateImage(QImage &image)
{
    QPixmap pixmap;
    pixmap = pixmap.fromImage(image);
    pixmap = pixmap.scaled(QSize(VIDEOWIDTH, VIDEOHEIGHT));
    video_label->setPixmap(pixmap);
}

void MainWidget::StartPlayLocalVideo()
{
    cv::VideoCapture capture;
    if(!capture.open(url_lineedit->text().toLatin1().data())){
        qDebug() << "Cann't open file" << url_lineedit->text();
        return;
    }
    double frame_rate = capture.get(CV_CAP_PROP_FPS);
    int delay = 1000 / frame_rate;
    while(1){
        cv::Mat frame;
        if(!capture.read(frame)) break;
        QImage image = MatToQImage(frame);
        UpdateImage(image);
        QThread::msleep(delay);
        QApplication::processEvents();
    }
    capture.release();
}

void MainWidget::StartPlayCamera()
{
    cv::VideoCapture capture;
    if(!capture.open(0)){
        qDebug() << "Cann't open camera.";
        return;
    }
//    // 打开网络摄像头 string = "rtsp://192.168.1.156:554/ch1/1"
//    if(!capture.open(url_lineedit->text().toStdString())){
//        qDebug() << "Cann't open camera" << url_lineedit->text();
//        return;
//    }
    while(1){
       cv::Mat frame;
       capture >> frame;
       QImage image = MatToQImage(frame);
       UpdateImage(image);
       QThread::msleep(40);
       QApplication::processEvents();
    }
    capture.release();
}

QImage MainWidget::MatToQImage(Mat &cvImg)
{
    QImage qImg;
    if(cvImg.channels()==3){
        cv::cvtColor(cvImg,cvImg,CV_BGR2RGB);
        qImg =QImage((const unsigned char*)(cvImg.data),
                    cvImg.cols, cvImg.rows,
                    cvImg.cols*cvImg.channels(),
                    QImage::Format_RGB888);
    }else if(cvImg.channels()==1){
        qImg =QImage((const unsigned char*)(cvImg.data),
                    cvImg.cols,cvImg.rows,
                    cvImg.cols*cvImg.channels(),
                    QImage::Format_Indexed8);
    }else{
        qImg =QImage((const unsigned char*)(cvImg.data),
                    cvImg.cols,cvImg.rows,
                    cvImg.cols*cvImg.channels(),
                    QImage::Format_RGB888);
    }
    return qImg;
}

MainWidget::~MainWidget()
{
}
