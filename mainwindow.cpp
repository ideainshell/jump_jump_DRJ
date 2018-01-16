#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QDebug"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    ui->setupUi(this);

     m_auto = new autojump(this);
     connect(m_auto,SIGNAL(ImageProcessFinished(QImage,double)),this,SLOT(showadd(QImage , double)));
    Mat srcImage;
    QImage image,imgScaled;
     srcImage= cv::imread("start.jpg");
     image=m_auto->Mat2QImage(srcImage);
     imgScaled=image.scaled(360,640,Qt::KeepAspectRatio);
     ui->label->setPixmap(QPixmap::fromImage(imgScaled));
}

MainWindow::~MainWindow()
{
     m_auto->terminate();
    delete ui;
}
void MainWindow::showadd(QImage imgScaled, double data)
{
    ui->label->setPixmap(QPixmap::fromImage(imgScaled));
    ui->label_2->setText(QString("%1").arg(data));
}
void MainWindow::on_pushButton_clicked()
{
    m_auto->setscale(ui->doubleSpinBox->value());
    m_auto->openctrl();
    m_auto->start();
    ui->pushButton->setDisabled(true);
    ui->doubleSpinBox->setDisabled(true);
}

void MainWindow::on_pushButton_2_clicked()
{
    ui->pushButton->setEnabled(true);
    ui->doubleSpinBox->setEnabled(true);
    m_auto->closectrl();
    m_auto->terminate();
    m_auto->wait();
}
