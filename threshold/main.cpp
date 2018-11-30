#include <QCoreApplication>
#include <QFile>
#include <QDebug>

#include <opencv2/opencv.hpp>
#include <vector>

using namespace cv;
using namespace std;

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QFile file("://res/threshold.png");
    file.open(QFile::ReadOnly);
    QByteArray buf = file.readAll();
    Mat src = imdecode(std::vector<uchar>(buf.begin(), buf.end()), IMREAD_GRAYSCALE);
    if(src.empty()) {
        qWarning() << "Open image file failed!";
        return -1;
    }

    imshow("Origin image", src);

    Mat dst1, dst2, dst3, dst4, dst5, dst6;
    double threshold, max;
    threshold = 128;
    max = 255;
    cv::threshold(src, dst1, threshold, max, THRESH_BINARY);
    imshow("THRESH_BINARY", dst1);
    cv::threshold(src, dst2, threshold, max, THRESH_BINARY_INV);
    imshow("THRESH_BINARY_INV", dst2);
    cv::threshold(src, dst3, threshold, max, THRESH_TRUNC);
    imshow("THRESH_TRUNC", dst3);
    cv::threshold(src, dst4, threshold, max, THRESH_TOZERO);
    imshow("THRESH_TOZERO", dst4);
    cv::threshold(src, dst5, threshold, max, THRESH_TOZERO_INV);
    imshow("THRESH_TOZERO_INV", dst5);
    cv::threshold(src, dst6, threshold, max, THRESH_BINARY | THRESH_OTSU);
    imshow("THRESH_BINARY | THRESH_OTSU", dst6);

    return a.exec();
}
