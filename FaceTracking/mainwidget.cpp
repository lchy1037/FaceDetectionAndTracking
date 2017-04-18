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
       DetectFaceCascade(frame);
       QImage image = MatToQImage(frame);
       UpdateImage(image);
       QThread::msleep(40);
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
    for(std::vector<cv::Rect>::const_iterator iter = faces.begin(); iter != faces.end(); ++iter){
        cv::Mat imgRoi;
        QImage qimgRoi;
        img(*iter).copyTo(imgRoi);
        qimgRoi = MatToQImage(imgRoi);
        ShowInTable("1_1", "./faces/1_0.jpg", qimgRoi, 0.97);
        cv::rectangle(img, *iter, cv::Scalar(0, 0, 255), 2, 8);
    }
}

void MainWidget::ShowInTable(const QString &name, const QString &imagePath, const QImage &shotFace, const float cosSim)
{
    int rowHeight = 90;

    QTableWidgetItem *item0 = new QTableWidgetItem();
    item0->setText(name);
    item0->setTextAlignment(Qt::AlignCenter);

    QLabel *item1 = new QLabel();
    item1->setPixmap(QPixmap(imagePath).scaled(rowHeight - 20, rowHeight - 20));
    item1->setAlignment(Qt::AlignHCenter);

    QLabel *item2 = new QLabel();
    item2->setPixmap(QPixmap::fromImage(shotFace).scaled(rowHeight - 20, rowHeight - 20));
    item2->setAlignment(Qt::AlignHCenter);

    QTableWidgetItem *item3 = new QTableWidgetItem();
    item3->setText(QString("%1").arg(cosSim));
    item3->setTextAlignment(Qt::AlignCenter);

    int rowCount = face_info_table->rowCount();
    if(rowCount >= MAXROWCOUNT){
        face_info_table->removeRow(0);
    }
    face_info_table->insertRow(rowCount);
    face_info_table->setRowHeight(rowCount, rowHeight);

    face_info_table->setItem(rowCount, 0, item0);
    face_info_table->setCellWidget(rowCount, 1, item1);
    face_info_table->setCellWidget(rowCount, 2, item2);
    face_info_table->setItem(rowCount, 3, item3);

    face_info_table->verticalScrollBar()->setValue(face_info_table->rowCount());
}

void MainWidget::ReadCsv(QString &filename, std::vector<cv::Mat> &images, std::vector<int> &labels, char separator)
{
    std::ifstream file(filename.toStdString().c_str(), std::ios::in);
    if(!file){
        qDebug() << "Cann't find the file" << filename;
        return;
    }
    std::string line, path, classLabel;
    while(std::getline(file, line)){
        std::stringstream liness(line);
        std::getline(liness, path, separator);
        std::getline(liness, classLabel);
        if(!path.empty() && !classLabel.empty()){
            images.push_back(cv::imread(path, 0));
            labels.push_back(atoi(classLabel.c_str()));
        }
    }
}

void MainWidget::TrainData(QString &fnCsv)
{
    std::vector<cv::Mat> images;
    std::vector<int> labels;
    ReadCsv(fnCsv, images, labels);
    if(images.size() <= 1){
        qDebug() << "Training images number is less than 2. Please add more images to training set.";
        return;
    }
}

MainWidget::~MainWidget()
{
}
