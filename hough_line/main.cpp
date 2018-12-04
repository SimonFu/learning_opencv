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
    QFile file(":/res/building.jpg");
    if(!file.open(QFile::ReadOnly))
    {
        qWarning() << "Open file failed!";
        return -1;
    }

    QByteArray ba = file.readAll();
    Mat src = imdecode(vector<char>(ba.begin(), ba.end()), IMREAD_GRAYSCALE);
    imshow("Origin image", src);

    Mat edgeImg;
    Canny(src, edgeImg, 50, 200, 3, true);
    imshow("Detected edge", edgeImg);

    Mat coloredDst;
    cvtColor(edgeImg, coloredDst, COLOR_GRAY2BGR);

    vector<Vec4i> lines;
    HoughLinesP(edgeImg, lines, 1, CV_PI / 180, 80, 30, 10);
    for(auto& l : lines) {
        line(coloredDst, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(255, 0, 0), 2, 8);
    }

    imshow("Detected lines", coloredDst);

    return a.exec();
}
