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

    Mat tmp;
    Mat padded;
    int m = getOptimalDFTSize(src.rows);
    int n = getOptimalDFTSize(src.cols);
    copyMakeBorder(src, padded, 0, m - src.rows, 0, n - src.cols, BORDER_CONSTANT, Scalar::all(0));
    //imshow("Padded image", padded);

    Mat planes[] = {Mat_<float>(padded), Mat::zeros(padded.size(), CV_32F)};
    Mat complexImg;
    merge(planes, 2, complexImg);
    dft(complexImg, complexImg);
    split(complexImg, planes);

    magnitude(planes[0], planes[1], planes[0]);

    Mat magI = planes[0];
    magI += Scalar::all(1);
    log(magI, magI);
    magI = magI(Rect(0, 0, magI.cols & (-2), magI.rows & (-2)));

    normalize(magI, magI, 0, 1, NORM_MINMAX);

    Mat orgFImg;
    tmp = magI * 255;
    tmp.convertTo(orgFImg, CV_8U);
    imshow("Original Fourier image", orgFImg);

    int cx = magI.cols / 2;
    int cy = magI.rows / 2;

    Mat q0 = magI(Rect(0, 0, cx, cy));
    Mat q1 = magI(Rect(cx, 0, cx, cy));
    Mat q2 = magI(Rect(0, cy, cx, cy));
    Mat q3 = magI(Rect(cx, cy, cx, cy));

    q0.copyTo(tmp);
    q3.copyTo(q0);
    tmp.copyTo(q3);
    q1.copyTo(tmp);
    q2.copyTo(q1);
    tmp.copyTo(q2);

    Mat fImg;
    tmp = magI * 255;
    tmp.convertTo(fImg, CV_8U);

    imshow("Fourier image", fImg);

    Mat ifft;
    idft(complexImg,ifft,DFT_REAL_OUTPUT);
    normalize(ifft, ifft, 0, 1, NORM_MINMAX);
    tmp = ifft * 255;
    tmp.convertTo(fImg, CV_8U);
    imshow("Reverse Fourier image", fImg);

    return a.exec();
}
