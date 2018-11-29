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
    Mat src = imdecode(vector<char>(ba.begin(), ba.end()), IMREAD_GRAYSCALE);
    imshow("Origin image", src);

    threshold(src, src, 120, 255, THRESH_BINARY);
    imshow("Threshold image", src);

    Mat dst1, dst2;
    dilate(src, dst1, Mat());
    imshow("Dilate image", dst1);

    erode(src, dst2, Mat());
    imshow("Erosion image", dst2);

    return a.exec();
}
