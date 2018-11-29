#include <QCoreApplication>
#include <QTimer>
#include <QDebug>

#include <opencv2/opencv.hpp>

using namespace cv;

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    if(argc < 2)
    {
        qInfo() << "Usage: " << argv[0] << " [video file path]";
        return -1;
    }

    VideoCapture capture(argv[1]);
    namedWindow("Video player");

    QTimer timer;

    timer.setInterval(33);
    QObject::connect(&timer, &QTimer::timeout, [&](){
        Mat frame;
        capture >> frame;
        if (frame.empty())
            return;
        imshow("Video player", frame);
    });
    timer.start();

    QObject::connect(&a, &QCoreApplication::aboutToQuit, [&]{
        qDebug() << "aboutToQuit";
        capture.release();
        destroyWindow("Video player");
    });

    return a.exec();
}
