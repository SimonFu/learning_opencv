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
    QFile file(":/res/stuff.jpg");
    if(!file.open(QFile::ReadOnly))
    {
        qWarning() << "Open file failed!";
        return -1;
    }

    QByteArray ba = file.readAll();
    Mat src = imdecode(vector<char>(ba.begin(), ba.end()), IMREAD_GRAYSCALE);
    imshow("Origin image", src);

    GaussianBlur(src, src, Size(3, 3), 0, 0);
    imshow("Blur image", src);

    vector<Vec3f> circles;
    HoughCircles(src, circles, HOUGH_GRADIENT, 2, src.cols / 10);
    for(auto& c : circles)
    {
        circle(src, Point(cvRound(c[0]), cvRound(c[1])), cvRound(c[2]), Scalar(127, 0, 0), 2, LINE_AA);
    }

    imshow("Detected image", src);

    return a.exec();
}
