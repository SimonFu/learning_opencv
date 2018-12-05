#include <QCoreApplication>
#include <QFile>
#include <QByteArray>
#include <QDebug>

#include <opencv2/opencv.hpp>
#include <vector>

using namespace cv;
using namespace std;

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QFile file(":/res/lena.jpg");
    if(!file.open(QFile::ReadOnly))
    {
        qWarning() << "Open file failed!";
        return -1;
    }

    QByteArray ba = file.readAll();
    Mat src = imdecode(vector<char>(ba.begin(), ba.end()), IMREAD_COLOR);
    imshow("Origin image", src);

    int histSize[] = {256};
    float range[] = { 0, 256};
    const float* ranges[] = { range};
    MatND histR, histG, histB;
    int channels[] = {0};
    calcHist(&src, 1, channels, Mat(), histR, 1, histSize, ranges);
    channels[0] = 1;
    calcHist(&src, 1, channels, Mat(), histG, 1, histSize, ranges);
    channels[0] = 2;
    calcHist(&src, 1, channels, Mat(), histB, 1, histSize, ranges);

    double maxValueB = 0, maxValueG = 0, maxValueR = 0;
    minMaxLoc(histB, nullptr, &maxValueB);
    minMaxLoc(histG, nullptr, &maxValueG);
    minMaxLoc(histR, nullptr, &maxValueR);

    Mat drawImageB = Mat::zeros(Size(256 * 3, 256 * 3), CV_8UC3);
    Mat drawImageG = Mat::zeros(Size(256 * 3, 256 * 3), CV_8UC3);
    Mat drawImageR = Mat::zeros(Size(256 * 3, 256 * 3), CV_8UC3);
    for(int i = 0; i < histSize[0]; ++i)
    {
        double valueB = histB.at<float>(i) / maxValueB;
        double valueG = histB.at<float>(i) / maxValueG;
        double valueR = histB.at<float>(i) / maxValueR;

        if(valueB != 0.0)
        {
            rectangle(drawImageB, Point(i*3, (1.0 - valueB) * drawImageB.rows),
                      Point((i+1)*3, drawImageB.rows), Scalar(255, 0, 0));
        }

        if(valueG != 0.0)
        {
            rectangle(drawImageG, Point(i*3, (1.0 - valueG) * drawImageG.rows),
                      Point((i+1)*3, drawImageG.rows), Scalar(0, 255, 0));
        }

        if(valueR != 0.0)
        {
            rectangle(drawImageR, Point(i*3, (1.0 - valueR) * drawImageR.rows),
                      Point((i+1)*3, drawImageR.rows), Scalar(0, 0, 255));
        }
    }

    imshow("Hist image blue", drawImageB);
    imshow("Hist image green", drawImageG);
    imshow("Hist image red", drawImageR);

    return a.exec();
}
