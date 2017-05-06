#include "mainwidget.h"

MainWidget::MainWidget(QWidget *parent)
    : QWidget(parent)
{
    setWindowTitle("Face Detection And Tracking");
    showMaximized();

    local_video_radio = new QRadioButton("本地视频");
    camera_radio = new QRadioButton("摄像头");
    url_lineedit = new QLineEdit();
    url_lineedit->setPlaceholderText("请选择视频来源");
    start_btn = new QPushButton("打开");
    video_label = new QLabel();
    video_label->resize(QSize(VIDEOWIDTH, VIDEOHEIGHT));
    QPixmap pixmap1(":/bg/bg1");
    pixmap1 = pixmap1.scaled(video_label->size());
    video_label->setPixmap(pixmap1);
//    video_label->setScaledContents(true);
    face_info_table = new QTableWidget();
    face_info_table->setColumnCount(4);
    face_info_table->setColumnWidth(0, 75);
    face_info_table->setColumnWidth(1, 100);
    face_info_table->setColumnWidth(2, 100);
    face_info_table->setColumnWidth(3, 50);
//    face_info_table->resize(1080, 325);
    face_info_table->setSelectionBehavior(QAbstractItemView::SelectRows);
    face_info_table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    face_info_table->verticalHeader()->setVisible(false);
    face_info_table->setShowGrid(true);
//    face_info_table->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
//    face_info_table->horizontalScrollBar()->setEnabled(false);
    QStringList header;
    header << tr("姓名") << tr("模板图") << tr("抓拍图") << tr("相似度");
    face_info_table->setHorizontalHeaderLabels(header);

    seetaDetector = new seeta::FaceDetection("D:/Studio/Project/git/FaceDetectionAndTracking/FaceTracking/SeetaFaceDetection/model/seeta_fd_frontal_v1.0.bin");
    seetaDetector->SetMinFaceSize(40);
    seetaDetector->SetScoreThresh(2.f);
    seetaDetector->SetImagePyramidScaleFactor(0.8f);
    seetaDetector->SetWindowStep(4, 4);

    stopFlag = false;
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
    if(start_btn->text() == tr("打开")){
        if(local_video_radio->isChecked()){
            start_btn->setText(tr("关闭"));
            StartPlayLocalVideo();
        }
        if(camera_radio->isChecked()){
            start_btn->setText(tr("关闭"));
            StartPlayCamera();
        }
    }else{
        start_btn->setText(tr("打开"));
        stopFlag = true;
    }
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
        if(stopFlag){
            QPixmap pixmap1(":/bg/bg1");
            pixmap1 = pixmap1.scaled(video_label->size());
            video_label->setPixmap(pixmap1);
            break;
        }
        cv::Mat frame;
        if(!capture.read(frame)) break;
        SeetaDetect(frame);
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
    // 打开网络摄像头 string = "rtsp://192.168.1.156:554/ch1/1"
    if(url_lineedit->text() != QString("")){
        if(!capture.open(url_lineedit->text().toStdString())){
            qDebug() << "Cann't open camera" << url_lineedit->text();
            return;
        }
    }else{
        if(!capture.open(0)){
            qDebug() << "Cann't open camera.";
            return;
        }
    }

    while(1){
        if(stopFlag){
            QPixmap pixmap1(":/bg/bg1");
            pixmap1 = pixmap1.scaled(video_label->size());
            video_label->setPixmap(pixmap1);
            break;
        }
       cv::Mat frame;
       capture >> frame;
       SeetaDetect(frame);
       QImage image = MatToQImage(frame);
       UpdateImage(image);
//       QThread::msleep(40);
       QApplication::processEvents();
    }
    capture.release();
}

QImage MainWidget::MatToQImage(cv::Mat &cvImg)
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

void MainWidget::DetectFaceCascade(cv::Mat &img)
{
    QString xmlPath = "C:/InstallFiles/OpenCV3.1.0/build/etc/haarcascades/haarcascade_frontalface_default.xml";
    cv::CascadeClassifier ccf;
    if(!ccf.load(xmlPath.toStdString())){
        qDebug()<<"Failed to load" << xmlPath;
        return;
    }
    std::vector<cv::Rect> faces;
    cv::Mat gray;
    cv::cvtColor(img, gray, CV_BGR2GRAY);
    cv::equalizeHist(gray, gray);
    ccf.detectMultiScale(gray, faces, 1.1, 3, 0, cv::Size(10, 10), cv::Size(100, 100));
    for(std::vector<cv::Rect>::iterator iter = faces.begin(); iter != faces.end(); ++iter){
        if((*iter).x < 0) (*iter).x = 0;
        if((*iter).y < 0) (*iter).y = 0;
        if((*iter).y + (*iter).height > gray.rows) (*iter).height = gray.rows - (*iter).y - 1;
        if((*iter).x + (*iter).width > gray.cols) (*iter).width = gray.cols - (*iter).x - 1;
        cv::Mat imgRoi;
        QImage qimgRoi;
        img(*iter).copyTo(imgRoi);
        qimgRoi = MatToQImage(imgRoi);
        ShowInTable("1_1", "./faces/1_0.jpg", qimgRoi, 0.97);
        cv::rectangle(img, *iter, cv::Scalar(0, 0, 255), 2, 8);
        cv::putText(img, "1_1", cv::Point((*iter).x, (*iter).y-5), CV_FONT_HERSHEY_TRIPLEX, 0.5, cv::Scalar(0, 0, 255));
    }
}

