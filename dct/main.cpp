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

    int height = src.rows;
    int width = src.cols;

    Mat yuvImage(src.size(), CV_8UC3);
    cvtColor(src, yuvImage, COLOR_RGB2YUV);

    Mat dstImage(src.size(), CV_64FC3);

    vector<Mat> channels;
    split(yuvImage, channels);

    Mat y = channels.at(0); imshow("y", y);
    Mat u = channels.at(1); imshow("u", u);
    Mat v = channels.at(2); imshow("v", v);

    Mat dctY(src.size(), CV_64FC1);
    Mat dctU(src.size(), CV_64FC1);
    Mat dctV(src.size(), CV_64FC1);

    dct(Mat_<double>(y), dctY);
    dct(Mat_<double>(u), dctU);
    dct(Mat_<double>(v), dctV);

    Mat idctY(src.size(), CV_64FC1);
    Mat idctU(src.size(), CV_64FC1);
    Mat idctV(src.size(), CV_64FC1);

    idct(dctY, idctY);
    idct(dctU, idctU);
    idct(dctV, idctV);

    channels[0] = idctY;
    channels[1] = idctU;
    channels[2] = idctV;

    merge(channels,dstImage);

    Mat tmpYUV, tmpRGB;
    dstImage.convertTo(tmpYUV, CV_8UC3);
    cvtColor(tmpYUV, tmpRGB, COLOR_YUV2RGB);
    imshow("dct image", tmpRGB);

    return a.exec();
}
