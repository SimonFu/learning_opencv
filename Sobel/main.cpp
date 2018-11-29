#include <QCoreApplication>
#include <QDebug>
#include <QByteArray>
#include <QFile>

#include <opencv2/opencv.hpp>
#include <vector>

using namespace cv;
using namespace std;

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QFile file(":/res/lena.jpg");
    file.open(QFile::ReadOnly);
    QByteArray buf = file.readAll();
    Mat src = imdecode(vector<uchar>(buf.begin(), buf.end()), IMREAD_GRAYSCALE);
    imshow("Original image", src);
    GaussianBlur(src, src, Size(3, 3), 0);
    imshow("Gaussion blur image", src);

    Mat gray_x, gray_y, gray_x_abs, gray_y_abs;
    Sobel(src, gray_x, CV_16S, 1, 0);
    convertScaleAbs(gray_x, gray_x_abs);
    imshow("Gray x image", gray_x_abs);

    Sobel(src, gray_y, CV_16S, 0, 1);
    convertScaleAbs(gray_y, gray_y_abs);
    imshow("Gray y image", gray_y_abs);

    Mat dst1, dst2;
    addWeighted(gray_x_abs, 0.5, gray_y_abs, 0.5, 0, dst1);
    imshow("Gray xy image", dst1);

    threshold(dst1, dst2, 0, 255, THRESH_BINARY|THRESH_OTSU);
    imshow("Threshold image", dst2);

    return a.exec();
}
