#include "autojump.h"
#include <QDebug>
autojump::autojump(QObject *parent)
    : QThread(parent)
{

}
autojump::~autojump()
{
}
bool autojump::stop = true;
void autojump::run()
{
   while (!stop)
   {
      double Dtemp;
      adbgetimage();
      srcImage=imread("screenshot.jpg");
      Dtemp=getdistance(srcImage);
      image=Mat2QImage(srcImage);
      imgScaled=image.scaled(360,640,Qt::KeepAspectRatio);
      emit ImageProcessFinished(imgScaled ,Dtemp);
      adbrun(Dtemp/scale);
      msleep(3500);
     }
     this->exec();
}

void autojump::adbrun(double num)
{
    QProcess * po = new QProcess();
    QString program = "adb.exe";
    po->startDetached(program,QStringList()<<"shell"<<"input"<<"swipe"<<"540"<<"1500"<<"540"<<"1500"<<QString("%1").arg((int)(num*100)));
    po->waitForFinished();
}
void autojump::adbgetimage()
{

    QProcess * po = new QProcess();
    QString program = "adb.exe";
    po->start(program,QStringList()<<"shell"<<"/system/bin/screencap"<<"-p"<<"/sdcard/screenshot.jpg");
    po->waitForFinished();
    po->start(program,QStringList()<<"pull"<<"/sdcard/screenshot.jpg"<<"screenshot.jpg");
    po->waitForFinished();
}

void autojump::openctrl()
{
   stop=false;
}
void autojump::closectrl()
{
    stop=true;
}
void autojump::setscale(double temp)
{
    scale=temp;
}
QImage autojump::Mat2QImage(Mat cvImg)
{
    QImage qImg;
    if(cvImg.channels()==3)                             //3 channels color image
    {

        cv::cvtColor(cvImg,cvImg,COLOR_BGR2RGB);
        qImg =QImage((const unsigned char*)(cvImg.data),
                    cvImg.cols, cvImg.rows,
                    cvImg.cols*cvImg.channels(),
                    QImage::Format_RGB888);
    }
    else if(cvImg.channels()==1)                    //grayscale image
    {
        qImg =QImage((const unsigned char*)(cvImg.data),
                    cvImg.cols,cvImg.rows,
                    cvImg.cols*cvImg.channels(),
                    QImage::Format_Indexed8);
    }
    else
    {
        qImg =QImage((const unsigned char*)(cvImg.data),
                    cvImg.cols,cvImg.rows,
                    cvImg.cols*cvImg.channels(),
                    QImage::Format_RGB888);
    }
    return qImg;

}
