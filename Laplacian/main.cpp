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

    Mat dst1, dst2, dst3;
    Laplacian(src, dst1, CV_16S, 3);
    convertScaleAbs(dst1, dst2);
    imshow("Laplacian image", dst2);

    threshold(dst2, dst3, 0, 255, THRESH_BINARY | THRESH_OTSU);
    imshow("Threshold image", dst3);

    return a.exec();
}