void MainWidget::SeetaDetect(cv::Mat &img)
{
    cv::Mat imgGray;
    if(img.channels() != 1)
        cv::cvtColor(img, imgGray, cv::COLOR_RGB2GRAY);
    else
        imgGray = img;
    seeta::ImageData imgData;
    imgData.data = imgGray.data;
    imgData.width = imgGray.cols;
    imgData.height = imgGray.rows;
    imgData.num_channels = 1;
    long t0 = cv::getTickCount();
    std::vector<seeta::FaceInfo> faces = seetaDetector->Detect(imgData);
    long t1 = cv::getTickCount();
    double secs = (t1-t0)/cv::getTickFrequency();
    qDebug() << "Detector takes " << secs << " seconds"  << imgData.width << imgData.height;
    cv::Rect faceRect;
    int numFace = faces.size();
    for(int i = 0; i != numFace; ++i){
        faceRect.x = faces[i].bbox.x;
        faceRect.y = faces[i].bbox.y;
        faceRect.width = faces[i].bbox.width;
        faceRect.height = faces[i].bbox.height;

        if(faceRect.x < 0) faceRect.x = 0;
        if(faceRect.y < 0) faceRect.y = 0;
        if(faceRect.y + faceRect.height > imgGray.rows) faceRect.height = imgGray.rows - faceRect.y - 1;
        if(faceRect.x + faceRect.width > imgGray.cols) faceRect.width = imgGray.cols - faceRect.x - 1;
        cv::Mat imgRoi;
        QImage qimgRoi;
        img(faceRect).copyTo(imgRoi);
        qimgRoi = MatToQImage(imgRoi);
        ShowInTable("1_1", "./faces/1_0.jpg", qimgRoi, 0.97);

        cv::rectangle(img, faceRect, CV_RGB(0, 0, 255), 2, 8, 0);
        cv::putText(img, "1_1", cv::Point(faceRect.x, faceRect.y - 5), CV_FONT_HERSHEY_TRIPLEX, 0.5, CV_RGB(0, 0, 255));
    }
}

void MainWidget::ShowInTable(const QString &name, const QString &imagePath, const QImage &shotFace, const float cosSim)
{
    int rowHeight = 90;

    QTableWidgetItem *item0 = new QTableWidgetItem();
    item0->setText(name);
    item0->setTextAlignment(Qt::AlignCenter);

    QTableWidgetItem *item1 = new QTableWidgetItem();
    item1->setData(Qt::DecorationRole, QPixmap(imagePath).scaled(rowHeight-20, rowHeight-20));

    QTableWidgetItem *item2 = new QTableWidgetItem();
    item2->setData(Qt::DecorationRole, QPixmap::fromImage(shotFace).scaled(rowHeight-20, rowHeight-20));

    QTableWidgetItem *item3 = new QTableWidgetItem();
    item3->setText(QString::number(cosSim, 'f', 3));
    item3->setTextAlignment(Qt::AlignCenter);

    int rowCount = face_info_table->rowCount();
    if(rowCount >= MAXROWCOUNT){
        face_info_table->removeRow(rowCount - 1);
    }
    face_info_table->insertRow(0);
    face_info_table->setRowHeight(0, rowHeight);

    face_info_table->setItem(0, 0, item0);
    face_info_table->setItem(0, 1, item1);
    face_info_table->setItem(0, 2, item2);
    face_info_table->setItem(0, 3, item3);
}

void MainWidget::closeEvent(QCloseEvent *event)
{
    switch(QMessageBox::information(this, tr("关闭提示"), tr("确认关闭？"), QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes))
    {
    case QMessageBox::Yes:
        qApp->quit();
        break;
    case QMessageBox::No:
        event->ignore();
        break;
    case QMessageBox::Cancel:
        event->ignore();
        break;
    default:
        event->ignore();
        break;
    }
}

MainWidget::~MainWidget()
{
}
