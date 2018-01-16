#ifndef AUTOJUMP_H
#define AUTOJUMP_H
#include <QObject>
#include <QThread>
#include <qprocess>
#include "jumpp.h"
#include <QImage>
using namespace std;
using namespace cv;
class autojump : public QThread
{
    Q_OBJECT
public:
    autojump(QObject *parent = 0);
    ~autojump();
   static bool stop;
   QImage Mat2QImage(Mat cvImg);
   void setscale(double);
   void adbrun(double num);
   void adbgetimage();
public slots:
    void openctrl();
    void closectrl();

private:
     void run();
    QImage image;
    QImage imgScaled;
    Mat srcImage;
    double scale;
signals:
    void ImageProcessFinished(QImage ,double);
};

#endif // AUTOJUMP_H
