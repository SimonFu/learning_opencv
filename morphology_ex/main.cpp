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
    Mat src = imdecode(std::vector<uchar>(buf.begin(), buf.end()), IMREAD_GRAYSCALE);
    imshow("Original image", src);

    Mat src1;
    threshold(src, src1, 0, 255, THRESH_BINARY | THRESH_OTSU);
    imshow("threshold image", src1);

    Mat dst1, dst2, dst3;
    morphologyEx(src1, dst1, MORPH_CLOSE, Mat());
    imshow("Morphology close image", dst1);

    morphologyEx(src1, dst2, MORPH_OPEN, Mat());
    imshow("Morphology open image", dst2);

    Mat kenerl = getStructuringElement(MORPH_RECT, Size(5, 5));
    morphologyEx(src1, dst3, MORPH_OPEN, kenerl);
    imshow("Morphology open image 2", dst3);

    return a.exec();
}
